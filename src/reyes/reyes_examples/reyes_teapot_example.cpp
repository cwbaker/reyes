
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/Options.hpp>
#include <reyes/Renderer.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

static void render_teapot( Renderer& renderer );

void render_teapot_example()
{
    const vec3 light_position( 16.0f, 16.0f, -32.0f );

    Options options;
    options.set_resolution( 512, 512, 1.0f );
    options.set_near_clip_distance( 32.0f );
    options.set_far_clip_distance( 128.0f );

    Renderer renderer;
    renderer.set_options( options );
    renderer.begin();
    renderer.shading_rate( 1.0f );
    renderer.perspective( float(M_PI) / 12.0f );
    renderer.projection();
    renderer.look_at( vec3(0.0f, 0.0f, 0.0f), light_position, vec3(0.0f, 1.0f, 0.0f) );
    renderer.begin_world();
    render_teapot( renderer );
    renderer.end_world();
    renderer.end();
    renderer.shadow_from_framebuffer( "shadow_map" );

    options.set_gamma( 1.0f / 2.2f );
    options.set_resolution( 640, 480, 1.0f );
    options.set_near_clip_distance( 2.0f );
    options.set_far_clip_distance( 256.0f );    
    options.set_dither( 2.0f );
    options.set_gain( 1.5f );
    options.set_filter( &Options::gaussian_filter, 2.0f, 2.0f );

    renderer.set_options( options );
    renderer.begin();
    renderer.shading_rate( 0.25f );
    renderer.perspective( 0.45f * float(M_PI) );
    renderer.projection();
    renderer.translate( 0.5f, -2.4f, 6.0f );
    renderer.begin_world();

    Grid& ambientlight = renderer.light_shader( SHADERS_PATH "ambientlight.sl" );
    ambientlight["intensity"] = 0.1f;
    ambientlight["lightcolor"] = vec3( 1.0f, 1.0f, 1.0f );

    Grid& shadowpointlight = renderer.light_shader( SHADERS_PATH "shadowpointlight.sl" );
    shadowpointlight["intensity"] = 2048.0f;
    shadowpointlight["lightcolor"] = vec3( 1.0f, 1.0f, 1.0f );
    shadowpointlight["from"] = light_position;
    shadowpointlight["shadowname"] = "shadow_map";
    shadowpointlight["bias"] = 0.15f;

    render_teapot( renderer );

    const vec3 P0( -64.0f, 0.0f, 64.0f );
    const vec3 P1( 64.0f, 0.0f, 64.0f );
    const vec3 P2( 64.0f, 0.0f, -64.0f );
    const vec3 P3( -64.0f, 0.0f, -64.0f );
    const vec3 N( 0.0f, 1.0f, 0.0f );
    const vec2 T0( 0.0f, 1.0f );
    const vec2 T1( 1.0f, 1.0f );    
    const vec2 T2( 1.0f, 0.0f );    
    const vec2 T3( 0.0f, 0.0f );    
    renderer.identity();
    renderer.color( vec3(0.6f, 0.6f, 0.6f) );
    renderer.surface_shader( SHADERS_PATH "matte.sl" );
    vec3 positions[] = { P0, P1, P2, P3 };
    vec3 normals[] = { N, N, N, N };
    vec2 texture_coordinates[] = { T0, T1, T2, T3 };
    renderer.orient_left_handed();
    renderer.polygon( 4, positions, normals, texture_coordinates );

    renderer.end_world();
    renderer.end();
    renderer.save_image_as_png( REYES_EXAMPLES_PATH "teapot.png" );
}

static void render_teapot( Renderer& renderer )
{
    renderer.push_attributes();
    renderer.two_sided( true );
    renderer.orient_right_handed();
    renderer.rotate( -0.1f * float(M_PI), 1.0f, 0.0f, 0.0f );
    renderer.rotate( 0.85f * float(M_PI), 0.0f, 1.0f, 0.0f );

    Grid& plastic = renderer.surface_shader( SHADERS_PATH "plastic.sl" );
    plastic["Ka"] = 1.0f;
    plastic["roughness"] = 0.0125f;

    renderer.color( vec3(0.5f, 0.0f, 0.0f) );

    const vec3 patch_0[] =
    {
        vec3( 1.4f, 2.25f, 0.0f ), 
        vec3( 1.3375f, 2.38125f, 0.0f ),
        vec3( 1.4375f, 2.38125f, 0.0f ),
        vec3( 1.5f, 2.25f, 0.0f ),
        
        vec3( 1.4f, 2.25f, 0.784f ),
        vec3( 1.3375f, 2.38125f, 0.749f ),
        vec3( 1.4375f, 2.38125f, 0.805f ),
        vec3( 1.5f, 2.25f, 0.84f ),
        
        vec3( 0.784f, 2.25f, 1.4f ),
        vec3( 0.749f, 2.38125f, 1.3375f ),
        vec3( 0.805f, 2.38125f, 1.4375f ),
        vec3( 0.84f, 2.25f, 1.5f ),
                
        vec3( 0.0f, 2.25f, 1.4f ),
        vec3( 0.0f, 2.38125f, 1.3375f ),
        vec3( 0.0f, 2.38125f, 1.4375f ),
        vec3( 0.0f, 2.25f, 1.5f ),
    };
    renderer.cubic_patch( patch_0 );

    const vec3 patch_1[] =
    {
        vec3( 0.0f, 2.25f, 1.4f ),
        vec3( 0.0f, 2.38125f, 1.3375f ),
        vec3( 0.0f, 2.38125f, 1.4375f ),
        vec3( 0.0f, 2.25f, 1.5f ),
        
        vec3( -0.784f, 2.25f, 1.4f ),
        vec3( -0.749f, 2.38125f, 1.3375f ),
        vec3( -0.805f, 2.38125f, 1.4375f ),
        vec3( -0.84f, 2.25f, 1.5f ),
        
        vec3( -1.4f, 2.25f, 0.784f ),
        vec3( -1.3375f, 2.38125f, 0.749f ),
        vec3( -1.4375f, 2.38125f, 0.805f ),
        vec3( -1.5f, 2.25f, 0.84f ),
        
        vec3( -1.4f, 2.25f, 0.0f ),
        vec3( -1.3375f, 2.38125f, 0.0f ),
        vec3( -1.4375f, 2.38125f, 0.0f ),
        vec3( -1.5f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_1 );
    
    const vec3 patch_2[] =
    {
        vec3( -1.4f, 2.25f, 0.0f ),
        vec3( -1.3375f, 2.38125f, 0.0f ),
        vec3( -1.4375f, 2.38125f, 0.0f ),
        vec3( -1.5f, 2.25f, 0.0f ),
        
        vec3( -1.4f, 2.25f, -0.784f ),
        vec3( -1.3375f, 2.38125f, -0.749f ),
        vec3( -1.4375f, 2.38125f, -0.805f ),
        vec3( -1.5f, 2.25f, -0.84f ),
        
        vec3( -0.784f, 2.25f, -1.4f ),
        vec3( -0.749f, 2.38125f, -1.3375f ),
        vec3( -0.805f, 2.38125f, -1.4375f ),
        vec3( -0.84f, 2.25f, -1.5f ),
        
        vec3( 0.0f, 2.25f, -1.4f ),
        vec3( 0.0f, 2.38125f, -1.3375f ),
        vec3( 0.0f, 2.38125f, -1.4375f ),
        vec3( 0.0f, 2.25f, -1.5f )
    };
    renderer.cubic_patch( patch_2 );

    const vec3 patch_3[] =
    {
        vec3( 0.0f, 2.25f, -1.4f ),
        vec3( 0.0f, 2.38125f, -1.3375f ),
        vec3( 0.0f, 2.38125f, -1.4375f ),
        vec3( 0.0f, 2.25f, -1.5f ),
        
        vec3( 0.784f, 2.25f, -1.4f ),
        vec3( 0.749f, 2.38125f, -1.3375f ),
        vec3( 0.805f, 2.38125f, -1.4375f ),
        vec3( 0.84f, 2.25f, -1.5f ),
        
        vec3( 1.4f, 2.25f, -0.784f ),
        vec3( 1.3375f, 2.38125f, -0.749f ),
        vec3( 1.4375f, 2.38125f, -0.805f ),
        vec3( 1.5f, 2.25f, -0.84f ),
        
        vec3( 1.4f, 2.25f, 0.0f ),
        vec3( 1.3375f, 2.38125f, 0.0f ),
        vec3( 1.4375f, 2.38125f, 0.0f ),
        vec3( 1.5f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_3 );
    
    const vec3 patch_4[] =
    {
        vec3( 1.5f, 2.25f, 0.0f ),
        vec3( 1.75f, 1.725f, 0.0f ),
        vec3( 2.0f, 1.2f, 0.0f ),
        vec3( 2.0f, 0.75f, 0.0f ),
        
        vec3( 1.5f, 2.25f, 0.84f ),
        vec3( 1.75f, 1.725f, 0.98f ),
        vec3( 2.0f, 1.2f, 1.12f ),
        vec3( 2.0f, 0.75f, 1.12f ),
        
        vec3( 0.84f, 2.25f, 1.5f ),
        vec3( 0.98f, 1.725f, 1.75f ),
        vec3( 1.12f, 1.2f, 2.0f ),
        vec3( 1.12f, 0.75f, 2.0f ),
        
        vec3( 0.0f, 2.25f, 1.5f ),
        vec3( 0.0f, 1.725f, 1.75f ),
        vec3( 0.0f, 1.2f, 2.0f ),
        vec3( 0.0f, 0.75f, 2.0f )
    };
    renderer.cubic_patch( patch_4 );

    const vec3 patch_5[] =
    {
        vec3( 0.0f, 2.25f, 1.5f ),
        vec3( 0.0f, 1.725f, 1.75f ),
        vec3( 0.0f, 1.2f, 2.0f ),
        vec3( 0.0f, 0.75f, 2.0f ),
        
        vec3( -0.84f, 2.25f, 1.5f ),
        vec3( -0.98f, 1.725f, 1.75f ),
        vec3( -1.12f, 1.2f, 2.0f ),
        vec3( -1.12f, 0.75f, 2.0f ),
        
        vec3( -1.5f, 2.25f, 0.84f ),
        vec3( -1.75f, 1.725f, 0.98f ),
        vec3( -2.0f, 1.2f, 1.12f ),
        vec3( -2.0f, 0.75f, 1.12f ),
        
        vec3( -1.5f, 2.25f, 0.0f ),
        vec3( -1.75f, 1.725f, 0.0f ),
        vec3( -2.0f, 1.2f, 0.0f ),
        vec3( -2.0f, 0.75f, 0.0f )
    };
    renderer.cubic_patch( patch_5 );
    
    const vec3 patch_6[] = 
    {
        vec3( -1.5f, 2.25f, 0.0f ),
        vec3( -1.75f, 1.725f, 0.0f ),
        vec3( -2.0f, 1.2f, 0.0f ),
        vec3( -2.0f, 0.75f, 0.0f ),
        
        vec3( -1.5f, 2.25f, -0.84f ),
        vec3( -1.75f, 1.725f, -0.98f ),
        vec3( -2.0f, 1.2f, -1.12f ),
        vec3( -2.0f, 0.75f, -1.12f ),
        
        vec3( -0.84f, 2.25f, -1.5f ),
        vec3( -0.98f, 1.725f, -1.75f ),
        vec3( -1.12f, 1.2f, -2.0f ),
        vec3( -1.12f, 0.75f, -2.0f ),
        
        vec3( 0.0f, 2.25f, -1.5f ),
        vec3( 0.0f, 1.725f, -1.75f ),
        vec3( 0.0f, 1.2f, -2.0f ),
        vec3( 0.0f, 0.75f, -2.0f )
    };
    renderer.cubic_patch( patch_6 );
    
    const vec3 patch_7[] =
    {
        vec3( 0.0f, 2.25f, -1.5f ),
        vec3( 0.0f, 1.725f, -1.75f ),
        vec3( 0.0f, 1.2f, -2.0f ),
        vec3( 0.0f, 0.75f, -2.0f ),
        
        vec3( 0.84f, 2.25f, -1.5f ),
        vec3( 0.98f, 1.725f, -1.75f ),
        vec3( 1.12f, 1.2f, -2.0f ),
        vec3( 1.12f, 0.75f, -2.0f ),
        
        vec3( 1.5f, 2.25f, -0.84f ),
        vec3( 1.75f, 1.725f, -0.98f ),
        vec3( 2.0f, 1.2f, -1.12f ),
        vec3( 2.0f, 0.75f, -1.12f ),
        
        vec3( 1.5f, 2.25f, 0.0f ),
        vec3( 1.75f, 1.725f, 0.0f ),
        vec3( 2.0f, 1.2f, 0.0f ),
        vec3( 2.0f, 0.75f, 0.0f )
    };
    renderer.cubic_patch( patch_7 );
    
    const vec3 patch_8[] =
    {
        vec3( 2.0f, 0.75f, 0.0f ),
        vec3( 2.0f, 0.3f, 0.0f ),
        vec3( 1.5f, 0.075f, 0.0f ),
        vec3( 1.5f, 0.0f, 0.0f ),
        
        vec3( 2.0f, 0.75f, 1.12f ),
        vec3( 2.0f, 0.3f, 1.12f ),
        vec3( 1.5f, 0.075f, 0.84f ),
        vec3( 1.5f, 0.0f, 0.84f ),
        
        vec3( 1.12f, 0.75f, 2.0f ),
        vec3( 1.12f, 0.3f, 2.0f ),
        vec3( 0.84f, 0.075f, 1.5f ),
        vec3( 0.84f, 0.0f, 1.5f ),
        
        vec3( 0.0f, 0.75f, 2.0f ),
        vec3( 0.0f, 0.3f, 2.0f ),
        vec3( 0.0f, 0.075f, 1.5f ),
        vec3( 0.0f, 0.0f, 1.5f )
    };
    renderer.cubic_patch( patch_8 );
    
    const vec3 patch_9[] =
    {
        vec3( 0.0f, 0.75f, 2.0f ),
        vec3( 0.0f, 0.3f, 2.0f ),
        vec3( 0.0f, 0.075f, 1.5f ),
        vec3( 0.0f, 0.0f, 1.5f ),
        
        vec3( -1.12f, 0.75f, 2.0f ),
        vec3( -1.12f, 0.3f, 2.0f ),
        vec3( -0.84f, 0.075f, 1.5f ),
        vec3( -0.84f, 0.0f, 1.5f ),
        
        vec3( -2.0f, 0.75f, 1.12f ),
        vec3( -2.0f, 0.3f, 1.12f ),
        vec3( -1.5f, 0.075f, 0.84f ),
        vec3( -1.5f, 0.0f, 0.84f ),
        
        vec3( -2.0f, 0.75f, 0.0f ),
        vec3( -2.0f, 0.3f, 0.0f ),
        vec3( -1.5f, 0.075f, 0.0f ),
        vec3( -1.5f, 0.0f, 0.0f )
    };
    renderer.cubic_patch( patch_9 );
    
    const vec3 patch_10[] =
    {
        vec3( -2.0f, 0.75f, 0.0f ),
        vec3( -2.0f, 0.3f, 0.0f ),
        vec3( -1.5f, 0.075f, 0.0f ),
        vec3( -1.5f, 0.0f, 0.0f ),
        
        vec3( -2.0f, 0.75f, -1.12f ),
        vec3( -2.0f, 0.3f, -1.12f ),
        vec3( -1.5f, 0.075f, -0.84f ),
        vec3( -1.5f, 0.0f, -0.84f ),
        
        vec3( -1.12f, 0.75f, -2.0f ),
        vec3( -1.12f, 0.3f, -2.0f ),
        vec3( -0.84f, 0.075f, -1.5f ),
        vec3( -0.84f, 0.0f, -1.5f ),
        
        vec3( 0.0f, 0.75f, -2.0f ),
        vec3( 0.0f, 0.3f, -2.0f ),
        vec3( 0.0f, 0.075f, -1.5 ),
        vec3( 0.0f, 0.0f, -1.5f )
    };
    renderer.cubic_patch( patch_10 );
    
    const vec3 patch_11[] =
    {
        vec3( 0.0f, 0.75f, -2.0f ),
        vec3( 0.0f, 0.3f, -2.0f ),
        vec3( 0.0f, 0.075f, -1.5f ),
        vec3( 0.0f, 0.0f, -1.5f ),
        
        vec3( 1.12f, 0.75f, -2.0f ),
        vec3( 1.12f, 0.3f, -2.0f ),
        vec3( 0.84f, 0.075f, -1.5f ),
        vec3( 0.84f, 0.0f, -1.5f ),
        
        vec3( 2.0f, 0.75f, -1.12f ),
        vec3( 2.0f, 0.3f, -1.12f ),
        vec3( 1.5f, 0.075f, -0.84f ),
        vec3( 1.5f, 0.0f, -0.84f ),
        
        vec3( 2.0f, 0.75f, 0.0f ),
        vec3( 2.0f, 0.3f, 0.0f ),
        vec3( 1.5f, 0.075f, 0.0f ),
        vec3( 1.5f, 0.0f, 0.0f )
    };
    renderer.cubic_patch( patch_11 );
        
    const vec3 patch_12[] =
    {
        vec3( -1.6f, 1.875f, 0.0f ),
        vec3( -2.3f, 1.875f, 0.0f ),
        vec3( -2.7f, 1.875f, 0.0f ),
        vec3( -2.7f, 1.65f, 0.0f ),
        
        vec3( -1.6f, 1.875f, 0.3f ),
        vec3( -2.3f, 1.875f, 0.3f ),
        vec3( -2.7f, 1.875f, 0.3f ),
        vec3( -2.7f, 1.65f, 0.3f ),
        
        vec3( -1.5f, 2.1f, 0.3f ),
        vec3( -2.5f, 2.1f, 0.3f ),
        vec3( -3.0f, 2.1f, 0.3f ),
        vec3( -3.0f, 1.65f, 0.3f ),
        
        vec3( -1.5f, 2.1f, 0.0f ),
        vec3( -2.5f, 2.1f, 0.0f ),
        vec3( -3.0f, 2.1f, 0.0f ),
        vec3( -3.0f, 1.65f, 0.0f )
    };
    renderer.cubic_patch( patch_12 );
      
    const vec3 patch_13[] =
    {      
        vec3( -1.5f, 2.1f, 0.0f ),
        vec3( -2.5f, 2.1f, 0.0f ),
        vec3( -3.0f, 2.1f, 0.0f ),
        vec3( -3.0f, 1.65f, 0.0f ),
                
        vec3( -1.5f, 2.1f, -0.3f ),
        vec3( -2.5f, 2.1f, -0.3f ),
        vec3( -3.0f, 2.1f, -0.3f ),
        vec3( -3.0f, 1.65f, -0.3f ),
        
        vec3( -1.6f, 1.875f, -0.3f ),
        vec3( -2.3f, 1.875f, -0.3f ),
        vec3( -2.7f, 1.875f, -0.3f ),
        vec3( -2.7f, 1.65f, -0.3f ),
        
        vec3( -1.6f, 1.875f, 0.0f ),
        vec3( -2.3f, 1.875f, 0.0f ),
        vec3( -2.7f, 1.875f, 0.0f ),
        vec3( -2.7f, 1.65f, 0.0f )
    };
    renderer.cubic_patch( patch_13 );
    
    const vec3 patch_14[] =
    {       
        vec3( -2.7f, 1.65f, 0.0f ),
        vec3( -2.7f, 1.425f, 0.0f ),
        vec3( -2.5f, 0.975f, 0.0f ),
        vec3( -2.0f, 0.75f, 0.0f ),
        
        vec3( -2.7f, 1.65f, 0.3f ),
        vec3( -2.7f, 1.425f, 0.3f ),
        vec3( -2.5f, 0.975f, 0.3f ),
        vec3( -2.0f, 0.75f, 0.3f ),
        
        vec3( -3.0f, 1.65f, 0.3f ),
        vec3( -3.0f, 1.2f, 0.3f ),
        vec3( -2.65f, 0.7875f, 0.3f ),
        vec3( -1.9f, 0.45f, 0.3f ),
        
        vec3( -3.0f, 1.65f, 0.0f ),
        vec3( -3.0f, 1.2f, 0.0f ),
        vec3( -2.65f, 0.7875f, 0.0f ),
        vec3( -1.9f, 0.45f, 0.0f )
    };
    renderer.cubic_patch( patch_14 );
      
    const vec3 patch_15[] =
    {  
        vec3( -3.0f, 1.65f, 0.0f ),
        vec3( -3.0f, 1.2f, 0.0f ),
        vec3( -2.65f, 0.7875f, 0.0f ),
        vec3( -1.9f, 0.45f, 0.0f ),
        
        vec3( -3.0f, 1.65f, -0.3f ),
        vec3( -3.0f, 1.2f, -0.3f ),
        vec3( -2.65f, 0.7875f, -0.3f ),
        vec3( -1.9f, 0.45f, -0.3f ),
        
        vec3( -2.7f, 1.65f, -0.3f ),
        vec3( -2.7f, 1.425f, -0.3f ),
        vec3( -2.5f, 0.975f, -0.3f ),
        vec3( -2.0f, 0.75f, -0.3f ),
        
        vec3( -2.7f, 1.65f, 0.0f ),
        vec3( -2.7f, 1.425f, 0.0f ),
        vec3( -2.5f, 0.975f, 0.0f ),
        vec3( -2.0f, 0.75f, 0.0f )
    };
    renderer.cubic_patch( patch_15 );
           
    const vec3 patch_16[] =
    {
        vec3( 1.7f, 1.275f, 0.0f ),
        vec3( 2.6f, 1.275f, 0.0f ),
        vec3( 2.3f, 1.95f, 0.0f ),
        vec3( 2.7f, 2.25f, 0.0f ),
        
        vec3( 1.7f, 1.275f, 0.66f ),
        vec3( 2.6f, 1.275f, 0.66f ),
        vec3( 2.3f, 1.95f, 0.25f ),
        vec3( 2.7f, 2.25f, 0.25f ),
        
        vec3( 1.7f, 0.45f, 0.66f ),
        vec3( 3.1f, 0.675f, 0.66f ),
        vec3( 2.4f, 1.875f, 0.25f ),
        vec3( 3.3f, 2.25f, 0.25f ),
        
        vec3( 1.7f, 0.45f, 0.0f ),
        vec3( 3.1f, 0.675f, 0.0f ),
        vec3( 2.4f, 1.875f, 0.0f ),
        vec3( 3.3f, 2.25f, 0.0f )     
    };  
    renderer.cubic_patch( patch_16 );
              
    const vec3 patch_17[] =
    {
        vec3( 1.7f, 0.45f, 0.0f ),
        vec3( 3.1f, 0.675f, 0.0f ),
        vec3( 2.4f, 1.875f, 0.0f ),
        vec3( 3.3f, 2.25f, 0.0f ),
        
        vec3( 1.7f, 0.45f, -0.66f ),
        vec3( 3.1f, 0.675f, -0.66f ),
        vec3( 2.4f, 1.875f, -0.25f ),
        vec3( 3.3f, 2.25f, -0.25f ),
        
        vec3( 1.7f, 1.275f, -0.66f ),
        vec3( 2.6f, 1.275f, -0.66f ),
        vec3( 2.3f, 1.95f, -0.25f ),
        vec3( 2.7f, 2.25f, -0.25 ),
        
        vec3( 1.7f, 1.275f, 0.0f ),
        vec3( 2.6f, 1.275f, 0.0f ),
        vec3( 2.3f, 1.95f, 0.0f ),
        vec3( 2.7f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_17 );
            
    const vec3 patch_18[] = 
    {
        vec3( 2.7f, 2.25f, 0.0f ),
        vec3( 2.8f, 2.325f, 0.0f ),
        vec3( 2.9f, 2.325f, 0.0f ),
        vec3( 2.8f, 2.25f, 0.0f ),
        
        vec3( 2.7f, 2.25f, 0.25f ),
        vec3( 2.8f, 2.325f, 0.25f ),
        vec3( 2.9f, 2.325f, 0.15f ),
        vec3( 2.8f, 2.25f, 0.15f ),
        
        vec3( 3.3f, 2.25f, 0.25f ),
        vec3( 3.525f, 2.34375f, 0.25f ),
        vec3( 3.45f, 2.3625f, 0.15f ),
        vec3( 3.2f, 2.25f, 0.15f ),
        
        vec3( 3.3f, 2.25f, 0.0f ),
        vec3( 3.525f, 2.34375f, 0.0f ),
        vec3( 3.45f, 2.3625f, 0.0f ),
        vec3( 3.2f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_18 );

    const vec3 patch_19[] =
    {        
        vec3( 3.3f, 2.25f, 0.0f ),
        vec3( 3.525f, 2.34375f, 0.0f ),
        vec3( 3.45f, 2.3625f, 0.0f ),
        vec3( 3.2f, 2.25f, 0.0f ),
        
        vec3( 3.3f, 2.25f, -0.25f ),
        vec3( 3.525f, 2.34375f, -0.25f ),
        vec3( 3.45f, 2.3625f, -0.15f ),
        vec3( 3.2f, 2.25f, -0.15f ),
        
        vec3( 2.7f, 2.25f, -0.25f ),
        vec3( 2.8f, 2.325f, -0.25f ),
        vec3( 2.9f, 2.325f, -0.15f ),
        vec3( 2.8f, 2.25f, -0.15f ),
        
        vec3( 2.7f, 2.25f, 0.0f ),
        vec3( 2.8f, 2.325f, 0.0f ),
        vec3( 2.9f, 2.325f, 0.0f ),
        vec3( 2.8f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_19 );
            
    const vec3 patch_20[] =
    {
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( 0.8f, 3.0f, 0.0f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.2f, 2.55f, 0.0f ),
        
        vec3( 0.0f, 3.0f, 0.002f ),
        vec3( 0.8f, 3.0f, 0.45f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.2f, 2.55f, 0.112f ),
        
        vec3( 0.002f, 3.0f, 0.0f ),
        vec3( 0.45f, 3.0f, 0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.112f, 2.55f, 0.2f ),
        
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( 0.0f, 3.0f, 0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.0f, 2.55f, 0.2f )
    };
    renderer.cubic_patch( patch_20 );

    const vec3 patch_21[] =
    {                
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( 0.0f, 3.0f, 0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.0f, 2.55f, 0.2f ),
        
        vec3( -0.002f, 3.0f, 0.0f ),
        vec3( -0.45f, 3.0f, 0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( -0.112f, 2.55f, 0.2f ),
        
        vec3( 0.0f, 3.0f, 0.002f ),
        vec3( -0.8f, 3.0f, 0.45f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( -0.2f, 2.55f, 0.112f ),
        
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( -0.8f, 3.0f, 0.0f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( -0.2f, 2.55f, 0.0f )
    };
    renderer.cubic_patch( patch_21 );

    const vec3 patch_22[] =
    {                
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( -0.8f, 3.0f, 0.0f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( -0.2f, 2.55f, 0.0f ),
        
        vec3( 0.0f, 3.0f, -0.002f ),
        vec3( -0.8f, 3.0f, -0.45f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( -0.2f, 2.55f, -0.112f ),
        
        vec3( -0.002f, 3.0f, 0.0f ),
        vec3( -0.45f, 3.0f, -0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( -0.112f, 2.55f, -0.2f ),
        
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( 0.0f, 3.0f, -0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.0f, 2.55f, -0.2f )
    };
    renderer.cubic_patch( patch_22 );

    const vec3 patch_23[] =
    {
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( 0.0f, 3.0f, -0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.0f, 2.55f, -0.2f ),
        
        vec3( 0.002f, 3.0f, 0.0f ),
        vec3( 0.45f, 3.0f, -0.8f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.112f, 2.55f, -0.2f ),
        
        vec3( 0.0f, 3.0f, -0.002f ),
        vec3( 0.8f, 3.0f, -0.45f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.2f, 2.55f, -0.112f ),
        
        vec3( 0.0f, 3.0f, 0.0f ),
        vec3( 0.8f, 3.0f, 0.0f ),
        vec3( 0.0f, 2.7f, 0.0f ),
        vec3( 0.2f, 2.55f, 0.0f )
    };
    renderer.cubic_patch( patch_23 );

    const vec3 patch_24[] =
    {            
        vec3( 0.2f, 2.55f, 0.0f ),
        vec3( 0.4f, 2.4f, 0.0f ),
        vec3( 1.3f, 2.4f, 0.0f ),
        vec3( 1.3f, 2.25f, 0.0f ),
        
        vec3( 0.2f, 2.55f, 0.112f ),
        vec3( 0.4f, 2.4f, 0.224f ),
        vec3( 1.3f, 2.4f, 0.728f ),
        vec3( 1.3f, 2.25f, 0.728f ),
        
        vec3( 0.112f, 2.55f, 0.2f ),
        vec3( 0.224f, 2.4f, 0.4f ),
        vec3( 0.728f, 2.4f, 1.3f ),
        vec3( 0.728f, 2.25f, 1.3f ),
        
        vec3( 0.0f, 2.55f, 0.2f ),
        vec3( 0.0f, 2.4f, 0.4f ),
        vec3( 0.0f, 2.4f, 1.3f ),
        vec3( 0.0f, 2.25f, 1.3f )
    };
    renderer.cubic_patch( patch_24 );

    const vec3 patch_25[] =
    {                
        vec3( 0.0f, 2.55f, 0.2f ),
        vec3( 0.0f, 2.4f, 0.4f ),
        vec3( 0.0f, 2.4f, 1.3f ),
        vec3( 0.0f, 2.25f, 1.3f ),
        
        vec3( -0.112f, 2.55f, 0.2f ),
        vec3( -0.224f, 2.4f, 0.4f ),
        vec3( -0.728f, 2.4f, 1.3f ),
        vec3( -0.728f, 2.25f, 1.3f ),
        
        vec3( -0.2f, 2.55f, 0.112f ),
        vec3( -0.4f, 2.4f, 0.224f ),
        vec3( -1.3f, 2.4f, 0.728f ),
        vec3( -1.3f, 2.25f, 0.728f ),
        
        vec3( -0.2f, 2.55f, 0.0f ),
        vec3( -0.4f, 2.4f, 0.0f ),
        vec3( -1.3f, 2.4f, 0.0f ),
        vec3( -1.3f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_25 );

    const vec3 patch_26[] =
    {                
        vec3( -0.2f, 2.55f, 0.0f ),
        vec3( -0.4f, 2.4f, 0.0f ),
        vec3( -1.3f, 2.4f, 0.0f ),
        vec3( -1.3f, 2.25f, 0.0f ),
        
        vec3( -0.2f, 2.55f, -0.112f ),
        vec3( -0.4f, 2.4f, -0.224f ),
        vec3( -1.3f, 2.4f, -0.728f ),
        vec3( -1.3f, 2.25f, -0.728f ),
        
        vec3( -0.112f, 2.55f, -0.2f ),
        vec3( -0.224f, 2.4f, -0.4f ),
        vec3( -0.728f, 2.4f, -1.3f ),
        vec3( -0.728f, 2.25f, -1.3f ),
        
        vec3( 0.0f, 2.55f, -0.2f ),
        vec3( 0.0f, 2.4f, -0.4f ),
        vec3( 0.0f, 2.4f, -1.3f ),
        vec3( 0.0f, 2.25f, -1.3f )
    };
    renderer.cubic_patch( patch_26 );
            
    const vec3 patch_27[] =
    {
        vec3( 0.0f, 2.55f, -0.2f ),
        vec3( 0.0f, 2.4f, -0.4f ),
        vec3( 0.0f, 2.4f, -1.3f ),
        vec3( 0.0f, 2.25f, -1.3f ),
        
        vec3( 0.112f, 2.55f, -0.2f ),
        vec3( 0.224f, 2.4f, -0.4f ),
        vec3( 0.728f, 2.4f, -1.3f ),
        vec3( 0.728f, 2.25f, -1.3f ),
        
        vec3( 0.2f, 2.55f, -0.112f ),
        vec3( 0.4f, 2.4f, -0.224f ),
        vec3( 1.3f, 2.4f, -0.728f ),
        vec3( 1.3f, 2.25f, -0.728f ),
        
        vec3( 0.2f, 2.55f, 0.0f ),
        vec3( 0.4f, 2.4f, 0.0f ),
        vec3( 1.3f, 2.4f, 0.0f ),
        vec3( 1.3f, 2.25f, 0.0f )
    };
    renderer.cubic_patch( patch_27 );
    renderer.pop_attributes();
}

