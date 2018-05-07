
#include <UnitTest++/UnitTest++.h>
#include <sweet/render/Renderer.hpp>
#include <sweet/render/VirtualMachine.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdio.h>

using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

static const float TOLERANCE = 0.01f;

SUITE( MatrixFunctions )
{
    struct MatrixFunctionTest
    {
        Renderer renderer;
        Grid grid;
        mat4x4* M;
        float* x;
     
        MatrixFunctionTest()
        : grid(),
          M( NULL ),
          x( NULL )
        {
            renderer.begin();
            renderer.perspective( float(M_PI) / 2.0f );
            renderer.projection();
            renderer.begin_world();

            grid.resize( 2, 2 );
            
            shared_ptr<Value> M_value = grid.add_value( "M", TYPE_MATRIX, STORAGE_UNIFORM );
            M_value->zero();
            M = M_value->mat4x4_values();
            
            shared_ptr<Value> P_value = grid.add_value( "P", TYPE_POINT );
            P_value->zero();
            
            shared_ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
        }
        
        void test( const char* source )
        {
            renderer.symbol_table().add_symbols()
                ( "M", TYPE_MATRIX, STORAGE_UNIFORM )
                ( "x", TYPE_FLOAT )
            ;
            Shader shader( source, source + strlen(source), renderer.symbol_table(), renderer.error_policy() );
            renderer.surface_shader( &shader );
            renderer.surface_shade( grid );
        }
    };

    TEST_FIXTURE( MatrixFunctionTest, comp )
    {
        M[0] = identity();
        
        for ( int i = 0; i < 16; ++i )
        {
            int row = i / 4;
            int column = i % 4;
            
            char shader [1024];
            sprintf( shader, "surface comp_test() { x = comp( M, %d, %d ); }", row, column );
            test( shader );
            
            float expected = M[0].m[row * 4 + column];
            CHECK_CLOSE( expected, x[0], TOLERANCE );
            CHECK_CLOSE( expected, x[1], TOLERANCE );
            CHECK_CLOSE( expected, x[2], TOLERANCE );
            CHECK_CLOSE( expected, x[3], TOLERANCE );
        }
    }

    TEST_FIXTURE( MatrixFunctionTest, setcomp )
    {
        for ( int i = 0; i < 16; ++i )
        {
            int row = i / 4;
            int column = i % 4;
            
            char shader [1024];
            sprintf( shader, "surface setcomp_test() { setcomp( M, %d, %d, 2 ); }", row, column );
            test( shader );
            
            CHECK_CLOSE( 2.0f, M[0].m[row * 4 + column], TOLERANCE );
        }
    }

    TEST_FIXTURE( MatrixFunctionTest, determinant )
    {
        M[0] = identity();
        
        test( "surface determinant_test() { x = determinant( M ); }" );
        
        float expected = determinant( M[0] );
        CHECK_CLOSE( expected, x[0], TOLERANCE );
        CHECK_CLOSE( expected, x[1], TOLERANCE );
        CHECK_CLOSE( expected, x[2], TOLERANCE );
        CHECK_CLOSE( expected, x[3], TOLERANCE );
    }

    TEST_FIXTURE( MatrixFunctionTest, translate )
    {
        M[0] = identity();
        
        test( "surface translate_test() { M = translate( M, (1, 2, 3) ); }" );
        
        CHECK_CLOSE( 1.0f, M[0].m[3], TOLERANCE );
        CHECK_CLOSE( 2.0f, M[0].m[7], TOLERANCE );
        CHECK_CLOSE( 3.0f, M[0].m[11], TOLERANCE );
    }

    TEST_FIXTURE( MatrixFunctionTest, rotate )
    {
        M[0] = identity();
        
        test( "surface rotate_test() { M = rotate( M, PI / 2, (0, 0, 1) ); }" );
        
        mat4x4 expected = rotate( vec3(0.0f, 0.0f, 1.0f), float(M_PI) / 2.0f );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );
    }

    TEST_FIXTURE( MatrixFunctionTest, scale )
    {
        M[0] = identity();
        
        test( "surface scale_test() { M = scale( M, (1, 2, 3) ); }" );
        
        mat4x4 expected = scale( vec3(1.0f, 2.0f, 3.0f) );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );
    }
    
    TEST_FIXTURE( MatrixFunctionTest, zero_constructor )
    {
        M[0] = identity();
        test( "surface zero_constructor_test() { M = 0; }" );
        mat4x4 expected( 
            0.0f, 0.0f, 0.0f, 0.0f, 
            0.0f, 0.0f, 0.0f, 0.0f, 
            0.0f, 0.0f, 0.0f, 0.0f, 
            0.0f, 0.0f, 0.0f, 0.0f 
        );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );
    }
    
    TEST_FIXTURE( MatrixFunctionTest, identity_constructor )
    {
        M[0] = identity();
        test( "surface identity_constructor_test() { M = 1; }" );
        mat4x4 expected( 
            1.0f, 0.0f, 0.0f, 0.0f, 
            0.0f, 1.0f, 0.0f, 0.0f, 
            0.0f, 0.0f, 1.0f, 0.0f, 
            0.0f, 0.0f, 0.0f, 1.0f 
        );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );
    }
    
    TEST_FIXTURE( MatrixFunctionTest, sixteentuple_constructor )
    {
        M[0] = identity();
        test( "surface sixteentuple_constructor() { M = matrix (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16); }" );
        mat4x4 expected = mat4x4(
            1.0f, 2.0f, 3.0f, 4.0f, 
            5.0f, 6.0f, 7.0f, 8.0f,
            9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f
        );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );
    }
    
    TEST_FIXTURE( MatrixFunctionTest, transforming_constructor )
    {
        M[0] = identity();
        test( "surface world_transforming_constructor() { M = matrix \"world\" 1; }" );
        mat4x4 expected = renderer.transform_to( "world" );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );

        M[0] = identity();
        test( "surface screen_transforming_constructor() { M = matrix \"screen\" 1; }" );
        expected = renderer.transform_to( "screen" );
        CHECK_CLOSE( expected.m[0], M[0].m[0], TOLERANCE );
        CHECK_CLOSE( expected.m[1], M[0].m[1], TOLERANCE );
        CHECK_CLOSE( expected.m[2], M[0].m[2], TOLERANCE );
        CHECK_CLOSE( expected.m[3], M[0].m[3], TOLERANCE );
        CHECK_CLOSE( expected.m[4], M[0].m[4], TOLERANCE );
        CHECK_CLOSE( expected.m[5], M[0].m[5], TOLERANCE );
        CHECK_CLOSE( expected.m[6], M[0].m[6], TOLERANCE );
        CHECK_CLOSE( expected.m[7], M[0].m[7], TOLERANCE );
        CHECK_CLOSE( expected.m[8], M[0].m[8], TOLERANCE );
        CHECK_CLOSE( expected.m[9], M[0].m[9], TOLERANCE );
        CHECK_CLOSE( expected.m[10], M[0].m[10], TOLERANCE );
        CHECK_CLOSE( expected.m[11], M[0].m[11], TOLERANCE );
        CHECK_CLOSE( expected.m[12], M[0].m[12], TOLERANCE );
        CHECK_CLOSE( expected.m[13], M[0].m[13], TOLERANCE );
        CHECK_CLOSE( expected.m[14], M[0].m[14], TOLERANCE );
        CHECK_CLOSE( expected.m[15], M[0].m[15], TOLERANCE );
    }
}
