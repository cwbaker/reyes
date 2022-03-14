
#include <UnitTest++/UnitTest++.h>
#include <reyes/Renderer.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Options.hpp>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <math/vec3.ipp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::shared_ptr;
using namespace math;
using namespace reyes;

static const float TOLERANCE = 0.01f;

SUITE( GeometricFunctions )
{
    struct GeometricFunctionTest
    {
        Renderer renderer;
        float x [4];
        float y [4];
        vec3 z [4];
        vec3 P [4];
     
        GeometricFunctionTest()
        : x{}
        , y{}
        , z{}
        , P{}
        {
            renderer.begin();
            renderer.perspective( float(M_PI) / 2.0f );
            renderer.projection();
            renderer.begin_world();
        }
        
        void test( const char* source )
        {
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
                ( "z", TYPE_VECTOR )
            ;            

            Shader shader;
            shader.load_memory( source, source + strlen(source), symbol_table, renderer.error_policy() );

            Grid& grid = renderer.surface_shader( &shader );
            grid.resize( 2, 2 );

            float* xx = grid.float_value( "x" );
            float* yy = grid.float_value( "y" );
            vec3* zz = grid.vec3_value( "z" );
            vec3* positions = grid.vec3_value( "P" );
            if ( xx && yy && zz && positions )
            {
                memcpy( xx, x, sizeof(float) * grid.size() );
                memcpy( yy, y, sizeof(float) * grid.size() );
                memcpy( zz, z, sizeof(vec3) * grid.size() );
                memcpy( positions, P, sizeof(vec3) * grid.size() );
                renderer.surface_shade( grid );
                memcpy( x, xx, sizeof(float) * grid.size() );
                memcpy( y, yy, sizeof(float) * grid.size() );
                memcpy( z, zz, sizeof(vec3) * grid.size() );
                memcpy( P, positions, sizeof(vec3) * grid.size() );
            }
        }
    };

    TEST_FIXTURE( GeometricFunctionTest, area )
    {
        P[0] = vec3( 0.0f, 0.0f, 0.0f );
        P[1] = vec3( 1.0f, 0.0f, 0.0f );
        P[2] = vec3( 0.0f, 1.0f, 0.0f );
        P[3] = vec3( 1.0f, 1.0f, 0.0f );
        test(
            "surface area_test() { \n"
            "   x = area( P ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[3], TOLERANCE );
    }

    TEST_FIXTURE( GeometricFunctionTest, refract )
    {
        P[0] = vec3( 0.0f, 0.0f, 0.0f );
        P[1] = vec3( 1.0f, 0.0f, 0.0f );
        P[2] = vec3( 0.0f, 1.0f, 0.0f );
        P[3] = vec3( 1.0f, 1.0f, 0.0f );
        test(
            "surface refract_test() { \n"
            "   z = refract( I, N, 0.5 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, z[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[0].y, TOLERANCE );
        CHECK_CLOSE( -0.866025388f, z[0].z, TOLERANCE );
        CHECK_CLOSE( 0.5f, z[1].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[1].y, TOLERANCE );
        CHECK_CLOSE( -0.866025388f, z[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[2].x, TOLERANCE );
        CHECK_CLOSE( 0.5f, z[2].y, TOLERANCE );
        CHECK_CLOSE( -0.866025388f, z[2].z, TOLERANCE );
        CHECK_CLOSE( 0.5f, z[3].x, TOLERANCE );
        CHECK_CLOSE( 0.5f, z[3].y, TOLERANCE );
        CHECK_CLOSE( -0.866025388f, z[3].z, TOLERANCE );
    }

    TEST_FIXTURE( GeometricFunctionTest, fresnel )
    {
        P[0] = vec3( 0.0f, 0.0f, 0.0f );
        P[1] = vec3( 1.0f, 0.0f, 0.0f );
        P[2] = vec3( 0.0f, 1.0f, 0.0f );
        P[3] = vec3( 1.0f, 1.0f, 0.0f );
        test(
            "surface fresnel_test() { \n"
            "   float Kr, Kt; \n"
            "   fresnel( I, N, 1.5, Kr, Kt ); \n"
            "}"
        );
    }
    
    TEST_FIXTURE( GeometricFunctionTest, depth )
    {
        const float near_clip_distance = renderer.options().near_clip_distance();
        const float far_clip_distance = renderer.options().far_clip_distance();        
        P[0] = vec3( 0.0f, 0.0f, near_clip_distance );
        P[1] = vec3( 0.0f, 0.0f, far_clip_distance );
        P[2] = vec3( 0.0f, 0.0f, near_clip_distance + 0.5f * (far_clip_distance - near_clip_distance) );
        P[3] = vec3( 0.0f, 0.0f, near_clip_distance + 1.5f * (far_clip_distance - near_clip_distance)  );
        test(
            "surface depth_test() { \n"
            "   x = depth( P ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[1], TOLERANCE );
        CHECK( x[2] < 1.0f && x[2] < x[1] );
        CHECK( x[3] > 1.0f && x[3] > x[1] );
    }
}
