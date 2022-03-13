
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Scope.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/assert.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

static const float TOLERANCE = 0.01f;

SUITE( TypeConversion )
{
    struct TypeConversionTest
    {
        float x [4];
        vec3 Cx [4];
        vec3 Px [4];
        vec3 Nx [4];
        vec3 Lx [4];
     
        TypeConversionTest()
        : x{}
        , Cx{}
        , Px{}
        , Nx{}
        , Lx{}
        {
        }
        
        void test( const char* source )
        {
            ErrorPolicy error_policy;

            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "Cx", TYPE_COLOR )
                ( "Px", TYPE_POINT )
                ( "Nx", TYPE_NORMAL )
                ( "Lx", TYPE_VECTOR )
            ;

            Shader shader;
            shader.load_memory( source, source + strlen(source), symbol_table, error_policy );

            Grid grid;
            grid.set_symbols( shader.symbols() );
            grid.resize( 2, 2 );
            grid.zero();

            float* xx = grid.float_value( "x" );
            vec3* Cxx = grid.vec3_value( "Cx" );
            vec3* Pxx = grid.vec3_value( "Px" );
            vec3* Nxx = grid.vec3_value( "Nx" );
            vec3* Lxx = grid.vec3_value( "Lx" );
            if ( xx && Cxx && Pxx && Nxx && Lxx )
            {                
                memcpy( xx, x, sizeof(float) * grid.size() );
                memcpy( Cxx, Cx, sizeof(vec3) * grid.size() );
                memcpy( Pxx, Px, sizeof(vec3) * grid.size() );
                memcpy( Nxx, Nx, sizeof(vec3) * grid.size() );
                memcpy( Lxx, Lx, sizeof(vec3) * grid.size() );
                VirtualMachine virtual_machine;
                virtual_machine.initialize( grid, shader );
                virtual_machine.shade( grid, shader );
                memcpy( x, xx, sizeof(float) * grid.size() );
                memcpy( Cx, Cxx, sizeof(vec3) * grid.size() );
                memcpy( Px, Pxx, sizeof(vec3) * grid.size() );
                memcpy( Nx, Nxx, sizeof(vec3) * grid.size() );
                memcpy( Lx, Lxx, sizeof(vec3) * grid.size() );
            }
        }
    };

    TEST_FIXTURE( TypeConversionTest, float_to_color )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface float_to_color() { \n"
            "   Cx = x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Cx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, color_plus_float_to_color )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface color_plus_float_to_color() { \n"
            "   Cx = Cx + x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Cx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_plus_color_to_color )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface float_plus_color_to_color() { \n"
            "   Cx = x + Cx; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Cx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_to_point )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        
        test(
            "surface float_to_point() { \n"
            "   Px = x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Px[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, point_plus_float_to_point )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        
        test(
            "surface point_plus_float_to_point() { \n"
            "   Px = Px + x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Px[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_plus_point_to_point )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;
        
        test(
            "surface float_plus_point_to_point() { \n"
            "   Px = x + Px; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Px[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_to_normal )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface float_to_normal() { \n"
            "   Nx = x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Nx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, normal_plus_float_to_normal )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface normal_plus_float_to_normal() { \n"
            "   Nx = Nx + x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Nx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_plus_normal_to_normal )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface float_plus_normal_to_normal() { \n"
            "   Nx = x + Nx; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Nx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_to_vector )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface float_to_vector() { \n"
            "   Lx = x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Lx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, vector_plus_float_to_vector )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface vector_plus_float_to_vector() { \n"
            "   Lx = Lx + x; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Lx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }

    TEST_FIXTURE( TypeConversionTest, float_plus_vector_to_vector )
    {
        x[0] = 1.0f;
        x[2] = 1.0f;

        test(
            "surface float_plus_vector_to_vector() { \n"
            "   Lx = x + Lx; \n"
            "}"
        );

        CHECK_CLOSE( 0.0f, length(Lx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }
}
