
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/Options.hpp>
#include <sweet/render/Renderer.hpp>
#include <sweet/math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

void render_wavy_sphere_example()
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
    ambientlight["intensity"] = 0.2f;
    ambientlight["lightcolor"] = vec3( 1.0f, 1.0f, 1.0f );

    Grid& pointlight = renderer.light_shader( SHADERS_PATH "pointlight.sl" );
    pointlight["intensity"] = 4096.0f;
    pointlight["lightcolor"] = vec3( 1.0f, 1.0f, 1.0f );
    pointlight["from"] = vec3( 25.0f, 25.0f, -50.0f );

    Grid& wavy = renderer.displacement_shader( SHADERS_PATH "wavy.sl" );
    wavy["Km"] = 0.2f;
    wavy["sfreq"] = 24.0f;
    wavy["tfreq"] = 32.0f;
    
    Grid& plastic = renderer.surface_shader( SHADERS_PATH "plastic.sl" );
    plastic["Ka"] = 0.2f;
    plastic["Kd"] = 0.4f;
    plastic["Ks"] = 0.4f;
    plastic["roughness"] = 0.05f;
    
    renderer.identity();
    renderer.translate( vec3(0.0f, 0.0f, 0.0f) );
    renderer.rotate( 0.5f * float(M_PI), 1.0f, 0.0f, 0.0f );
    renderer.two_sided( true );
    renderer.color( vec3(0.3f, 0.55f, 0.75f) );
    renderer.sphere( 6.0f );

    renderer.end_world();
    renderer.end();
    renderer.save_image_as_png( RENDER_EXAMPLES_PATH "wavy_sphere.png" );
}
