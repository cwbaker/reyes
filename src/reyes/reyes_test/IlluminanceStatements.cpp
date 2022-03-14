
#include <UnitTest++/UnitTest++.h>
#include <reyes/Renderer.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Scope.hpp>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <math/vec3.ipp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

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
        vec3 P [8];
        vec3 N [8];
        vec3 Ci [8];
     
        IlluminanceStatementTest()
        : renderer()
        , light_shader()
        , P{}
        , N{}
        , Ci{}
        {
            light_shader.load_memory( LIGHT_SHADER_SOURCE, LIGHT_SHADER_SOURCE + strlen(LIGHT_SHADER_SOURCE), renderer.error_policy() );

            renderer.begin();
            renderer.perspective( float(M_PI) / 2.0f );
            renderer.projection();
            renderer.begin_world();

            Grid& light = renderer.light_shader( &light_shader );
            light["from"] = vec3( 0.0f, 1.0f, 0.0f );
            light["lightcolor"] = vec3( 0.0f, 1.0f, 0.0f );
        }
        
        void test( const char* source )
        {
            Shader shader;
            shader.load_memory( source, source + strlen(source), renderer.error_policy() );

            Grid& grid = renderer.surface_shader( &shader );
            grid.resize( 2, 4 );
            grid.set_normals_generated( true );

            vec3* positions = grid.vec3_value( "P" );
            vec3* normals = grid.vec3_value( "N" );
            vec3* colors = grid.vec3_value( "Ci" );
            if ( positions && normals && colors )
            {
                const bool geometry_left_handed = true;
                grid.generate_normals( geometry_left_handed );
                memcpy( positions, P, sizeof(vec3) * grid.size() );
                memcpy( normals, N, sizeof(vec3) * grid.size() );
                memcpy( colors, Ci, sizeof(vec3) * grid.size() );
                renderer.surface_shade( grid );
                memcpy( P, positions, sizeof(vec3) * grid.size() );
                memcpy( N, normals, sizeof(vec3) * grid.size() );
                memcpy( Ci, colors, sizeof(vec3) * grid.size() );
            }
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
