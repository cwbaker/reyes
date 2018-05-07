
#include <UnitTest++/UnitTest++.h>
#include <sweet/render/Renderer.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <math/vec3.ipp>
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

static const char* LIGHT_SHADER_SOURCE = 
    "light pointlight(color lightcolor = 1; point from = point \"shader\" (0, 0, 0); ) { \n"
    "   illuminate(from) { \n"
    "       Cl = lightcolor; \n"
    "       Ol = color (1, 1, 1); \n"
    "   } \n"
    "} \n"
;

SUITE( IlluminanceStatements )
{
    struct IlluminanceStatementTest
    {
        Renderer renderer;
        Shader light_shader;
        Grid grid;
        vec3* P;
        vec3* N;
        vec3* Ci;
     
        IlluminanceStatementTest()
        : renderer(),
          light_shader( LIGHT_SHADER_SOURCE, LIGHT_SHADER_SOURCE + strlen(LIGHT_SHADER_SOURCE), renderer.symbol_table(), renderer.error_policy() ),
          grid(),
          P( NULL ),
          N( NULL ),
          Ci( NULL )
        {       
            renderer.begin();
            renderer.perspective( float(M_PI) / 2.0f );
            renderer.projection();
            renderer.begin_world();

            Grid& light = renderer.light_shader( &light_shader );
            light["from"] = vec3( 0.0f, 1.0f, 0.0f );
            light["lightcolor"] = vec3( 0.0f, 1.0f, 0.0f );
                       
            grid.resize( 2, 4 );

            shared_ptr<Value> P_value = grid.add_value( "P", TYPE_POINT );
            P_value->zero();
            P = P_value->vec3_values();

            shared_ptr<Value> N_value = grid.add_value( "N", TYPE_NORMAL );
            N_value->zero();
            N = N_value->vec3_values();

            shared_ptr<Value> Ci_value = grid.add_value( "Ci", TYPE_COLOR );
            Ci_value->zero();
            Ci = Ci_value->vec3_values();
        }
        
        void test( const char* source )
        {
            Shader shader( source, source + strlen(source), renderer.symbol_table(), renderer.error_policy() );
            renderer.surface_shader( &shader );
            renderer.surface_shade( grid );
        }
    };

    TEST_FIXTURE( IlluminanceStatementTest, illuminance_axis_angle_statement )
    {
        N[0] = vec3( 0.0f, 0.0f, 0.0f );
        N[1] = normalize( vec3(0.0f, 0.0f, 1.0f) );
        N[2] = normalize( vec3(0.0f, 1.0f, 0.0f) );
        N[3] = normalize( vec3(0.0f, 1.0f, 1.0f) );
        N[4] = normalize( vec3(1.0f, 0.0f, 0.0f) );
        N[5] = normalize( vec3(1.0f, 0.0f, 1.0f) );
        N[6] = normalize( vec3(1.0f, 1.0f, 0.0f) );
        N[7] = normalize( vec3(1.0f, 1.0f, 1.0f) );

        test(
            "surface illuminance_axis_angle_statement() { \n"
            "   illuminance( P, N, 3.14 / 2 ) { \n"
            "       Ci += Ol * Cl; \n"
            "   } \n"
            "}"
        );
        
        CHECK_CLOSE( 0.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[4].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[5].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[6].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[7].y, TOLERANCE );
    }
}
