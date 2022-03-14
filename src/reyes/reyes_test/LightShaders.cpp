
#include <UnitTest++/UnitTest++.h>
#include <reyes/Grid.hpp>
#include <reyes/Scope.hpp>
#include <reyes/Segment.hpp>
#include <reyes/Shader.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Scope.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/assert.hpp>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

using math::vec3;
using std::vector;
using namespace reyes;

SUITE( TestLightShaders )
{
    TEST( AmbientLight )
    {
        //SymbolTable symbol_table;
        ErrorPolicy error_policy;
        Shader shader( SHADERS_PATH "ambientlight.sl", /*symbol_table,*/ error_policy );
    }

    TEST( DistantLight )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        Shader shader( SHADERS_PATH "distantlight.sl", symbol_table, error_policy );
    }
    
    TEST( PointLight )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        Shader shader( SHADERS_PATH "pointlight.sl", symbol_table, error_policy );
    }
    
    TEST( SpotLight )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        Shader shader( SHADERS_PATH "spotlight.sl", symbol_table, error_policy );
    }
    
    TEST( TestShadowPointLight )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        Shader shader( SHADERS_PATH "shadowpointlight.sl", symbol_table, error_policy );
    }
    
    TEST( active_light_list )
    {
        Renderer renderer;
        renderer.begin();
        renderer.perspective( float(float(M_PI)) / 2.0f );
        renderer.projection();
        renderer.begin_world();
        Grid& light = renderer.light_shader( SHADERS_PATH "ambientlight.sl" );
        Shader constant_shader( SHADERS_PATH "constant.sl", renderer.error_policy() );

        {
            Grid& grid = renderer.surface_shader( &constant_shader );
            grid.resize( 2, 2 );
            renderer.light_shade( grid );
            CHECK( grid.lights().size() == 1 );
        }
        
        {
            renderer.deactivate_light_shader( light );
            Grid& grid = renderer.surface_shader( &constant_shader );
            grid.resize( 2, 2 );
            renderer.light_shade( grid );
            CHECK( grid.lights().size() == 0 );
        }
        
        {
            renderer.activate_light_shader( light );
            Grid& grid = renderer.surface_shader( &constant_shader );
            grid.resize( 2, 2 );
            renderer.light_shade( grid );
            CHECK( grid.lights().size() == 1 );
        }
    }
}
