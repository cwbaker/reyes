
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Scope.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

static const float TOLERANCE = 0.01f;

SUITE( IfStatements )
{
    struct IfStatementTest
    {
        float x [4];
        float y [4];
     
        IfStatementTest()
        : x{}
        , y{}
        {
        }
        
        void test( const char* source )
        {
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
            ;

            ErrorPolicy error_policy;
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

    TEST_FIXTURE( IfStatementTest, if_statement )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        
        test(
            "surface if_statement() { \n"
            "   if ( x > 0 ) { \n"
            "       y = 1 - 2 * x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( -1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }
    
    TEST_FIXTURE( IfStatementTest, if_statement_with_add_assign )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        
        test(
            "surface if_statement_with_add_assign() { \n"
            "   if ( x > 0 ) { \n"
            "       y += 1 - 2 * x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( -1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( IfStatementTest, if_statement_with_uniform_condition )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        
        test(
            "surface if_statement_with_uniform_condition() { \n"
            "   uniform float i = 1; \n"
            "   if ( i > 0 ) { \n"
            "       y = x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( 1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( IfStatementTest, if_else_statement )
    {
        x[0] = 1.0f;
        x[1] = -1.0f;
        x[2] = 1.0f;
        x[3] = -1.0f;      
          
        test(
            "surface if_else_statement() { \n"
            "   if ( x > 0 ) { \n"
            "       y = 1 - 2 * x; \n"
            "   } \n"
            "   else { \n"
            "       y = 1 + 2 * x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( -1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[1], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( IfStatementTest, if_else_statement_with_add_assign )
    {
        x[0] = 1.0f;
        x[1] = -1.0f;
        x[2] = 1.0f;
        x[3] = -1.0f;    
            
        test(
            "surface if_else_statement_with_add_assign() { \n"
            "   if ( x > 0 ) { \n"
            "       y += 1 - 2 * x; \n"
            "   } \n"
            "   else { \n"
            "       y += 1 + 2 * x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( -1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[1], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( IfStatementTest, nested_if_else_statement_with_no_processing )
    {
        x[0] = 1.0f;
        x[1] = -1.0f;
        x[2] = 1.0f;
        x[3] = -1.0f;
            
        test(
            "surface nested_if_else_statement() { \n"
            "   if ( x > 0 ) { \n"
            "       if ( x < 0 ) { \n"
            "           y += 1 - 2 * x; \n"
            "       } \n"
            "       y += 1 - 2 * x; \n"
            "   } \n"
            "   else { \n"
            "       if ( x > 0 ) { \n"
            "           y += 1 - 2 * x; \n"
            "       } \n"
            "       y += 1 + 2 * x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( -1.0f, y[0], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[1], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[2], TOLERANCE );
        CHECK_CLOSE( -1.0f, y[3], TOLERANCE );
    }

    TEST_FIXTURE( IfStatementTest, nested_if_else_statement_with_processing )
    {
        x[0] = 1.0f;
        x[1] = -1.0f;
        x[2] = 1.0f;
        x[3] = -1.0f;
            
        test(
            "surface nested_if_else_statement() { \n"
            "   if ( x > 0 ) { \n"
            "       if ( x > 0 ) { \n"
            "           y += 1 - 2 * x; \n"
            "       } \n"
            "       y += 1 - 2 * x; \n"
            "   } \n"
            "   else { \n"
            "       if ( x < 0 ) { \n"
            "           y += 1 + 2 * x; \n"
            "       } \n"
            "       y += 1 + 2 * x; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( -2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( -2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( -2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( -2.0f, y[3], TOLERANCE );
    }
}
