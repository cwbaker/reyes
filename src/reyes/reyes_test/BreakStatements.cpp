
#include "CaptureErrorPolicy.hpp"
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Scope.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/ErrorCode.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

#include <reyes/Debugger.hpp>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

using std::string;

static const float TOLERANCE = 0.01f;

SUITE( BreakStatements )
{
    struct BreakStatementTest
    {
        float x [4];
        float y [4];
     
        BreakStatementTest()
        : x{}
        , y{}
        {
        }

        void test( const char* source )
        {
            ErrorPolicy error_policy;
            test( source, error_policy );
        }
        
        void test( const char* source, ErrorPolicy& error_policy )
        {
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                .add_surface_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
            ;            

            Shader shader( source, source + strlen(source), symbol_table, error_policy );

            Grid grid;
            grid.set_symbols( shader.symbols() );
            grid.resize( 2, 2 );            
            grid.zero();

            float* xx = grid.float_value( "x" );
            float* yy = grid.float_value( "y" );
            if ( xx && yy )
            {
                memcpy( xx, x, sizeof(float) * grid.size() );
                memcpy( yy, y, sizeof(float) * grid.size() );
                VirtualMachine virtual_machine;
                virtual_machine.initialize( grid, shader );
                virtual_machine.shade( grid, shader );
                memcpy( x, xx, sizeof(float) * grid.size() );
                memcpy( y, yy, sizeof(float) * grid.size() );
            }
        }
    };

    TEST_FIXTURE( BreakStatementTest, break_statement_in_for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface break_statement_in_for_loop() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       y += x; \n"
            "       break; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_in_for_loop_with_level )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface break_statement_in_for_loop_with_level() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       y += x; \n"
            "       break 1; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_in_for_loop_with_out_of_range_level )
    {
        const char* source =
            "surface break_statement_in_for_loop_with_out_of_range_level() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       break 2; \n"
            "   } \n"
            "}"
        ;

        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        REQUIRE CHECK( !error_policy.errors.empty() && !error_policy.messages.empty() );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Break to a level outside of a loop" );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_in_while_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface break_statement_in_while_loop() { \n"
            "   uniform float i = 0; \n"
            "   while ( i < 4 ) { \n"
            "       y += x; \n"
            "       i += 1; \n"
            "       break; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_in_while_loop_with_level )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface break_statement_in_while_loop() { \n"
            "   uniform float i = 0; \n"
            "   while ( i < 4 ) { \n"
            "       y += x; \n"
            "       i += 1; \n"
            "       break 1; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_in_while_loop_with_out_of_range_level )
    {
        const char* source =
            "surface break_statement_in_while_loop_with_out_of_range_level() { \n"
            "   uniform float i = 0; \n"
            "   while ( i < 4 ) { \n"
            "       break 2; \n"
            "       i += 1; \n"
            "   } \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Break to a level outside of a loop" );
    }

    TEST_FIXTURE( BreakStatementTest, break_out_of_first_level_of_nested_for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface break_out_of_first_level_of_nested_for_loop() { \n"
            "   uniform float i, j; \n"
            "   for ( j = 0; j < 4; j += 1 ) { \n"
            "       for ( i = 0; i < 4; i += 1 ) { \n"
            "           y += x; \n"
            "           break 1; \n"
            "       } \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( BreakStatementTest, break_out_of_second_level_of_nested_for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface break_out_of_second_level_of_nested_for_loop() { \n"
            "   uniform float i, j; \n"
            "   for ( j = 0; j < 4; j += 1 ) { \n"
            "       for ( i = 0; i < 4; i += 1 ) { \n"
            "           y += x; \n"
            "           break 2; \n"
            "       } \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_outside_of_loop )
    {
        const char* source =
            "surface break_statement_outside_of_loop() { \n"
            "   break; \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Break outside of a loop" );
    }

    TEST_FIXTURE( BreakStatementTest, break_statement_with_level_outside_of_loop )
    {
        const char* source =
            "surface break_statement_outside_of_loop() { \n"
            "   break 2; \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Break outside of a loop" );
    }
}
