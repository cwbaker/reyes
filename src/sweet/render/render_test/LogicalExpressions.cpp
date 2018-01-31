
#include <UnitTest++/UnitTest++.h>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/ErrorPolicy.hpp>
#include <sweet/render/VirtualMachine.hpp>
#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

SUITE( LogicalExpressions )
{
    struct LogicalExpressionTest
    {
        Grid grid;
        float* x;
        float* y;
        int* a;
        int* b;
        int* c;
     
        LogicalExpressionTest()
        : grid(),
          x( NULL ),
          y( NULL ),
          a( NULL ),
          b( NULL ),
          c( NULL )
        {
            grid.resize( 2, 2 );
            ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();
            
            ptr<Value> a_value = grid.add_value( "a", TYPE_INTEGER );
            a_value->zero();
            a = a_value->int_values();
      
            ptr<Value> b_value = grid.add_value( "b", TYPE_INTEGER );
            b_value->zero();
            b = b_value->int_values();
            
            ptr<Value> c_value = grid.add_value( "c", TYPE_INTEGER );
            c_value->zero();
            c = c_value->int_values();
        }
        
        void test( const char* source )
        {
            ErrorPolicy error_policy;
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
                ( "a", TYPE_INTEGER )
                ( "b", TYPE_INTEGER )
                ( "c", TYPE_INTEGER )
            ;
            
            Shader shader( source, source + strlen(source), symbol_table, error_policy );
            VirtualMachine virtual_machine;
            virtual_machine.initialize( grid, shader );
            virtual_machine.shade( grid, grid, shader );
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
