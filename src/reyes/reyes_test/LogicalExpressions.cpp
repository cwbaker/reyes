
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Scope.hpp>
#include <reyes/Scope.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

SUITE( LogicalExpressions )
{
    struct LogicalExpressionTest
    {
        float x [4];
        float y [4];
        int a [4];
        int b [4];
        int c [4];
     
        LogicalExpressionTest()
        : x{}
        , y{}
        , a{}
        , b{}
        , c{}
        {
        }
        
        void test( const char* source )
        {
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
                ( "a", TYPE_INTEGER )
                ( "b", TYPE_INTEGER )
                ( "c", TYPE_INTEGER )
            ;

            ErrorPolicy error_policy;
            Shader shader( source, source + strlen(source), symbol_table, error_policy );

            Grid grid;
            grid.set_symbols( shader.symbols() );
            grid.resize( 2, 2 );
            grid.zero();

            float* xx = grid.float_value( "x" );
            float* yy = grid.float_value( "y" );
            int* aa = grid.int_value( "a" );
            int* bb = grid.int_value( "b" );
            int* cc = grid.int_value( "c" );
            if ( xx && yy && aa && bb && cc )
            {
                memcpy( xx, x, sizeof(float) * grid.size() );
                memcpy( yy, y, sizeof(float) * grid.size() );
                memcpy( aa, a, sizeof(int) * grid.size() );
                memcpy( bb, b, sizeof(int) * grid.size() );
                memcpy( cc, c, sizeof(int) * grid.size() );
                VirtualMachine virtual_machine;
                virtual_machine.initialize( grid, shader );
                virtual_machine.shade( grid, shader );
                memcpy( x, xx, sizeof(float) * grid.size() );
                memcpy( y, yy, sizeof(float) * grid.size() );
                memcpy( a, aa, sizeof(int) * grid.size() );
                memcpy( b, bb, sizeof(int) * grid.size() );
                memcpy( c, cc, sizeof(int) * grid.size() );
            }
        }
    };

    TEST_FIXTURE( LogicalExpressionTest, greater )
    {
        const char* source =
            "surface greater() { \n"
            "   a = x > 0; \n"
            "}"
        ;
        
        x[0] = -1.0f;
        x[1] = 0.0f;
        x[2] = 0.0f;
        x[3] = 1.0f;
        test( source );
        CHECK( a[0] == 0 );
        CHECK( a[1] == 0 );
        CHECK( a[2] == 0 );
        CHECK( a[3] == 1 );
    }

    TEST_FIXTURE( LogicalExpressionTest, greater_equal )
    {
        const char* source =
            "surface greater_equal() { \n"
            "   a = x >= 0; \n"
            "}"
        ;
        
        x[0] = -1.0f;
        x[1] = 0.0f;
        x[2] = 0.0f;
        x[3] = 1.0f;
        test( source );
        CHECK( a[0] == 0 );
        CHECK( a[1] == 1 );
        CHECK( a[2] == 1 );
        CHECK( a[3] == 1 );
    }

    TEST_FIXTURE( LogicalExpressionTest, less )
    {
        const char* source =
            "surface less() { \n"
            "   a = x < 0; \n"
            "}"
        ;
        
        x[0] = -1.0f;
        x[1] = 0.0f;
        x[2] = 0.0f;
        x[3] = 1.0f;
        test( source );
        CHECK( a[0] == 1 );
        CHECK( a[1] == 0 );
        CHECK( a[2] == 0 );
        CHECK( a[3] == 0 );
    }


    TEST_FIXTURE( LogicalExpressionTest, less_equal )
    {
        const char* source =
            "surface less_equal() { \n"
            "   a = x <= 0; \n"
            "}"
        ;
        
        x[0] = -1.0f;
        x[1] = 0.0f;
        x[2] = 0.0f;
        x[3] = 1.0f;
        test( source );
        CHECK( a[0] == 1 );
        CHECK( a[1] == 1 );
        CHECK( a[2] == 1 );
        CHECK( a[3] == 0 );
    }
    
    TEST_FIXTURE( LogicalExpressionTest, equal )
    {
        const char* source =
            "surface equal() { \n"
            "   a = x == 0; \n"
            "}"
        ;
        
        x[0] = -1.0f;
        x[1] = 0.0f;
        x[2] = 0.0f;
        x[3] = 1.0f;
        test( source );
        CHECK( a[0] == 0 );
        CHECK( a[1] == 1 );
        CHECK( a[2] == 1 );
        CHECK( a[3] == 0 );
    }
    
    TEST_FIXTURE( LogicalExpressionTest, not_equal )
    {
        const char* source =
            "surface not_equal() { \n"
            "   a = x != 0; \n"
            "}"
        ;
        
        x[0] = -1.0f;
        x[1] = 0.0f;
        x[2] = 0.0f;
        x[3] = 1.0f;
        test( source );
        CHECK( a[0] == 1 );
        CHECK( a[1] == 0 );
        CHECK( a[2] == 0 );
        CHECK( a[3] == 1 );
    }
    
    TEST_FIXTURE( LogicalExpressionTest, logical_and )
    {
        const char* source =
            "surface logical_and() { \n"
            "   c = a && b; \n"
            "}"
        ;
        
        a[0] = 0;
        a[1] = 1;
        a[2] = 0;
        a[3] = 1;
        
        b[0] = 1;
        b[1] = 0;
        b[2] = 0;
        b[3] = 1;
        
        test( source );
        CHECK( c[0] == 0 );
        CHECK( c[1] == 0 );
        CHECK( c[2] == 0 );
        CHECK( c[3] == 1 );
    }
    
    TEST_FIXTURE( LogicalExpressionTest, logical_or )
    {
        const char* source =
            "surface logical_or() { \n"
            "   c = a || b; \n"
            "}"
        ;
        
        a[0] = 0;
        a[1] = 1;
        a[2] = 0;
        a[3] = 1;
        
        b[0] = 1;
        b[1] = 0;
        b[2] = 0;
        b[3] = 1;
        
        test( source );
        CHECK( c[0] == 1 );
        CHECK( c[1] == 1 );
        CHECK( c[2] == 0 );
        CHECK( c[3] == 1 );
    }
}
