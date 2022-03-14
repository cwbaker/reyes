
#include "CaptureErrorPolicy.hpp"
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Scope.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/ErrorCode.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

static const float TOLERANCE = 0.01f;

SUITE( ContinueStatements )
{
    struct ContinueStatementTest
    {
        float x [4];
        float y [4];
     
        ContinueStatementTest()
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
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
            ;            

            Shader shader( source, source + strlen(source), symbol_table, error_policy );

            Grid grid;
            grid.set_symbols( shader.symbols() );
            grid.resize( 2, 2 );
            grid.zero();

            VirtualMachine virtual_machine;
            virtual_machine.initialize( grid, shader );
            virtual_machine.shade( grid, shader );

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

    TEST_FIXTURE( ContinueStatementTest, continue_statement_in_for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface continue_statement_in_for_loop() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       y += x; \n"
            "       continue; \n"
            "       y += x; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_in_for_loop_with_level )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface continue_statement_in_for_loop_with_level() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       y += x; \n"
            "       continue 1; \n"
            "       y += x; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_in_for_loop_with_out_of_range_level )
    {
        const char* source =
            "surface continue_statement_in_for_loop_with_out_of_range_level() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       continue 2; \n"
            "   } \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        REQUIRE CHECK( !error_policy.errors.empty() && ! error_policy.messages.empty() );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Continue to a level outside of a loop" );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_in_while_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface continue_statement_in_while_loop() { \n"
            "   uniform float i = 0; \n"
            "   while ( i < 4 ) { \n"
            "       y += x; \n"
            "       i += 1; \n"
            "       continue; \n"
            "       y += x; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_in_while_loop_with_level )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface continue_statement_in_while_loop() { \n"
            "   uniform float i = 0; \n"
            "   while ( i < 4 ) { \n"
            "       y += x; \n"
            "       i += 1; \n"
            "       continue 1; \n"
            "       y += x; \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_in_while_loop_with_out_of_range_level )
    {
        const char* source =
            "surface continue_statement_in_while_loop_with_out_of_range_level() { \n"
            "   uniform float i = 0; \n"
            "   while ( i < 4 ) { \n"
            "       continue 2; \n"
            "       i += 1; \n"
            "   } \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        REQUIRE CHECK( !error_policy.errors.empty() && ! error_policy.messages.empty() );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Continue to a level outside of a loop" );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_out_of_first_level_of_nested_for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface continue_out_of_first_level_of_nested_for_loop() { \n"
            "   uniform float i, j; \n"
            "   for ( j = 0; j < 4; j += 1 ) { \n"
            "       for ( i = 0; i < 4; i += 1 ) { \n"
            "           y += x; \n"
            "           continue 1; \n"
            "           y += x; \n"
            "       } \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 16.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 16.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_out_of_second_level_of_nested_for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        test(
            "surface continue_out_of_second_level_of_nested_for_loop() { \n"
            "   uniform float i, j; \n"
            "   for ( j = 0; j < 4; j += 1 ) { \n"
            "       for ( i = 0; i < 4; i += 1 ) { \n"
            "           y += x; \n"
            "           continue 2; \n"
            "           y += x; \n"
            "       } \n"
            "   } \n"
            "}"
        );
        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_outside_of_loop )
    {
        const char* source =
            "surface continue_statement_outside_of_loop() { \n"
            "   continue; \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        REQUIRE CHECK( !error_policy.errors.empty() && ! error_policy.messages.empty() );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Continue outside of a loop" );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_with_level_outside_of_loop )
    {
        const char* source =
            "surface continue_statement_outside_of_loop() { \n"
            "   continue 2; \n"
            "}"
        ;
        CaptureErrorPolicy error_policy;
        test( source, error_policy );
        REQUIRE CHECK( !error_policy.errors.empty() && ! error_policy.messages.empty() );
        CHECK_EQUAL( error_policy.errors[0], RENDER_ERROR_CODE_GENERATION_ERROR );
        CHECK_EQUAL( error_policy.messages[0], "Continue outside of a loop" );
    }
}
