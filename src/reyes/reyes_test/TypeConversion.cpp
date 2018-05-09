
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/assert.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::reyes;

static const float TOLERANCE = 0.01f;

SUITE( TypeConversion )
{
    struct TypeConversionTest
    {
        Grid grid;
        float* x;
        vec3* Cx;
        vec3* Px;
        vec3* Nx;
        vec3* Lx;
     
        TypeConversionTest()
        : grid(),
          x( NULL ),
          Cx( NULL ),
          Px( NULL ),
          Nx( NULL ),
          Lx( NULL )
        {
            grid.resize( 2, 2 );            
            shared_ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();

            shared_ptr<Value> Cx_value = grid.add_value( "Cx", TYPE_COLOR );
            Cx_value->zero();
            Cx = Cx_value->vec3_values();
            
            shared_ptr<Value> Px_value = grid.add_value( "Px", TYPE_POINT );
            Px_value->zero();
            Px = Px_value->vec3_values();
            
            shared_ptr<Value> Nx_value = grid.add_value( "Nx", TYPE_NORMAL );
            Nx_value->zero();
            Nx = Nx_value->vec3_values();
            
            shared_ptr<Value> Lx_value = grid.add_value( "Lx", TYPE_VECTOR );
            Lx_value->zero();
            Lx = Lx_value->vec3_values();            
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
            
            grid["Cx"].zero();
            grid["Px"].zero();
            grid["Nx"].zero();
            grid["Lx"].zero();
            
            Shader shader( source, source + strlen(source), symbol_table, error_policy );
            VirtualMachine virtual_machine;
            virtual_machine.initialize( grid, shader );
            virtual_machine.shade( grid, grid, shader );
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

        test(
            "surface float_to_color() { \n"
            "   Cx = Cx + x; \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, length(Cx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Cx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );

        test(
            "surface float_to_color() { \n"
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

        test(
            "surface float_to_point() { \n"
            "   Px = Px + x; \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, length(Px[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Px[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );

        test(
            "surface float_to_point() { \n"
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

        test(
            "surface float_to_normal() { \n"
            "   Nx = Nx + x; \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, length(Nx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Nx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );

        test(
            "surface float_to_normal() { \n"
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

        test(
            "surface float_to_vector() { \n"
            "   Lx = Lx + x; \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, length(Lx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );

        test(
            "surface float_to_vector() { \n"
            "   Lx = x + Lx; \n"
            "}"
        );
        CHECK_CLOSE( 0.0f, length(Lx[0] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[1] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[2] - vec3(1.0f, 1.0f, 1.0f)), TOLERANCE );
        CHECK_CLOSE( 0.0f, length(Lx[3] - vec3(0.0f, 0.0f, 0.0f)), TOLERANCE );
    }
}
