
#include <sweet/unit/UnitTest.h>
#include <sweet/render/Renderer.hpp>
#include <sweet/render/VirtualMachine.hpp>
#include <sweet/render/ErrorPolicy.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/Options.hpp>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/Error.hpp>
#include <sweet/math/vec3.ipp>
#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

static const float TOLERANCE = 0.01f;

SUITE( GeometricFunctions )
{
    struct GeometricFunctionTest
    {
        Renderer renderer;
        Grid grid;
        float* x;
        float* y;
        vec3* z;
        vec3* P;
     
        GeometricFunctionTest()
        : grid(),
          x( NULL ),
          y( NULL ),
          z( NULL ),
          P( NULL )
        {
            renderer.begin();
            renderer.perspective( float(M_PI) / 2.0f );
            renderer.projection();
            renderer.begin_world();

            grid.resize( 2, 2 );
            ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();
            
            ptr<Value> z_value = grid.add_value( "z", TYPE_VECTOR );
            z_value->zero();
            z = z_value->vec3_values();

            ptr<Value> P_value = grid.add_value( "P", TYPE_POINT );
            P_value->zero();
            P = P_value->vec3_values();
        }
        
        void test( const char* source )
        {
            ErrorPolicy error_policy;
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
                ( "z", TYPE_VECTOR )
            ;            
            Shader shader( source, source + strlen(source), symbol_table, error_policy );
            renderer.surface_shader( &shader );
            renderer.surface_shade( grid );
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
            "   z = refract( I, N, 1 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, z[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[0].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[0].z, TOLERANCE );
        CHECK_CLOSE( 1.0f, z[1].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[1].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[2].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, z[2].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[2].z, TOLERANCE );
        CHECK_CLOSE( 1.0f, z[3].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, z[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, z[3].z, TOLERANCE );
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
