//
// Renderer.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Renderer.hpp"
#include "Options.hpp"
#include "SampleBuffer.hpp"
#include "ImageBuffer.hpp"
#include "Sampler.hpp"
#include "Grid.hpp"
#include "Cone.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "Hyperboloid.hpp"
#include "Paraboloid.hpp"
#include "Disk.hpp"
#include "Torus.hpp"
#include "CubicPatch.hpp"
#include "LinearPatch.hpp"
#include "Geometry.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "Texture.hpp"
#include "SymbolTable.hpp"
#include "VirtualMachine.hpp"
#include "SymbolTable.hpp"
#include "Attributes.hpp"
#include "ErrorPolicy.hpp"
#include "DisplayMode.hpp"
#include "ImageBufferFormat.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include "assert.hpp"
#include <vector>
#include <list>
#include <algorithm>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits.h>

using std::max;
using std::swap;
using std::list;
using std::map;
using std::vector;
using std::string;
using std::pair;
using std::make_pair;
using std::shared_ptr;
using namespace math;
using namespace reyes;

static const int ATTRIBUTES_RESERVE = 32;
static const char* NULL_SURFACE_SHADER = "surface null() { Ci = Cs; Oi = Os; }";

/**
// Constructor.
*/
Renderer::Renderer()
: error_policy_( nullptr )
, virtual_machine_( nullptr )
, null_surface_shader_( nullptr )
, sample_buffer_( nullptr )
, image_buffer_( nullptr )
, sampler_( nullptr )
, screen_transform_( math::identity() )
, camera_transform_( math::identity() )
, textures_()
, shaders_()
, options_( nullptr )
, attributes_()
{
    error_policy_ = new ErrorPolicy;
    virtual_machine_ = new VirtualMachine( *this );
    null_surface_shader_ = new Shader( NULL_SURFACE_SHADER, NULL_SURFACE_SHADER + strlen(NULL_SURFACE_SHADER), error_policy() );
    options_ = new Options();
    attributes_.reserve( ATTRIBUTES_RESERVE );
}

/**
// Destructor.
*/
Renderer::~Renderer()
{
    attributes_.clear();

    for ( map<string, Shader*>::const_iterator i = shaders_.begin(); i != shaders_.end(); ++i )
    {
        Shader* shader = i->second;
        REYES_ASSERT( shader );
        delete shader;
    }
    shaders_.clear();
    
    for ( map<string, Texture*>::const_iterator i = textures_.begin(); i != textures_.end(); ++i )
    {
        Texture* texture = i->second;
        REYES_ASSERT( texture );
        delete texture;
    }
    textures_.clear();

    delete sampler_;
    sampler_ = nullptr;

    delete image_buffer_;
    image_buffer_ = nullptr;

    delete sample_buffer_;
    sample_buffer_ = nullptr;
    
    delete null_surface_shader_;
    null_surface_shader_ = nullptr;

    delete virtual_machine_;
    virtual_machine_ = nullptr;

    delete options_;
    options_ = nullptr;
    
    delete error_policy_;
    error_policy_ = nullptr;
}

/**
// Get the error policy used to report errors.
//
// @return
//  The ErrorPolicy used to report errors.
*/
ErrorPolicy& Renderer::error_policy() const
{
    return *error_policy_;
}

/**
// Set the global options used when rendering.
//
// The options are referred to at the beginning of each frame marked by a
// call to Renderer::begin() so options must be set *before* this to have
// any effect.
//
// @param options
//  The global options to set.
*/
void Renderer::set_options( const Options& options )
{
    *options_ = options;
}

/**
// Get the global options used when rendering.
//
// @return
//  The global options.
*/
const Options& Renderer::options() const
{
    return *options_;
}

/**
// Push a copy of the the current render state onto the attribute stack.
*/
void Renderer::push_attributes()
{
    REYES_ASSERT( !attributes_.empty() );
    shared_ptr<Attributes> attributes( new Attributes(*attributes_.back()) );
    attributes_.push_back( attributes );
}

/**
// Pop the current render state from the attribute stack.
*/
void Renderer::pop_attributes()
{
    REYES_ASSERT( attributes_.size() > 1 );
    if ( attributes_.size() > 1 )
    {
        attributes_.pop_back();
    }
}

/**
// Get the current render state attributes.
*/
Attributes& Renderer::attributes() const
{
    REYES_ASSERT( !attributes_.empty() );
    return *attributes_.back();
}

/**
// Set the shading rate.
//
// The shading rate is the size of a micropolygon (in pixels) that the 
// renderer tries to dice geometry into.  For example the default shading
// rate of 0.25 tries to dice geometry into micropolygons that are half a
// pixel along each edge or four micropolygons per pixel.
//
// @param shading_rate
//  The value to set the shading rate to.
*/
void Renderer::shading_rate( float shading_rate )
{
    attributes().set_shading_rate( shading_rate );
}

/**
// Set the matte object flag.
//
// When the matte object flag is true then the calculated surface color of
// micropolygons is ignored and transparent black is used in its place.  This
// has the effect of creating a "hole" in the space occupied by any objects 
// rendered with this flag on and the background (or matte painting) is able
// to show through.
//
// @param matte
//  The value to set the matte object flag to.
*/
void Renderer::matte( bool matte )
{
    attributes().set_matte( matte );
}

/**
// Set the two sided flag.
//
// When the two sided flag is true then no culling of micropolygons based on
// their orientation is performed.  Both front-facing and back-facing 
// micropolygons are considered when sampling.
//
// @param two_sided
//  The value to set the two sided flag to.
*/
void Renderer::two_sided( bool two_sided )
{
    attributes().set_two_sided( two_sided );
}

/**
// Set the handedness of front-facing micropolygons to the opposite of the 
// handedness of the current object space.
*/
void Renderer::orient_inside()
{
    Attributes& attributes = Renderer::attributes();
    attributes.set_geometry_left_handed( !attributes.transform_left_handed() );
}

/**
// Set the handedness of front-facing micropolygons to match the handedness
// of the current object space.
*/
void Renderer::orient_outside()
{
    Attributes& attributes = Renderer::attributes();
    attributes.set_geometry_left_handed( attributes.transform_left_handed() );
}

/**
// Set the handedness of front-facing micropolygons to left handed.
*/
void Renderer::orient_left_handed()
{
    attributes().set_geometry_left_handed( true );
}

/**
// Set the handedness of front-facing micropolygons to right handed.
*/
void Renderer::orient_right_handed()
{
    attributes().set_geometry_left_handed( false );
}

/**
// Set the current surface color ("Cs").
//
// @param color
//  The value to set the current color to.
*/
void Renderer::color( const math::vec3& color )
{
    attributes().set_color( color );
}

/**
// Set the current surface opacity ("Os").
//
// @param opacity
//  The value to set the current surface opacity to.
*/
void Renderer::opacity( const math::vec3& opacity )
{
    attributes().set_opacity( opacity );
}

/**
// Mark the beginning of a frame.
//
// Allocates and initializes the sample and image buffers used during 
// rendering according to the global options set in this renderer and 
// initialize the attribute stack to have the default initial render
// state.
*/
void Renderer::begin()
{
    if ( sample_buffer_ )
    {
        delete sample_buffer_;
        sample_buffer_ = nullptr;
    }
       
    if ( image_buffer_ )
    {
        delete image_buffer_;
        image_buffer_ = nullptr;
    }
    
    if ( sampler_ )
    {
        delete sampler_;
        sampler_ = nullptr;
    }
    
    sample_buffer_ = new SampleBuffer( options_->horizontal_resolution(), options_->vertical_resolution(), options_->horizontal_sampling_rate(), options_->vertical_sampling_rate(), options_->filter_width(), options_->filter_height() );
    image_buffer_ = new ImageBuffer( options_->horizontal_resolution(), options_->vertical_resolution(), 4, FORMAT_U8 );
    sampler_ = new Sampler( float(sample_buffer_->width() - 1), float(sample_buffer_->height() - 1), options_->crop_window() );

    screen_transform_ = math::identity();
    camera_transform_ = math::identity();

    shared_ptr<Attributes> attributes( new Attributes(virtual_machine_) );
    attributes_.clear();
    attributes_.push_back( attributes );    
    attributes->set_surface_shader( null_surface_shader_, camera_transform_ );
    attributes->set_u_basis( bezier_basis() );
    attributes->set_v_basis( bezier_basis() );
}

/**
// Mark the end of a frame.
//
// Clear the current attribute stack and filter, expose, and quantize the
// sample buffer down into the image buffer.
*/
void Renderer::end()
{
    REYES_ASSERT( options_ );
    
    attributes_.clear();
    
    ImageBuffer image_buffer;
    sample_buffer_->filter( options_->filter_function(), &image_buffer );
    image_buffer.expose( options_->gain(), options_->gamma() );
    image_buffer_->quantize( image_buffer, options_->one(), options_->minimum(), options_->maximum(), options_->dither() );
}

/**
// Mark the beginning of world space in a frame.
//
// Sets the camera transform to the current transform and clears the current
// transform back to the identity.  Sets up coordinate system transforms for
// the "screen", "camera", and "world" coordinate systems.  Pushes the current
// render state.
*/
void Renderer::begin_world()
{
    camera_transform_ = current_transform();
    identity();

    add_coordinate_system( "screen", inverse(screen_transform_) );
    add_coordinate_system( "camera", math::identity() );
    add_coordinate_system( "world", camera_transform_ );
    
    push_attributes();
}

/**
// Mark the end of world space in a frame.
//
// Sets the current transform back to the identity.  Removes the coordinate
// system transforms for the "screen", "camera", and "world" coordinate 
// systems.  Pops the current render state.
*/
void Renderer::end_world()
{
    identity();
    
    remove_coordinate_system( "world" );
    remove_coordinate_system( "camera" );
    remove_coordinate_system( "screen" );
    
    pop_attributes();
}

/**
// Mark the end of the projection setup.
//
// Sets the screen transform to the current transform and clears the current
// transform back to the identity.  Pushes the current render state.
*/
void Renderer::projection()
{
    screen_transform_ = current_transform();
    identity();
    push_attributes();
}

/**
// Get the screen transform.
//
// @return
//  The screen transform.
*/
const math::mat4x4& Renderer::screen_transform() const
{
    return screen_transform_;
}

/**
// Get the camera transform.
//
// @return 
//  The camera transform.
*/
const math::mat4x4& Renderer::camera_transform() const
{
    return camera_transform_;
}

/**
// Add a named coordinate system to the current render state.
//
// @param name
//  The name of the coordinate system (assumed not null).
//
// @param transform
//  The transform from the named coordinate system to "camera" space.
*/
void Renderer::add_coordinate_system( const char* name, const math::mat4x4& transform )
{
    attributes().add_coordinate_system( name, transform );
}

/**
// Remove a named coordinate system from the current render state.
//
// @param name
//  The name of the coordinate system to remove (assumed not null).
*/
void Renderer::remove_coordinate_system( const char* name )
{
    attributes().remove_coordinate_system( name );    
}

/**
// Get the transformation from the coordinate system specified by \e name
// to "camera" space.
//
// @param name
//  The name of the coordinate system to get the transform from.
//
// @return
//  The transform to "camera" space from the coordinate system \e name or 
//  the identity transform if no such coordinate system is defined.
*/
math::mat4x4 Renderer::transform_from( const std::string& name ) const
{
    // @todo
    //  Determine whether an exception should be thrown if an attempt is made to 
    //  retrieve the transform for a coordinate system that hasn't been defined.
    //  At the moment Renderer::coordinate_system() is asserting and returning
    //  the identity in this case.
    return attributes().transform_from( name );    
}

/**
// Get the transformation from "camera" space to the coordinate system 
// specified by \e name.
//
// @param name
//  The name of the coordinate system to get the transform to.
//
// @return
//  The transform from "camera" space to the coordinate system \e name or 
//  the identity transform if there is no such coordinate system defined.
*/
math::mat4x4 Renderer::transform_to( const std::string& name ) const
{
    return inverse( transform_from(name) );
}

/**
// Get the transformation from the coordinate system \e from to the coordinate
// system \e to.
//
// @param from
//  The named coordinate system to transform from.
//
// @param to
//  The named coordinate system to transform to.
//
// @return
//  The transformation that transforms from coordinate system \e from to the
//  coordinates system \e to.
*/
math::mat4x4 Renderer::transform_between( const std::string& from, const std::string& to ) const
{
    return transform_to( to ) * transform_from( from );
}

/**
// Push the current transform onto a new level of the transform stack.
*/
void Renderer::begin_transform()
{
    attributes().push_transform();
}

/**
// Pop the current transform from the transform stack.
*/
void Renderer::end_transform()
{
    REYES_ASSERT( !attributes_.empty() );
    attributes().pop_transform();
}

/**
// Set the current transform to the identity.
*/
void Renderer::identity()
{
    REYES_ASSERT( !attributes_.empty() );
    attributes().identity();
}

/**
// Set the current transform to \e transform.
//
// @param transform
//  The value to set the current transform to.
*/
void Renderer::transform( const math::mat4x4& transform )
{
    REYES_ASSERT( !attributes_.empty() );
    attributes().transform( transform );
}

/**
// Concatenate the current transform with \e transform.
//
// The transform specified by \e transform is multiplied on the right of the
// current transform to generate the new transform.  In this renderer this has
// the effect of \e transform applying another transform that occurs *before*
// the current transform.
//
// @param transform
//  The value to concatenate with the current transform.
*/
void Renderer::concat_transform( const math::mat4x4& transform )
{
    REYES_ASSERT( !attributes_.empty() );
    attributes().concat_transform( transform );
}

/**
// Concatenate the current transform with an orthographic projection.
//
// The orthographic projection projects the region specified by the screen 
// window in the global options at the near plane onto the framebuffer.
*/
void Renderer::orthographic()
{    
    REYES_ASSERT( options_ );
    const vec4& screen_window = options_->screen_window();
    concat_transform( renderman_orthographic(screen_window.x, screen_window.y, screen_window.z, screen_window.w, options_->near_clip_distance(), options_->far_clip_distance()) );
}

/**
// Concatenate the current transform with a perspective projection.
//
// The field of view is taken to be in either of x or y depending on the 
// current aspect ratio of the global options.  If the aspect is wide then the
// field of view is taken to by in x otherwise the aspect is tall and the
// field of view is taken to be in y.
//
// The current aspect ratio is the pixel aspect ratio times the frame aspect
// ratio (both set in the global options).  Typically the pixel aspect ratio
// is 1 to represent square pixels and the frame aspect ratio is the 
// horizontal resolution of the framebuffer over the vertical resolution of
// the framebuffer.
//
// @param fov
//  The field of view of the perspective projection to concatenate (in 
//  radians).
*/
void Renderer::perspective( float fov )
{
    REYES_ASSERT( options_ );

    float near_clip_distance = options_->near_clip_distance();
    float far_clip_distance = options_->far_clip_distance();
    float aspect_ratio = options_->pixel_aspect_ratio() * options_->frame_aspect_ratio();
    if ( aspect_ratio >= 1.0f )
    {
        float half_width = near_clip_distance * tanf( fov / 2.0f );
        float half_height = half_width / aspect_ratio;
        concat_transform( renderman_perspective(-half_width, half_width, -half_height, half_height, near_clip_distance, far_clip_distance) );
    }
    else
    {
        float half_height = near_clip_distance * tanf( fov / 2.0f );
        float half_width = half_height * aspect_ratio;
        concat_transform( renderman_perspective(-half_width, half_width, -half_height, half_height, near_clip_distance, far_clip_distance) );
    }
}

/**
// Concatenate the current transform with a translation.
//
// @param x, y, z
//  The coordinates of the translation to concatenate.
*/
void Renderer::translate( float x, float y, float z )
{
    concat_transform( math::translate(x, y, z) );
}

/**
// Concatenate the current transform with a translation.
//
// @param translation
//  The the translation to concatenate.
*/
void Renderer::translate( const math::vec3& translation )
{
    concat_transform( math::translate(translation) );
}

/**
// Concatenate the current transform with a rotation.
//
// @param angle
//  The angle of the rotation to apply (ccw looking from the +ive axis 
//  specified towards the origin).
//
// @param x, y, z
//  The axis that the rotation rotates around.
*/
void Renderer::rotate( float angle, float x, float y, float z )
{
    concat_transform( math::rotate(vec3(x, y, z), angle) );
}

/**
// Concatenate the current transform with a scale.
//
// @param x, y, z
//  The scale to apply in x, y, and z.
*/
void Renderer::scale( float x, float y, float z )
{
    concat_transform( math::scale(x, y, z) );
}

/**
// Concatenate the current transform with a rotation vector that looks from
// \e eye to \e at with \e up considered "up".
//
// @param at
//  The point to look at.
//
// @param eye
//  The point to look from.
//
// @param up
//  The direction to consider as "up" when generating the other axis of the
//  look at transform.
*/
void Renderer::look_at( const math::vec3& at, const math::vec3& eye, const math::vec3& up )
{
    concat_transform( math::renderman_look_at(at, eye, up) );
}

/**
// Get the current transform.
//
// @return
//  The current transform.
*/
const math::mat4x4& Renderer::current_transform() const
{
    return attributes().transform();
}

/**
// Set the current displacement shader.
//
// @param displacement_shader
//  The shader to set the current displacement shader to or null to not do any
//  displacement shading.
//
// @return
//  A grid that can be used to set the uniform parameters used by the shader.
*/
Grid& Renderer::displacement_shader( Shader* displacement_shader )
{
    REYES_ASSERT( !attributes_.empty() );
    Attributes& attributes = Renderer::attributes();
    attributes.set_displacement_shader( displacement_shader, camera_transform_ );
    return attributes.displacement_parameters();
}

/**
// Set the current surface shader.
//
// @param surface_shader
//  The shader to set the current surface shader to or null to set the surface
//  shader to the special null surface shader (just passes through surface 
//  color and opacity).
//
// @return
//  A grid that can be used to set the uniform parameters used by the shader.
*/
Grid& Renderer::surface_shader( Shader* surface_shader )
{
    REYES_ASSERT( !attributes_.empty() );
    Attributes& attributes = Renderer::attributes();
    attributes.set_surface_shader( surface_shader ? surface_shader : null_surface_shader_, camera_transform_ );
    return attributes.surface_parameters();
}

/**
// Add a light shader.
//
// @param light_shader
//  The light shader to add (assumed not null).
//
// @return
//  A grid that can be used to set the uniform parameters used by the shader
//  and also to activate and/or deactivate that particular light for different
//  pieces of geometry.
*/
Grid& Renderer::light_shader( Shader* light_shader )
{
    return attributes().add_light_shader( light_shader, camera_transform_ );
}

/**
// Set the current displacement shader.
//
// @param filename
//  The filename of the displacement shader to set the current displacement
//  shader to (assumed not null).
//
// @return
//  A grid that can be used to set the uniform parameters used by the shader.
*/
Grid& Renderer::displacement_shader( const char* filename )
{
    REYES_ASSERT( filename );
    return displacement_shader( shader(filename) );
}

/**
// Set the current surface shader.
//
// @param filename
//  The filename of the surface shader to set the current surface
//  shader to (assumed not null).
//
// @return
//  A grid that can be used to set the uniform parameters used by the shader.
*/
Grid& Renderer::surface_shader( const char* filename )
{
    REYES_ASSERT( filename );
    return surface_shader( shader(filename) );
}

/**
// Add a light shader.
//
// @param light_shader
//  The filename of the light shader to add (assumed not null).
//
// @return
//  A grid that can be used to set the uniform parameters used by the shader
//  and also to activate and/or deactivate that particular light for different
//  pieces of geometry.
*/
Grid& Renderer::light_shader( const char* filename )
{
    REYES_ASSERT( filename );
    return light_shader( shader(filename) );
}

/**
// Activate a light.
//
// @param grid
//  The grid returned by the call to Renderer::light_shader() that was 
//  initially used to add the light.
*/
void Renderer::activate_light_shader( Grid& grid )
{
    attributes().activate_light_shader( grid );
}

/**
// Deactivate a light.
//
// @param grid
//  The grid returned by the call to Renderer::light_shader() that was 
//  initially used to add the light.
*/
void Renderer::deactivate_light_shader( Grid& grid )
{
    attributes().deactivate_light_shader( grid );
}

/**
// Render a cone.
//
// @param height
//  The height of the cone.
//
// @param radius
//  The radius of the cone.
//
// @param thetamax
//  The angle around the cone that is considered part of the surface.
*/
void Renderer::cone( float height, float radius, float thetamax )
{
    split( Cone(height, radius, thetamax) );
}

/**
// Render a sphere.
//
// @param radius
//  The radius of the sphere.
*/
void Renderer::sphere( float radius )
{   
    split( Sphere(radius, -FLT_MAX, FLT_MAX, 2.0f * float(M_PI)) );
}

/**
// Render a sphere.
//
// @param radius
//  The radius of the sphere.
//
// @param zmin
//  The minimum depth that is considered part of the surface.
//
// @param zmax
//  The maximum depth that is considered part of the surface.
//
// @param thetamax
//  The angle around the sphere that is considered part of the surface.
*/
void Renderer::sphere( float radius, float zmin, float zmax, float thetamax )
{   
    split( Sphere(radius, zmin, zmax, thetamax) );
}

/**
// Render a cylinder.
//
// @param radius
//  The radius of the cylinder.
//
// @param zmin
//  The minimum depth that is considered part of the surface.
//
// @param zmax
//  The maximum depth that is considered part of the surface.
//
// @param thetamax
//  The angle around the cylinder that is considered part of the surface.
*/
void Renderer::cylinder( float radius, float zmin, float zmax, float thetamax )
{   
    split( Cylinder(radius, zmin, zmax, thetamax) );
}

/**
// Render a hyperboloid.
//
// @param point1
//  The start of the line that is rotated to generate the hyperboloid.
//
// @param point2
//  The end of the line that is rotated to generate the hyperboloid.
//
// @param thetamax
//  The angle around the hyperboloid that is considered part of the surface.
*/
void Renderer::hyperboloid( const math::vec3& point1, const math::vec3& point2, float thetamax )
{   
    split( Hyperboloid(point1, point2, thetamax) );
}

/**
// Render a paraboloid.
//
// @param rmax
//  The radius at the widest end of the paraboloid.
//
// @param zmin
//  The minimum depth that is considered part of the surface.
//
// @param zmax
//  The maximum depth that is considered part of the surface.
//
// @param thetamax
//  The angle around the cylinder that is considered part of the surface.
*/
void Renderer::paraboloid( float rmax, float zmin, float zmax, float thetamax )
{   
    split( Paraboloid(rmax, zmin, zmax, thetamax) );
}

/**
// Render a disk.
//
// @param height
//  The height of the disk.
//
// @param radius
//  The radius of the disk.
//
// @param thetamax
//  The angle around the disk that is considered part of the surface.
*/
void Renderer::disk( float height, float radius, float thetamax )
{
    split( Disk(height, radius, thetamax) );
}

/**
// Render a torus.
//
// @param rmajor
//  The major radius of the torus.
//
// @param rminor
//  The minor radius of the torus.
//
// @param phimin
//  The minimum amount around the minor radius torus that is considered part 
//  of the surface.
//
// @param phimax
//  The maximum amount around the minor radius of the torus that is 
//  considered part of the surface.
//
// @param thetamax
//  The angle around the major radius of the torus that is considered part
//  of the surface.
*/
void Renderer::torus( float rmajor, float rminor, float phimin, float phimax, float thetamax )
{
    split( Torus(rmajor, rminor, phimin, phimax, thetamax) );
}

/**
// Render a polygon.
//
// @param vertices
//  The number of vertices in the polygon (assumed >= 3).
//
// @param ppositions
//  The positions in the polygon (assumed not null).
//
// @param nnormals
//  The normals at the polygon vertices (assumed not null).
//
// @param ttexture_coordinates
//  The texture coordinates at the polygon vertices (assumed not null).
*/
void Renderer::polygon( int vertices, const math::vec3* ppositions, const math::vec3* nnormals, const math::vec2* ttexture_coordinates )
{
    REYES_ASSERT( vertices >= 3 );
    REYES_ASSERT( ppositions );
    REYES_ASSERT( nnormals );
    REYES_ASSERT( ttexture_coordinates );

    for ( int i = 1; i < vertices - 1; ++i )
    {
        int i0 = 0;
        int i1 = i;
        int i2 = i + 1;
    
        const vec3& p0 = ppositions[i0];
        const vec3& p1 = ppositions[i1];
        const vec3& p2 = ppositions[i2];
        
        const vec3& n0 = nnormals[i0];
        const vec3& n1 = nnormals[i1];
        const vec3& n2 = nnormals[i2];
        
        const vec2& t0 = ttexture_coordinates[i0];
        const vec2& t1 = ttexture_coordinates[i1];
        const vec2& t2 = ttexture_coordinates[i2];
        
        vec3 p3 = (p0 + p1) / 2.0f;
        vec3 n3 = (n0 + n1) / 2.0f;
        vec2 t3 = (t0 + t1) / 2.0f;

        vec3 p4 = (p1 + p2) / 2.0f;
        vec3 n4 = (n1 + n2) / 2.0f;
        vec2 t4 = (t1 + t2) / 2.0f;

        vec3 p5 = (p2 + p0) / 2.0f;
        vec3 n5 = (n2 + n0) / 2.0f;
        vec2 t5 = (t2 + t0) / 2.0f;
        
        vec3 p6 = (p0 + p1 + p2) / 3.0f;
        vec3 n6 = (n0 + n1 + n2) / 3.0f;
        vec2 t6 = (t0 + t1 + t2) / 3.0f;
        
        vec3 positions [4];
        vec3 normals [4];
        vec2 texture_coordinates [4];
        
        positions[0] = p0;
        positions[1] = p3;
        positions[2] = p6;
        positions[3] = p5;
        normals[0] = n0;
        normals[1] = n3;
        normals[2] = n6;
        normals[3] = n5;
        texture_coordinates[0] = t0;
        texture_coordinates[1] = t3;
        texture_coordinates[2] = t6;
        texture_coordinates[3] = t5;
        linear_patch( positions, normals, texture_coordinates );

        positions[0] = p3;
        positions[1] = p1;
        positions[2] = p4;
        positions[3] = p6;
        normals[0] = n3;
        normals[1] = n1;
        normals[2] = n4;
        normals[3] = n6;
        texture_coordinates[0] = t3;
        texture_coordinates[1] = t1;
        texture_coordinates[2] = t4;
        texture_coordinates[3] = t6;
        linear_patch( positions, normals, texture_coordinates );

        positions[0] = p4;
        positions[1] = p2;
        positions[2] = p5;
        positions[3] = p6;
        normals[0] = n4;
        normals[1] = n2;
        normals[2] = n5;
        normals[3] = n6;
        texture_coordinates[0] = t4;
        texture_coordinates[1] = t2;
        texture_coordinates[2] = t5;
        texture_coordinates[3] = t6;
        linear_patch( positions, normals, texture_coordinates );
    }
}

/**
// Render a cubic patch.
//
// @param positions
//  The sixteen positions that define the cubic patch.
*/
void Renderer::cubic_patch( const math::vec3* positions )
{
    const Attributes& attributes = Renderer::attributes();
    split( CubicPatch(positions, attributes.u_basis(), attributes.v_basis()) );
}

/**
// Render a linear patch.
//
// @param postions
//  The positions at the corners of the linear patch.
//
// @param normals
//  The normals at the corners of the linear patch.
//
// @param texure_coordinates
//  The texture coordinates at the corners of the linear patch.
*/
void Renderer::linear_patch( const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates )
{
    split( LinearPatch(positions, normals, texture_coordinates) );
}

/**
// Render a polygon mesh.
//
// @param polygons
//  The number of polygons in the mesh.
//
// @param vertices
//  An array of the number of vertices in each polygon in the mesh (assumed 
//  not null and with a number of elements equal to \e polygons).
//
// @param indices
//  An array of vertex indices for each polygon (assumed not null and with a
//  number of elements equal to the sum of elements in \e vertices).
//
// @param positions
//  The positions at the vertices in the mesh.
//
// @param normals
//  The normals at the vertices in the mesh.
//
// @param texture_coordiantes
//  The texture coordinates at the vertices in the mesh.
*/
void Renderer::polygon_mesh( int polygons, const int* vertices, const int* indices, const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates )
{
    REYES_ASSERT( vertices );
    REYES_ASSERT( indices );
    REYES_ASSERT( positions );
    REYES_ASSERT( normals );
    REYES_ASSERT( texture_coordinates );
    
    int base = 0;
    for ( int i = 0; i < polygons; ++i )
    {
        for ( int j = 1; j < vertices[i] - 1; ++j )
        {
            int v0 = 0;
            int v1 = j;
            int v2 = j + 1;
            
            int i0 = indices[base + v0];
            int i1 = indices[base + v1];
            int i2 = indices[base + v2];
        
            const vec3& p0 = positions[i0];
            const vec3& p1 = positions[i1];
            const vec3& p2 = positions[i2];
            
            const vec3& n0 = normals[i0];
            const vec3& n1 = normals[i1];
            const vec3& n2 = normals[i2];
            
            const vec2& t0 = texture_coordinates[i0];
            const vec2& t1 = texture_coordinates[i1];
            const vec2& t2 = texture_coordinates[i2];
            
            vec3 p3 = (p0 + p1) / 2.0f;
            vec3 n3 = (n0 + n1) / 2.0f;
            vec2 t3 = (t0 + t1) / 2.0f;

            vec3 p4 = (p1 + p2) / 2.0f;
            vec3 n4 = (n1 + n2) / 2.0f;
            vec2 t4 = (t1 + t2) / 2.0f;

            vec3 p5 = (p2 + p0) / 2.0f;
            vec3 n5 = (n2 + n0) / 2.0f;
            vec2 t5 = (t2 + t0) / 2.0f;
            
            vec3 p6 = (p0 + p1 + p2) / 3.0f;
            vec3 n6 = (n0 + n1 + n2) / 3.0f;
            vec2 t6 = (t0 + t1 + t2) / 3.0f;
            
            vec3 ppositions [4];
            vec3 nnormals [4];
            vec2 ttexture_coordinates [4];
            
            ppositions[0] = p0;
            ppositions[1] = p3;
            ppositions[2] = p6;
            ppositions[3] = p5;
            nnormals[0] = n0;
            nnormals[1] = n3;
            nnormals[2] = n6;
            nnormals[3] = n5;
            ttexture_coordinates[0] = t0;
            ttexture_coordinates[1] = t3;
            ttexture_coordinates[2] = t6;
            ttexture_coordinates[3] = t5;
            linear_patch( ppositions, nnormals, ttexture_coordinates );

            ppositions[0] = p3;
            ppositions[1] = p1;
            ppositions[2] = p4;
            ppositions[3] = p6;
            nnormals[0] = n3;
            nnormals[1] = n1;
            nnormals[2] = n4;
            nnormals[3] = n6;
            ttexture_coordinates[0] = t3;
            ttexture_coordinates[1] = t1;
            ttexture_coordinates[2] = t4;
            ttexture_coordinates[3] = t6;
            linear_patch( ppositions, nnormals, ttexture_coordinates );

            ppositions[0] = p4;
            ppositions[1] = p2;
            ppositions[2] = p5;
            ppositions[3] = p6;
            nnormals[0] = n4;
            nnormals[1] = n2;
            nnormals[2] = n5;
            nnormals[3] = n6;
            ttexture_coordinates[0] = t4;
            ttexture_coordinates[1] = t2;
            ttexture_coordinates[2] = t5;
            ttexture_coordinates[3] = t6;
            linear_patch( ppositions, nnormals, ttexture_coordinates );
        }        
        base += vertices[i];
    }
}

/**
// Recursively split geometry until it is small enough to dice.
//
// Geometry is recursively split into smaller and smaller pieces until it can
// be diced into a number of micropolygons that satisfies both the shading 
// rate and the maximum number of micropolygons per grid.  Grids are also 
// culled if their bounds project outside of the screen or on the outside of
// the near or far clipping planes.
//
// The current transform is used to transform the grid from object space into
// camera space where all shading and lighting calculations are performed.  
// The current transform is also use to define the "object" coordinate system
// that can be referred to in shaders at this point.
//
// Once a grid has been split small enough it is shaded, sampled, and then
// discarded.
//
// @param geometry
//  The geometry to split.
*/
void Renderer::split( const Geometry& geometry )
{
    const mat4x4 transform = camera_transform_ * current_transform();
    add_coordinate_system( "object", transform );

    const float WIDTH = float(sample_buffer_->width() - 1);
    const float HEIGHT = float(sample_buffer_->height() - 1);
    const float SAMPLES_PER_PIXEL = float(options_->horizontal_sampling_rate() * options_->vertical_sampling_rate());

    list<shared_ptr<Geometry>> geometries;
    geometries.push_back( shared_ptr<Geometry>(const_cast<Geometry*>(&geometry), [](Geometry* /*geometry*/){}) );
    while ( !geometries.empty() )
    {
        const shared_ptr<Geometry>& geometry = geometries.front();
        REYES_ASSERT( geometry );

        vec3 minimum = vec3( 0.0f, 0.0f, 0.0f );
        vec3 maximum = vec3( 0.0f, 0.0f, 0.0f );

        bool primitive_spans_epsilon_plane = false;
        int width = 0;
        int height = 0;
        
        if ( geometry->boundable() )
        {
            Grid& grid = attributes().surface_parameters();
            geometry->bound( transform, &minimum, &maximum, &grid );
            if ( minimum.z > options_->far_clip_distance() || maximum.z < options_->near_clip_distance() )
            {
                geometries.pop_front();   
                continue;
            }
            
            const float EPSILON = 0.01f;
            primitive_spans_epsilon_plane = minimum.z < EPSILON && geometry->splittable();
            if ( !primitive_spans_epsilon_plane )
            {
                vec3 s[8];
                s[0] = vec3( raster(vec3(minimum.x, minimum.y, minimum.z)) );
                s[1] = vec3( raster(vec3(minimum.x, maximum.y, minimum.z)) );
                s[2] = vec3( raster(vec3(maximum.x, minimum.y, minimum.z)) );
                s[3] = vec3( raster(vec3(maximum.x, maximum.y, minimum.z)) );
                s[4] = vec3( raster(vec3(minimum.x, minimum.y, maximum.z)) );
                s[5] = vec3( raster(vec3(minimum.x, maximum.y, maximum.z)) );
                s[6] = vec3( raster(vec3(maximum.x, minimum.y, maximum.z)) );
                s[7] = vec3( raster(vec3(maximum.x, maximum.y, maximum.z)) );

                vec2 screen_minimum( FLT_MAX, FLT_MAX );
                vec2 screen_maximum( -FLT_MAX, -FLT_MAX );
                for ( int i = 0; i < 8; ++i )
                {
                    screen_minimum.x = std::min( screen_minimum.x, s[i].x );
                    screen_minimum.y = std::min( screen_minimum.y, s[i].y );
                    screen_maximum.x = std::max( screen_maximum.x, s[i].x );
                    screen_maximum.y = std::max( screen_maximum.y, s[i].y );
                }
                
                float x0 = screen_minimum.x;
                float x1 = screen_maximum.x;
                float y0 = screen_minimum.y;
                float y1 = screen_maximum.y;

                if ( x1 < 0.0f || x0 >= WIDTH || y1 < 0.0f || y0 >= HEIGHT )
                {
                    geometries.pop_front();
                    continue;
                }

                float pixels = (x1 - x0) * (y1 - y0) / SAMPLES_PER_PIXEL;
                float micropolygons = pixels / attributes().shading_rate();
                int power = std::max( 0, int(ceilf(lb(micropolygons) / 2.0f)) );
                width = std::min( 1 << power, SHRT_MAX );
                height = std::min( 1 << power, SHRT_MAX );
            }
        }
        
        Grid& grid = attributes().surface_parameters();
        if ( !primitive_spans_epsilon_plane && width * height <= grid.maximum_vertices() && geometry->diceable() )
        {
            geometry->dice( transform, width, height, &grid );
            displacement_shade( grid );
            surface_shade( grid );
            sample( grid );
        }
        else if ( geometry->splittable() )
        {
            geometry->split( &geometries );
        }
        
        geometries.pop_front();
    }

    remove_coordinate_system( "object" );
}

/**
// Displacement shade \e grid.
//
// @param grid
//  The grid to displacement shade.
*/
void Renderer::displacement_shade( Grid& grid )
{
    attributes().displacement_shade( grid );
}

/**
// Surface shader \e grid.
//
// @param grid
//  The grid to surface shade.
*/
void Renderer::surface_shade( Grid& grid )
{
    attributes().surface_shade( grid );
}

/**
// Light shade \e grid.
//
// @param grid
//  The grid to light shade.
*/
void Renderer::light_shade( Grid& grid )
{
    attributes().light_shade( grid );
}

/**
// Sample \e grid.
//
// @param grid
//  The grid to sample.
*/
void Renderer::sample( const Grid& grid )
{
    REYES_ASSERT( sampler_ );    
    const Attributes& attributes = Renderer::attributes();
    bool matte = attributes.matte();
    bool two_sided = attributes.two_sided();
    bool left_handed = attributes.geometry_left_handed();
    sampler_->sample( screen_transform_, grid, matte, two_sided, left_handed, sample_buffer_ );
}

/**
// Save the current contents of the image buffer to a file.
//
// @param format
//  A printf style format string that specifies the name of the file to write
//  the image buffer to (assumed not null).
//
// @param ...
//  Parameters as specified by \e format.
*/
void Renderer::save_image( const char* format, ... ) const
{
    REYES_ASSERT( image_buffer_ );
    REYES_ASSERT( format );

    char filename [1024];
    va_list args;
    va_start( args, format );
    vsnprintf( filename, sizeof(filename), format, args );
    va_end( args );
    filename [sizeof(filename) - 1] = 0;

    image_buffer_->save( filename );
}

/**
// Save the current contents of the image buffer to a PNG file.
//
// @param format
//  A printf style format string that specifies the name of the file to write
//  the image buffer to (assumed not null).
//
// @param ...
//  Parameters as specified by \e format.
*/
void Renderer::save_image_as_png( const char* format, ... ) const
{
    REYES_ASSERT( image_buffer_ );
    REYES_ASSERT( format );
    
    char filename [1024];
    va_list args;
    va_start( args, format );
    vsnprintf( filename, sizeof(filename), format, args );
    va_end( args );
    filename [sizeof(filename) - 1] = 0;

    image_buffer_->save_png( filename );
}

/**
// Save the current contents of the sample buffer to a file.
//
// @param mode
//  The bitwise or'd DisplayMode values that specify which elements of the
//  sample buffer to write to the file.
//
// @param format
//  A printf style format string that specifies the name of the file to write
//  the image buffer to (assumed not null).
//
// @param ...
//  Parameters as specified by \e format.
*/
void Renderer::save_samples( int mode, const char* format, ... ) const
{
    REYES_ASSERT( sample_buffer_ );
    REYES_ASSERT( format );

    char filename [1024];
    va_list args;
    va_start( args, format );
    vsnprintf( filename, sizeof(filename), format, args );
    va_end( args );
    filename [sizeof(filename) - 1] = 0;

    sample_buffer_->save( mode, filename );
}

/**
// Save the current contents of the sample buffer to a PNG file.
//
// The \e mode parameter *must* specify DISPLAY_MODE_RGB and at most one of
// DISPLAY_MODE_Z or DISPLAY_MODE_A.
//
// @param mode
//  The bitwise or'd DisplayMode values that specify which elements of the
//  sample buffer to write to the file.
//
// @param format
//  A printf style format string that specifies the name of the file to write
//  the image buffer to (assumed not null).
//
// @param ...
//  Parameters as specified by \e format.
*/
void Renderer::save_samples_as_png( int mode, const char* format, ... ) const
{
    REYES_ASSERT( sample_buffer_ );
    REYES_ASSERT( format );

    char filename [1024];
    va_list args;
    va_start( args, format );
    vsnprintf( filename, sizeof(filename), format, args );
    va_end( args );
    filename [sizeof(filename) - 1] = 0;

    sample_buffer_->save_png( mode, filename, error_policy_ );
}

/**
// Load a texture map to be referred to by shaders.
//
// The texture is loaded and made available to shaders.  To refer to the 
// texture each shader should use the same string value as passed to
// \e filename to identify it in a texture() call.
//
// @param filename
//  The path to the texture map to load (.png, .jpeg, or .jpg are recognized).
*/
void Renderer::texture( const char* filename )
{
    REYES_ASSERT( filename );
    
    Texture* texture = find_texture( filename );
    if ( !texture )
    {
        texture = new Texture( filename, TEXTURE_COLOR, error_policy_ );
        textures_.insert( make_pair(filename, texture) );
    }
}

/**
// Load an environment map to be referred to by shaders.
//
// The texture is loaded and made available to shaders.  To refer to the 
// texture each shader should use the same string value as passed to
// \e filename to identify it in a texture() call.
//
// @param filename
//  The path to the texture map to load (.png, .jpeg, or .jpg are recognized).
*/
void Renderer::environment( const char* filename )
{
    Texture* texture = find_texture( filename );
    if ( !texture )
    {
        texture = new Texture( filename, TEXTURE_LATLONG_ENVIRONMENT, error_policy_ );
        textures_.insert( make_pair(filename, texture) );
    }
}

/**
// Load a cubic environment map to be referred to by shaders.
//
// The \e filename parameter is expected to contain a '%s' format specified 
// that is replaced by "nx", "pz", "px", "nz", "ny", "py" to generate the 
// filenames of the files to load the six faces of the cubic environment map
// from.
//
// The environment map is loaded and made available to shaders.  To refer to 
// the map each shader should use the same string value as passed to 
// \e filename to identify it in an environment() call.
//
// @param filename
//  The path to the texture map to load (.png, .jpeg, or .jpg are recognized).
*/
void Renderer::cubic_environment( const char* filename )
{
    Texture* texture = find_texture( filename );
    if ( !texture )
    {
        texture = new Texture( filename, TEXTURE_CUBIC_ENVIRONMENT, error_policy_ );
        textures_.insert( make_pair(filename, texture) );
    }
}

/**
// Generate a shadow map from the current contents of the sample buffer.
//
// To refer to the shadow map each shader should use the same string value as 
// passed to \e name to identify it in a shadow() call.
//
// @param name
//  The name to identify the shadow map with (assumed not null).
*/
void Renderer::shadow_from_framebuffer( const char* name )
{
    REYES_ASSERT( name );

    Texture* texture = find_texture( name );
    if ( !texture )
    {
        texture = new Texture( TEXTURE_SHADOW, camera_transform_, screen_transform_ );
        textures_.insert( make_pair(name, texture) );
    }

    REYES_ASSERT( texture->type() == TEXTURE_SHADOW );
    sample_buffer_->pack( DISPLAY_MODE_Z, texture->image_buffers() );
}

/**
// Generate a texture from the current contents of the sample buffer.
//
// To refer to the texture map each shader should use the same string value as 
// passed to \e name to identify it in a texture() call.
//
// @param name
//  The name to identify the texture with (assumed not null).
*/
void Renderer::texture_from_framebuffer( const char* name )
{
    REYES_ASSERT( name );

    Texture* texture = find_texture( name );
    if ( !texture )
    {
        texture = new Texture( TEXTURE_COLOR, math::identity(), math::identity() );
        textures_.insert( make_pair(name, texture) );
    }    

    REYES_ASSERT( texture->type() == TEXTURE_COLOR );
    sample_buffer_->pack( DISPLAY_MODE_RGB | DISPLAY_MODE_A, texture->image_buffers() );
}

/**
// Find a loaded texture.
//
// @param filename
//  The string that identifies the texture (assumed not null).
//
// @return 
//  The texture or null if no such texture could be found.
*/
Texture* Renderer::find_texture( const char* filename ) const
{
    REYES_ASSERT( filename );
    
    map<string, Texture*>::const_iterator i = textures_.find( filename );
    return i != textures_.end() ? i->second : nullptr;
}

/**
// Load or find an existing shader.
//
// Looks for an existing loaded shader that was loaded using the same 
// filename and returns that.  If there is no existing loaded shader with
// that filename then a new shader is loaded and returned.
//
// @param filename
//  The filename of the shader to find or load.
//
// @return
//  The shader.
*/
Shader* Renderer::shader( const char* filename )
{
    REYES_ASSERT( filename );

    Shader* shader = find_shader( filename );
    if ( !shader )
    {
        shader = new Shader( filename, error_policy() );
        shaders_.insert( make_pair(filename, shader) );
    }
    return shader;
}

/**
// Find an existing shader.
//
// @param filename
//  The filename of the shader to find.
//
// @return
//  The shader or null if no matching shader was found.
*/
Shader* Renderer::find_shader( const char* filename ) const
{
    REYES_ASSERT( filename );
    
    map<string, Shader*>::const_iterator i = shaders_.find( filename );
    return i != shaders_.end() ? i->second : nullptr;
}

/**
// Get the bezier basis matrix for cubic patches.
//
// @return
//  An array of sixteen floats that define the bezier basis matrix.
*/
const math::vec4* Renderer::bezier_basis() const
{
    static const float basis [16] =
    {
        -1.0f,  3.0f, -3.0f,  1.0f,
         3.0f, -6.0f,  3.0f,  0.0f,
        -3.0f,  3.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,  0.0f
    };
    return reinterpret_cast<const math::vec4*>(basis);
}

/**
// Get the Bezier basis matrix for cubic patches.
//
// @return
//  An array of sixteen floats that define the Bezier basis matrix.
*/
const math::vec4* Renderer::bspline_basis() const
{
    static const float basis [16] =
    {
        -1.0f / 6.0f,  3.0f / 6.0f, -3.0f / 6.0f,  1.0f / 6.0f,
         3.0f / 6.0f, -6.0f / 6.0f,  3.0f / 6.0f,  0.0f / 6.0f,
        -3.0f / 6.0f,  3.0f / 6.0f,  0.0f / 6.0f,  0.0f / 6.0f,
         1.0f / 6.0f,  0.0f / 6.0f,  0.0f / 6.0f,  0.0f / 6.0f
    };
    return reinterpret_cast<const math::vec4*>(basis);
}

/**
// Get the Catmull-Rom basis matrix for cubic patches.
//
// @return
//  An array of sixteen floats that define the Catmull-Rom basis matrix.
*/
const math::vec4* Renderer::catmull_rom_basis() const
{
    static const float basis [16] =
    {
        -1.0f / 2.0f,  3.0f / 2.0f, -3.0f / 2.0f,  1.0f / 2.0f,
         2.0f / 2.0f, -5.0f / 2.0f,  4.0f / 2.0f, -1.0f / 2.0f,
        -1.0f / 2.0f,  0.0f / 2.0f,  1.0f / 2.0f,  0.0f / 2.0f,
         0.0f / 2.0f,  2.0f / 2.0f,  0.0f / 2.0f,  0.0f / 2.0f
    };
    return reinterpret_cast<const math::vec4*>(basis);
}

/**
// Get the Hermite basis matrix for cubic patches.
//
// @return
//  An array of sixteen floats that define the Hermite basis matrix.
*/
const math::vec4* Renderer::hermite_rom_basis() const
{
    static const float basis [16] =
    {
         2.0f,  1.0f, -2.0f,  1.0f,
        -3.0f, -2.0f,  3.0f, -1.0f,
         0.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,  0.0f
    };
    return reinterpret_cast<const math::vec4*>(basis);
}

/**
// Get the power basis (identity) matrix for cubic patches.
//
// @return
//  An array of sixteen floats that define the power basis matrix.
*/
const math::vec4* Renderer::power_basis() const
{
    static const float basis [16] =
    {
         1.0f,  0.0f,  0.0f,  0.0f,
         0.0f,  1.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  0.0f,  1.0f
    };
    return reinterpret_cast<const math::vec4*>(basis);
}

math::vec4 Renderer::raster( const math::vec3& x ) const
{
    // @todo
    //  Make the Renderer::raster() function take into account the projection
    //  and view transforms to transform from view space into sample space 
    //  correctly.
    const float width = float(sample_buffer_->width() - 1);
    const float height = float(sample_buffer_->height() - 1);    
    return renderman_project( screen_transform_, width, height, x );
}

float Renderer::min( float a, float b, float c, float d ) const
{
    return std::min( std::min(a, b), std::min(c, d) );
}

float Renderer::max( float a, float b, float c, float d ) const
{
    return std::max( std::max(a, b), std::max(c, d) );
}

float Renderer::lb( float x ) const
{
    return log( x ) / log( 2.0f );
}
