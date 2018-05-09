
#include <UnitTest++/UnitTest++.h>
#include <reyes/Renderer.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/ErrorCode.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::reyes;

static const float TOLERANCE = 0.01f;

SUITE( ColorFunctions )
{
    struct ColorFunctionTest
    {
        Renderer renderer;
        Grid grid;
        vec3* C;
        vec3* Ci;
        float* x;
     
        ColorFunctionTest()
        : grid(),
          C( NULL ),
          Ci( NULL ),
          x( NULL )
        {
            renderer.begin();
            renderer.perspective( float(M_PI) / 2.0f );
            renderer.projection();
            renderer.begin_world();

            grid.resize( 2, 2 );
            
            shared_ptr<Value> P_value = grid.add_value( "P", TYPE_POINT );
            P_value->zero();
            
            shared_ptr<Value> C_value = grid.add_value( "C", TYPE_COLOR );
            C_value->zero();
            C = C_value->vec3_values();
            
            shared_ptr<Value> Ci_value = grid.add_value( "Ci", TYPE_COLOR );
            Ci_value->zero();
            Ci = Ci_value->vec3_values();
            
            shared_ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
        }
        
        void test( const char* source )
        {
            renderer.symbol_table().add_symbols()
                ( "C", TYPE_COLOR )
                ( "x", TYPE_FLOAT )
            ;
            Shader shader( source, source + strlen(source), renderer.symbol_table(), renderer.error_policy() );
            renderer.surface_shader( &shader );
            renderer.surface_shade( grid );
        }
    };

    TEST_FIXTURE( ColorFunctionTest, mix )
    {
        C[0] = vec3( 0.0f, 0.0f, 1.0f );
        C[1] = vec3( 0.0f, 1.0f, 0.0f );
        C[2] = vec3( 0.0f, 1.0f, 1.0f );
        C[3] = vec3( 1.0f, 0.0f, 0.0f );
        test(
            "surface mix_test() { \n"
            "   Ci = mix( C, Ci, 0.5 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 0.5f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 0.5f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 0.5f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 0.5f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 0.5f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].z, TOLERANCE );
    }

    TEST_FIXTURE( ColorFunctionTest, comp )
    {
        C[0] = vec3( 0.0f, 0.0f, 1.0f );
        C[1] = vec3( 0.0f, 1.0f, 0.0f );
        C[2] = vec3( 0.0f, 1.0f, 1.0f );
        C[3] = vec3( 1.0f, 0.0f, 0.0f );

        test(
            "surface comp_test() { \n"
            "   x = comp( C, 0 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 0.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[3], TOLERANCE );

        test(
            "surface comp_test() { \n"
            "   x = comp( C, 1 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, x[3], TOLERANCE );

        test(
            "surface comp_test() { \n"
            "   x = comp( C, 2 ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 0.0f, x[3], TOLERANCE );
    }

    TEST_FIXTURE( ColorFunctionTest, comp_uniform )
    {
        test(
            "surface comp_test() { \n"    
            "   uniform color c = color ( 1, 0, 0 ); \n"        
            "   x = comp( c, 0 ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[3], TOLERANCE );

        test(
            "surface comp_test() { \n"
            "   uniform color c = color ( 0, 1, 0 ); \n"
            "   x = comp( c, 1 ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[3], TOLERANCE );

        test(
            "surface comp_test() { \n"
            "   uniform color c = color ( 0, 0, 1 ); \n"        
            "   x = comp( c, 2 ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[2], TOLERANCE );
        CHECK_CLOSE( 1.0f, x[3], TOLERANCE );
    }

    TEST_FIXTURE( ColorFunctionTest, setcomp )
    {
        x[0] = 1.0f;
        x[1] = 1.1f;
        x[2] = 1.2f;
        x[3] = 1.3f;
    
        test(
            "surface setcomp_test() { \n"
            "   setcomp( Ci, 0, x ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 1.1f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 1.2f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 1.3f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].z, TOLERANCE );

        test(
            "surface setcomp_test() { \n"
            "   setcomp( Ci, 1, x ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 1.1f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 1.2f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 1.3f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].z, TOLERANCE );

        test(
            "surface setcomp_test() { \n"
            "   setcomp( Ci, 2, x ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 1.1f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 1.2f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 1.3f, Ci[3].z, TOLERANCE );
    }

    TEST_FIXTURE( ColorFunctionTest, setcomp_uniform )
    {
        test(
            "surface setcomp_test() { \n"
            "   setcomp( Ci, 0, 1 ); \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].z, TOLERANCE );

        test(
            "surface setcomp_test() { \n"
            "   setcomp( Ci, 1, 1 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].z, TOLERANCE );

        test(
            "surface setcomp_test() { \n"
            "   setcomp( Ci, 2, 1 ); \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, Ci[0].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[0].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[0].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[1].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[1].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[2].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[2].z, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].x, TOLERANCE );
        CHECK_CLOSE( 0.0f, Ci[3].y, TOLERANCE );
        CHECK_CLOSE( 1.0f, Ci[3].z, TOLERANCE );
    }
    
    TEST_FIXTURE( ColorFunctionTest, hsv_to_rgb_typecast )
    {
        const int HUES = 4;
        const int SATURATIONS = 4;
        const int VALUES = 4;
        
        for ( int h = 0; h < HUES; ++h )
        {
            for ( int s = 0; s < SATURATIONS; ++s )
            {
                for ( int v = 0; v < VALUES; ++v )
                {
                    vec3 hsv( float(h) / HUES * 360.0f, float(s) / SATURATIONS, float(v) / VALUES );
                    vec3 rgb = rgb_from_hsv( hsv );
                    char shader [256];
                    sprintf( shader, "surface hsv_to_rgb_typecast_test() { Ci = color \"hsv\" (%.02f, %.02f, %.02f); }", hsv.x, hsv.y, hsv.z );
                    test( shader );
                    CHECK_CLOSE( rgb.x, Ci[0].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[0].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[0].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[1].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[1].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[1].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[2].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[2].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[2].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[3].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[3].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[3].z, TOLERANCE );
                }
            }
        }    
    }
    
    TEST_FIXTURE( ColorFunctionTest, hsl_to_rgb_typecast )
    {
        const int HUES = 4;
        const int SATURATIONS = 4;
        const int LIGHTNESSES = 4;
        
        for ( int h = 0; h < HUES; ++h )
        {
            for ( int s = 0; s < SATURATIONS; ++s )
            {
                for ( int l = 0; l < LIGHTNESSES; ++l )
                {
                    vec3 hsl( float(h) / HUES * 360.0f, float(s) / SATURATIONS, float(l) / LIGHTNESSES );
                    vec3 rgb = rgb_from_hsl( hsl );
                    char shader [256];
                    sprintf( shader, "surface hsv_to_rgb_typecast_test() { Ci = color \"hsl\" (%.02f, %.02f, %.02f); }", hsl.x, hsl.y, hsl.z );
                    test( shader );
                    CHECK_CLOSE( rgb.x, Ci[0].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[0].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[0].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[1].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[1].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[1].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[2].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[2].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[2].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[3].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[3].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[3].z, TOLERANCE );
                }
            }
        }    
    }

    TEST_FIXTURE( ColorFunctionTest, hsv_to_rgb_transform )
    {
        const int HUES = 2;
        const int SATURATIONS = 2;
        const int VALUES = 2;
        
        for ( int h = 0; h < HUES; ++h )
        {
            for ( int s = 0; s < SATURATIONS; ++s )
            {
                for ( int v = 0; v < VALUES; ++v )
                {
                    vec3 hsv( float(h) / HUES * 360.0f, float(s) / SATURATIONS, float(v) / VALUES );
                    vec3 rgb = rgb_from_hsv( hsv );
                    char shader [256];
                    sprintf( shader, "surface hsv_to_rgb_transform_test() { Ci = ctransform( \"hsv\", color(%.02f, %.02f, %.02f) ); }", hsv.x, hsv.y, hsv.z );
                    test( shader );
                    CHECK_CLOSE( rgb.x, Ci[0].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[0].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[0].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[1].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[1].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[1].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[2].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[2].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[2].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[3].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[3].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[3].z, TOLERANCE );
                }
            }
        }    
    }
    
    TEST_FIXTURE( ColorFunctionTest, hsl_to_rgb_transform )
    {
        const int HUES = 2;
        const int SATURATIONS = 2;
        const int LIGHTNESSES = 2;
        
        for ( int h = 0; h < HUES; ++h )
        {
            for ( int s = 0; s < SATURATIONS; ++s )
            {
                for ( int l = 0; l < LIGHTNESSES; ++l )
                {
                    vec3 hsl( float(h) / HUES * 360.0f, float(s) / SATURATIONS, float(l) / LIGHTNESSES );
                    vec3 rgb = rgb_from_hsl( hsl );
                    char shader [256];
                    sprintf( shader, "surface hsl_to_rgb_transform_test() { Ci = ctransform( \"hsl\", color(%.02f, %.02f, %.02f) ); }", hsl.x, hsl.y, hsl.z );
                    test( shader );
                    CHECK_CLOSE( rgb.x, Ci[0].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[0].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[0].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[1].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[1].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[1].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[2].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[2].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[2].z, TOLERANCE );
                    CHECK_CLOSE( rgb.x, Ci[3].x, TOLERANCE );
                    CHECK_CLOSE( rgb.y, Ci[3].y, TOLERANCE );
                    CHECK_CLOSE( rgb.z, Ci[3].z, TOLERANCE );
                }
            }
        }    
    }
}
