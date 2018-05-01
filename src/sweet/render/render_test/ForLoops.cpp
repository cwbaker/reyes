
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
using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

static const float TOLERANCE = 0.01f;

SUITE( ForLoops )
{
    struct ForLoopTest
    {
        Grid grid;
        float* x;
        float* y;
     
        ForLoopTest()
        : grid(),
          x( NULL ),
          y( NULL )
        {
            grid.resize( 2, 2 );
            shared_ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            shared_ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();
        }
        
        void test( const char* source )
        {
            ErrorPolicy error_policy;
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

    TEST_FIXTURE( ForLoopTest, for_loop )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test( 
            "surface for_loop() { \n"
            "   uniform float i; \n"
            "   for ( i = 0; i < 4; i += 1 ) { \n"
            "       y += x; \n"
            "   } \n"
            "}"
        );

        CHECK_CLOSE( 4.0f, y[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[1], TOLERANCE );
        CHECK_CLOSE( 4.0f, y[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, y[3], TOLERANCE );
    }
}
