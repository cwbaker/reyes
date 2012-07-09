
#include <sweet/unit/UnitTest.h>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/VirtualMachine.hpp>
#include <sweet/render/ErrorPolicy.hpp>
#include <sweet/render/ErrorAction.hpp>
#include <sweet/render/Error.hpp>
#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

static const float TOLERANCE = 0.01f;

SUITE( ContinueStatements )
{
    struct ExpectLineAndMessage : public render::ErrorPolicy
    {
        int line_;
        const char* message_;
        
        ExpectLineAndMessage( int line, const char* message )
        : ErrorPolicy(),
          line_( line ),
          message_( message )
        {
            SWEET_ASSERT( line >= 0 );
            SWEET_ASSERT( message );
            actions( ERROR_ACTION_THROW );
        }
        
        void parser_error( int line, const char* message )
        {
            CHECK_EQUAL( line_, line );
            CHECK( strcmp(message, message_) == 0 );
        }
    };

    struct ContinueStatementTest
    {
        Grid grid;
        float* x;
        float* y;
     
        ContinueStatementTest()
        : grid(),
          x( NULL ),
          y( NULL )
        {
            grid.resize( 2, 2 );
            ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();
        }

        void test( const char* source )
        {
            ErrorPolicy error_policy;
            error_policy.actions( ERROR_ACTION_THROW );
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
            VirtualMachine virtual_machine;
            virtual_machine.initialize( grid, shader );
            virtual_machine.shade( grid, grid, shader );
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
        CHECK_THROW( test(source, ExpectLineAndMessage(4, "Continue to a level outside of a loop")), CodeGenerationFailedError );
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
        CHECK_THROW( test(source, ExpectLineAndMessage(4, "Continue to a level outside of a loop")), CodeGenerationFailedError );
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
        CHECK_THROW( test(source, ExpectLineAndMessage(2, "Continue outside of a loop")), CodeGenerationFailedError );
    }

    TEST_FIXTURE( ContinueStatementTest, continue_statement_with_level_outside_of_loop )
    {
        const char* source =
            "surface continue_statement_outside_of_loop() { \n"
            "   continue 2; \n"
            "}"
        ;
        CHECK_THROW( test(source, ExpectLineAndMessage(2, "Continue outside of a loop")), CodeGenerationFailedError );
    }
}
