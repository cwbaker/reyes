
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/Options.hpp>
#include <sweet/render/Renderer.hpp>
#include <sweet/render/Texture.hpp>
#include <sweet/render/TextureType.hpp>
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

void render_shaders_example()
{  
    Options options;
    options.set_gamma( 1.0f / 2.2f );
    options.set_resolution( 640, 480, 1.0f );
    options.set_dither( 1.0f );
    options.set_filter( &Options::gaussian_filter, 2.0f, 2.0f );

    Renderer renderer;
    renderer.set_options( options );
    renderer.begin();
    renderer.perspective( 0.25f * float(M_PI) );
    renderer.projection();
    renderer.translate( 0.0f, 0.0f, 24.0f );
    renderer.begin_world();

    Grid& ambientlight = renderer.light_shader( SHADERS_PATH "ambientlight.sl" );
    ambientlight["intensity"] = 0.1f;
    ambientlight["lightcolor"] = vec3( 1.0f, 1.0f, 1.0f );

    Grid& pointlight = renderer.light_shader( SHADERS_PATH "pointlight.sl" );
    pointlight["intensity"] = 4096.0f;
    pointlight["lightcolor"] = vec3( 1.0f, 1.0f, 1.0f );
    pointlight["from"] = vec3( 25.0f, 25.0f, -50.0f );

    const float WIDTH = 15.5f;
    const float HEIGHT = 12.5f;
    const float ACROSS = float(8 - 1);
    const float DOWN = float(7 - 1);

    float y = HEIGHT / 2.0f - HEIGHT / DOWN * float(0);
    renderer.push_attributes();
    renderer.surface_shader( SHADERS_PATH "constant.sl" );
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.color( rgb_from_hsv(vec3(360.0f * float(i) / 8.0f, 0.95f, 0.75f)) );
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();
    
    y = HEIGHT / 2.0f - HEIGHT / DOWN * float(1);
    renderer.push_attributes();
    renderer.surface_shader( SHADERS_PATH "matte.sl" );    
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.color( rgb_from_hsv(vec3(360.0f * float(i) / 8.0f, 0.95f, 0.75f)) );
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();
    
    y = HEIGHT / 2.0f - HEIGHT / DOWN * float(2);
    renderer.push_attributes();
    Grid& metal = renderer.surface_shader( SHADERS_PATH "metal.sl" );    
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.color( rgb_from_hsv(vec3(360.0f * float(i) / 8.0f, 0.95f, 0.75f)) );
        metal["roughness"] = 0.005f + float(i) * 0.005f;
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();
    
    y = HEIGHT / 2.0f - HEIGHT / DOWN * float(3);
    renderer.push_attributes();
    Grid& plastic = renderer.surface_shader( SHADERS_PATH "plastic.sl" );    
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.color( rgb_from_hsv(vec3(360.0f * float(i) / 8.0f, 0.95f, 0.75f)) );
        plastic["roughness"] = 0.01f + float(i) * 0.005f;
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();    

    y = HEIGHT / 2.0f - HEIGHT / DOWN * float(4);
    renderer.push_attributes();
    renderer.texture( RENDER_EXAMPLES_PATH "bumpy.jpg" );
    renderer.color( vec3(1.0f, 1.0f, 1.0f) );
    Grid& paintedplastic = renderer.surface_shader( SHADERS_PATH "paintedplastic.sl" );    
    paintedplastic["texturename"] = RENDER_EXAMPLES_PATH "bumpy.jpg";
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.rotate( 0.5f * float(M_PI), 1.0f, 0.0f, 0.0f );
        paintedplastic["roughness"] = 0.01f + float(i) * 0.05f;
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();    

    y = HEIGHT / 2.0f - HEIGHT / DOWN * float(5);
    renderer.push_attributes();
    renderer.two_sided( true );
    renderer.texture( RENDER_EXAMPLES_PATH "bumpy.jpg" );
    Grid& bumpy = renderer.displacement_shader( SHADERS_PATH "bumpy.sl" );
    bumpy["texturename"] = RENDER_EXAMPLES_PATH "bumpy.jpg";
    Grid& bumpy_plastic = renderer.surface_shader( SHADERS_PATH "plastic.sl" );    
    bumpy_plastic["Ka"] = 0.2f;
    bumpy_plastic["Kd"] = 0.4f;
    bumpy_plastic["Ks"] = 0.4f;
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.rotate( 0.5f * float(M_PI), 1.0f, 0.0f, 0.0f );
        bumpy["Km"] = 0.05f + 0.05f * float(i) / 6.0f;
        renderer.color( rgb_from_hsv(vec3(360.0f * float(i) / 8.0f, 0.95f, 0.75f)) );
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();    

    y = HEIGHT / 2.0f - HEIGHT / DOWN * float(6);
    renderer.push_attributes();
    renderer.environment( RENDER_EXAMPLES_PATH "st-eutropius.jpg" );
    Grid& shinymetal = renderer.surface_shader( SHADERS_PATH "shinymetal.sl" );
    shinymetal["Ka"] = 0.2f;
    shinymetal["Ks"] = 0.4f;
    shinymetal["Kr"] = 0.4f;
    shinymetal["roughness"] = 0.01f;
    shinymetal["texturename"] = RENDER_EXAMPLES_PATH "st-eutropius.jpg";
    for ( int i = 0; i < 8; ++i )
    {
        renderer.identity();
        renderer.translate( vec3(-WIDTH / 2.0f + WIDTH / ACROSS * float(i), y, 0.0f) );
        renderer.rotate( float(M_PI), 1.0f, 0.0f, 0.0f );
        renderer.color( rgb_from_hsv(vec3(360.0f * float(i) / 8.0f, 0.95f, 0.75f)) );
        renderer.sphere( 1.0f );
    }
    renderer.pop_attributes();    

    renderer.end_world();
    renderer.end();
    renderer.save_image_as_png( RENDER_EXAMPLES_PATH "shaders.png" );
}
