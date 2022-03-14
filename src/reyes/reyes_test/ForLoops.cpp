
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Scope.hpp>
#include <reyes/SymbolTable.hpp>
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

static const float TOLERANCE = 0.01f;

SUITE( ForLoops )
{
    struct ForLoopTest
    {
        float x [4];
        float y [4];
     
        ForLoopTest()
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
            grid.set_shader( &shader );
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
