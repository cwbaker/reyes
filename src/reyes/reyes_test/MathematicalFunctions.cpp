
#include <UnitTest++/UnitTest++.h>
#include <reyes/Renderer.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <string.h>

using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::reyes;

static const float TOLERANCE = 0.01f;

SUITE( MathematicalFunctions )
{
    struct MathematicalFunctionTest
    {
        Renderer renderer;
        Grid grid;
        float* x;
        float* y;
        vec3* P;
        vec3* N;
        vec3* Ci;
     
        MathematicalFunctionTest()
        : grid(),
          x( NULL ),
          y( NULL ),
          P( NULL ),
          N( NULL ),
          Ci( NULL )
        {
            renderer.begin();
            renderer.perspective( float(float(M_PI)) / 2.0f );
            renderer.projection();
            renderer.begin_world();

            grid.resize( 2, 2 );
            shared_ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            shared_ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();

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
            renderer.symbol_table().add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
            ;
            Shader shader( source, source + strlen(source), renderer.symbol_table(), renderer.error_policy() );
            renderer.surface_shader( &shader );
            renderer.surface_shade( grid );
        }
        
        float radians( float degrees ) const
        {
            return degrees * float(M_PI) / 180.0f;
        }
        
        float degrees( float radians ) const
        {
            return radians * 180.0f / float(M_PI);
        }
    };

    TEST_FIXTURE( MathematicalFunctionTest, radians_constant )
    {
        test(
            "surface radians_uniform() { \n"
            "   y = radians( 3.14159 ); \n"
            "}"
        );
        CHECK_CLOSE( radians(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( radians(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( radians(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( radians(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, radians_uniform )
    {
        test(
            "surface radians_uniform(uniform float pi = 3.14159;) { \n"
            "   y = radians( pi ); \n"
            "}"
        );
        CHECK_CLOSE( radians(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( radians(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( radians(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( radians(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, radians_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface radians_varying() { \n"
            "   y = radians( x ); \n"
            "}"
        );
        CHECK_CLOSE( radians(x[0]), y[0], TOLERANCE );
        CHECK_CLOSE( radians(x[1]), y[1], TOLERANCE );
        CHECK_CLOSE( radians(x[2]), y[2], TOLERANCE );
        CHECK_CLOSE( radians(x[3]), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, degrees_constant )
    {
        test(
            "surface degrees_uniform() { \n"
            "   y = degrees( 3.14159 ); \n"
            "}"
        );
        CHECK_CLOSE( degrees(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( degrees(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( degrees(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( degrees(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, degrees_uniform )
    {
        test(
            "surface degrees_uniform(uniform float pi = 3.14159;) { \n"
            "   y = degrees( pi ); \n"
            "}"
        );
        CHECK_CLOSE( degrees(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( degrees(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( degrees(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( degrees(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, degrees_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface degrees_varying() { \n"
            "   y = degrees( x ); \n"
            "}"
        );
        CHECK_CLOSE( degrees(x[0]), y[0], TOLERANCE );
        CHECK_CLOSE( degrees(x[1]), y[1], TOLERANCE );
        CHECK_CLOSE( degrees(x[2]), y[2], TOLERANCE );
        CHECK_CLOSE( degrees(x[3]), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, sin_constant )
    {
        test(
            "surface sin_uniform() { \n"
            "   y = sin( 3.14159 ); \n"
            "}"
        );
        CHECK_CLOSE( sinf(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( sinf(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( sinf(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( sinf(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, sin_uniform )
    {
        test(
            "surface sin_uniform(uniform float pi = 3.14159;) { \n"
            "   y = sin( pi ); \n"
            "}"
        );
        CHECK_CLOSE( sinf(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( sinf(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( sinf(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( sinf(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, sin_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface sin_varying() { \n"
            "   y = sin( x ); \n"
            "}"
        );
        CHECK_CLOSE( sinf(x[0]), y[0], TOLERANCE );
        CHECK_CLOSE( sinf(x[1]), y[1], TOLERANCE );
        CHECK_CLOSE( sinf(x[2]), y[2], TOLERANCE );
        CHECK_CLOSE( sinf(x[3]), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, asin_constant )
    {
        test(
            "surface asin_constant() { \n"
            "   y = asin( sin(3.14159 / 2) ); \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[3], TOLERANCE );

        test(
            "surface asin_constant() { \n"
            "   y = asin( 0 ); \n"
            "}"
        );
        CHECK_CLOSE( asinf(0.0f), y[0], TOLERANCE );
        CHECK_CLOSE( asinf(0.0f), y[1], TOLERANCE );
        CHECK_CLOSE( asinf(0.0f), y[2], TOLERANCE );
        CHECK_CLOSE( asinf(0.0f), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, asin_uniform )
    {
        test(
            "surface asin_uniform(uniform float pi = 3.14159;) { \n"
            "   y = asin( sin(pi / 2) ); \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[3], TOLERANCE );

        test(
            "surface asin_uniform(uniform float zero = 0;) { \n"
            "   y = asin( zero ); \n"
            "}"
        );
        CHECK_CLOSE( asinf(0.0f), y[0], TOLERANCE );
        CHECK_CLOSE( asinf(0.0f), y[1], TOLERANCE );
        CHECK_CLOSE( asinf(0.0f), y[2], TOLERANCE );
        CHECK_CLOSE( asinf(0.0f), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, asin_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface asin_varying() { \n"
            "   y = asin( sin(x) ); \n"
            "}"
        );
        CHECK_CLOSE( asinf(sinf(x[0])), y[0], TOLERANCE );
        CHECK_CLOSE( asinf(sinf(x[1])), y[1], TOLERANCE );
        CHECK_CLOSE( asinf(sinf(x[2])), y[2], TOLERANCE );
        CHECK_CLOSE( asinf(sinf(x[3])), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, cos_constant )
    {
        test(
            "surface cos_uniform() { \n"
            "   y = cos( 3.14159 ); \n"
            "}"
        );
        CHECK_CLOSE( cosf(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( cosf(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( cosf(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( cosf(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, cos_uniform )
    {
        test(
            "surface cos_uniform(uniform float pi = 3.14159;) { \n"
            "   y = cos( pi ); \n"
            "}"
        );
        CHECK_CLOSE( cosf(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( cosf(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( cosf(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( cosf(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, cos_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface cos_varying() { \n"
            "   y = cos( x ); \n"
            "}"
        );
        CHECK_CLOSE( cosf(x[0]), y[0], TOLERANCE );
        CHECK_CLOSE( cosf(x[1]), y[1], TOLERANCE );
        CHECK_CLOSE( cosf(x[2]), y[2], TOLERANCE );
        CHECK_CLOSE( cosf(x[3]), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, acos_constant )
    {
        test(
            "surface acos_constant() { \n"
            "   y = acos( cos(3.14159 / 2) ); \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[3], TOLERANCE );

        test(
            "surface acos_constant() { \n"
            "   y = acos( 0 ); \n"
            "}"
        );
        CHECK_CLOSE( acosf(0.0f), y[0], TOLERANCE );
        CHECK_CLOSE( acosf(0.0f), y[1], TOLERANCE );
        CHECK_CLOSE( acosf(0.0f), y[2], TOLERANCE );
        CHECK_CLOSE( acosf(0.0f), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, acos_uniform )
    {
        test(
            "surface acos_uniform(uniform float pi = 3.14159;) { \n"
            "   y = acos( cos(pi / 2) ); \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[3], TOLERANCE );

        test(
            "surface acos_uniform(uniform float zero = 0;) { \n"
            "   y = acos( zero ); \n"
            "}"
        );
        CHECK_CLOSE( acosf(0.0f), y[0], TOLERANCE );
        CHECK_CLOSE( acosf(0.0f), y[1], TOLERANCE );
        CHECK_CLOSE( acosf(0.0f), y[2], TOLERANCE );
        CHECK_CLOSE( acosf(0.0f), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, acos_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface acos_varying() { \n"
            "   y = acos( cos(x) ); \n"
            "}"
        );
        CHECK_CLOSE( acosf(cosf(x[0])), y[0], TOLERANCE );
        CHECK_CLOSE( acosf(cosf(x[1])), y[1], TOLERANCE );
        CHECK_CLOSE( acosf(cosf(x[2])), y[2], TOLERANCE );
        CHECK_CLOSE( acosf(cosf(x[3])), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, tan_constant )
    {
        test(
            "surface tan_uniform() { \n"
            "   y = tan( 3.14159 ); \n"
            "}"
        );
        CHECK_CLOSE( tanf(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( tanf(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( tanf(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( tanf(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, tan_uniform )
    {
        test(
            "surface tan_uniform(uniform float pi = 3.14159;) { \n"
            "   y = tan( pi ); \n"
            "}"
        );
        CHECK_CLOSE( tanf(float(M_PI)), y[0], TOLERANCE );
        CHECK_CLOSE( tanf(float(M_PI)), y[1], TOLERANCE );
        CHECK_CLOSE( tanf(float(M_PI)), y[2], TOLERANCE );
        CHECK_CLOSE( tanf(float(M_PI)), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, tan_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface tan_varying() { \n"
            "   y = tan( x ); \n"
            "}"
        );
        CHECK_CLOSE( tanf(x[0]), y[0], TOLERANCE );
        CHECK_CLOSE( tanf(x[1]), y[1], TOLERANCE );
        CHECK_CLOSE( tanf(x[2]), y[2], TOLERANCE );
        CHECK_CLOSE( tanf(x[3]), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, atan_constant )
    {
        test(
            "surface atan_constant() { \n"
            "   y = atan( tan(3.14159 / 2) ); \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[3], TOLERANCE );

        test(
            "surface atan_constant() { \n"
            "   y = atan( 0 ); \n"
            "}"
        );
        CHECK_CLOSE( atanf(0.0f), y[0], TOLERANCE );
        CHECK_CLOSE( atanf(0.0f), y[1], TOLERANCE );
        CHECK_CLOSE( atanf(0.0f), y[2], TOLERANCE );
        CHECK_CLOSE( atanf(0.0f), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, atan_uniform )
    {
        test(
            "surface atan_uniform(uniform float pi = 3.14159;) { \n"
            "   y = atan( tan(pi / 2) ); \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI) / 2.0f, y[3], TOLERANCE );

        test(
            "surface atan_uniform(uniform float zero = 0;) { \n"
            "   y = atan( zero ); \n"
            "}"
        );
        CHECK_CLOSE( atanf(0.0f), y[0], TOLERANCE );
        CHECK_CLOSE( atanf(0.0f), y[1], TOLERANCE );
        CHECK_CLOSE( atanf(0.0f), y[2], TOLERANCE );
        CHECK_CLOSE( atanf(0.0f), y[3], TOLERANCE );
    }

    TEST_FIXTURE( MathematicalFunctionTest, atan_varying )
    {
        x[0] = float(M_PI) / 2.0f;
        x[1] = float(M_PI);
        x[2] = 2.0f * float(M_PI);
        x[3] = 4.0f * float(M_PI);
        
        test(
            "surface atan_varying() { \n"
            "   y = atan( tan(x) ); \n"
            "}"
        );
        CHECK_CLOSE( atanf(tanf(x[0])), y[0], TOLERANCE );
        CHECK_CLOSE( atanf(tanf(x[1])), y[1], TOLERANCE );
        CHECK_CLOSE( atanf(tanf(x[2])), y[2], TOLERANCE );
        CHECK_CLOSE( atanf(tanf(x[3])), y[3], TOLERANCE );
    }
    
    TEST_FIXTURE( MathematicalFunctionTest, random_function )
    {
        srand( 0 );
        test( 
            "surface random_function() { \n"
            "   x = random(); \n"
            "}"
        );
        CHECK( x[0] >= -1.0f && x[0] <= 1.0f );
        CHECK( x[1] >= -1.0f && x[1] <= 1.0f );
        CHECK( x[2] >= -1.0f && x[2] <= 1.0f );
        CHECK( x[3] >= -1.0f && x[3] <= 1.0f );
    }
   
    TEST_FIXTURE( MathematicalFunctionTest, float_random_function )
    {
        srand( 0 );
        test( 
            "surface float_random_function() { \n"
            "   x = float random(); \n"
            "}"
        );
        CHECK( x[0] >= 0.0f && x[0] <= 1.0f );
        CHECK( x[1] >= 0.0f && x[1] <= 1.0f );
        CHECK( x[2] >= 0.0f && x[2] <= 1.0f );
        CHECK( x[3] >= 0.0f && x[3] <= 1.0f );
    }
   
    TEST_FIXTURE( MathematicalFunctionTest, color_random_function )
    {
        srand( 0 );
        test( 
            "surface color_random_function() { \n"
            "   Ci = color random(); \n"
            "}"
        );
        for ( int i = 0; i < 4; ++i )
        {
            CHECK( Ci[0].x >= 0.0f && Ci[0].x <= 1.0f );
            CHECK( Ci[0].y >= 0.0f && Ci[0].y <= 1.0f );
            CHECK( Ci[0].z >= 0.0f && Ci[0].z <= 1.0f );
        }
    }
   
    TEST_FIXTURE( MathematicalFunctionTest, point_random_function )
    {
        srand( 0 );
        test( 
            "surface point_random_function() { \n"
            "   P = point random(); \n"
            "}"
        );
        for ( int i = 0; i < 4; ++i )
        {
            CHECK( P[0].x >= 0.0f && P[0].x <= 1.0f );
            CHECK( P[0].y >= 0.0f && P[0].y <= 1.0f );
            CHECK( P[0].z >= 0.0f && P[0].z <= 1.0f );
        }
    }
    
    TEST_FIXTURE( MathematicalFunctionTest, float_deriv_function )
    {
        x[0] = 0.0f;
        x[1] = 1.0f;
        x[2] = 2.0f;
        x[3] = 3.0f;
        
        y[0] = 0.0f;
        y[1] = 1.0f;
        y[2] = 2.0f;
        y[3] = 3.0f;        
        
        test(
            "surface float_deriv_function() { \n"
            "   P = Deriv( y, x ); \n"
            "}"
        );
        
        CHECK_CLOSE( (y[1] - y[0]) / (x[1] - x[0]) + (y[2] - y[0]) / (x[2] - x[0]), P[0].x, TOLERANCE );
        CHECK_CLOSE( (y[1] - y[0]) / (x[1] - x[0]) + (y[3] - y[1]) / (x[3] - x[1]), P[1].x, TOLERANCE );
        CHECK_CLOSE( (y[3] - y[2]) / (x[3] - x[2]) + (y[2] - y[0]) / (x[2] - x[0]), P[2].x, TOLERANCE );
        CHECK_CLOSE( (y[3] - y[2]) / (x[3] - x[2]) + (y[3] - y[1]) / (x[3] - x[1]), P[3].x, TOLERANCE );
    }    
}
