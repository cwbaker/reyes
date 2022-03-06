//
// Attributes.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Attributes.hpp"
#include "Value.hpp"
#include "Grid.hpp"
#include "Shader.hpp"
#include "VirtualMachine.hpp"
#include "assert.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <string.h>

using std::map;
using std::pair;
using std::string;
using std::vector;
using std::swap;
using std::make_pair;
using std::shared_ptr;
using namespace math;
using namespace reyes;

Attributes::Attributes( VirtualMachine* virtual_machine )
: virtual_machine_( virtual_machine ),
  shading_rate_( 0.25f ),
  matte_( false ),
  two_sided_( false ),
  transform_left_handed_( true ),
  geometry_left_handed_( true ),
  color_( 0.0f, 0.0f, 0.0f ),
  opacity_( 1.0f, 1.0f, 1.0f ),
  u_basis_( nullptr ),
  v_basis_( nullptr ),
  displacement_grid_( nullptr ),
  displacement_shader_( nullptr ),
  surface_grid_( nullptr ),
  surface_shader_( nullptr ),
  light_shaders_(),
  active_light_shaders_(),
  transforms_(),
  named_transforms_()
{
    REYES_ASSERT( virtual_machine_ );
    displacement_grid_ = new Grid();
    surface_grid_ = new Grid();

    const unsigned int LIGHT_SHADERS_RESERVE = 32;
    light_shaders_.reserve( LIGHT_SHADERS_RESERVE );

    const unsigned int TRANSFORMS_RESERVE = 32;
    transforms_.reserve( TRANSFORMS_RESERVE );
    transforms_.push_back( math::identity() );
}

Attributes::Attributes( const Attributes& attributes )
: virtual_machine_( attributes.virtual_machine_ ),
  shading_rate_( attributes.shading_rate_ ),
  matte_( attributes.matte_ ),
  two_sided_( attributes.two_sided_ ),
  transform_left_handed_( attributes.transform_left_handed_ ),
  geometry_left_handed_( attributes.geometry_left_handed_ ),
  color_( attributes.color_ ),
  opacity_( attributes.opacity_ ),
  u_basis_( attributes.u_basis_ ),
  v_basis_( attributes.v_basis_ ),
  displacement_grid_( nullptr ),
  displacement_shader_( attributes.displacement_shader_ ),
  surface_grid_( nullptr ),
  surface_shader_( attributes.surface_shader_ ),
  light_shaders_( attributes.light_shaders_ ),
  active_light_shaders_( attributes.active_light_shaders_ ),
  transforms_( attributes.transforms_ ),
  named_transforms_( attributes.named_transforms_ )
{
    REYES_ASSERT( virtual_machine_ );
    displacement_grid_ = new Grid( *attributes.displacement_grid_ );
    surface_grid_ = new Grid( *attributes.surface_grid_ );
}

Attributes::~Attributes()
{
    delete surface_grid_;
    surface_grid_ = nullptr;
    
    delete displacement_grid_;
    displacement_grid_ = nullptr;
}

float Attributes::shading_rate() const
{
    return shading_rate_;
}

bool Attributes::matte() const
{
    return matte_;
}

bool Attributes::two_sided() const
{
    return two_sided_;
}

bool Attributes::transform_left_handed() const
{
    return transform_left_handed_;
}

bool Attributes::geometry_left_handed() const
{
    return geometry_left_handed_;
}

const math::vec3& Attributes::color() const
{
    return color_;
}

const math::vec3& Attributes::opacity() const
{
    return opacity_;
}

const math::vec4* Attributes::u_basis() const
{
    return u_basis_;
}

const math::vec4* Attributes::v_basis() const
{
    return v_basis_;
}

void Attributes::set_shading_rate( float shading_rate )
{
    REYES_ASSERT( shading_rate > 0.0f );
    shading_rate_ = shading_rate > 0.0f ? shading_rate : 1.0f;
}

void Attributes::set_matte( bool matte )
{
    matte_ = matte;
}

void Attributes::set_two_sided( bool two_sided )
{
    two_sided_ = two_sided;
}

void Attributes::set_transform_left_handed( bool transform_left_handed )
{
    transform_left_handed_ = transform_left_handed;
}

void Attributes::set_geometry_left_handed( bool geometry_left_handed )
{
    geometry_left_handed_ = geometry_left_handed;
}

void Attributes::set_color( const math::vec3& color )
{
    color_ = color;
}

void Attributes::set_opacity( const math::vec3& opacity )
{
    opacity_ = opacity;
}

void Attributes::set_u_basis( const math::vec4* u_basis )
{
    REYES_ASSERT( u_basis );
    u_basis_ = u_basis;
}

void Attributes::set_v_basis( const math::vec4* v_basis )
{
    REYES_ASSERT( v_basis );
    v_basis_ = v_basis;
}

void Attributes::displacement_shade( Grid& grid )
{
    if ( displacement_shader_ )
    {
        grid.generate_normals( geometry_left_handed() );

        displacement_grid_->resize( grid.width(), grid.height() );
        memcpy( displacement_grid_->vec3_value("P"), grid.vec3_value("P"), sizeof(vec3) * displacement_grid_->size() );
        memcpy( displacement_grid_->vec3_value("N"), grid.vec3_value("N"), sizeof(vec3) * displacement_grid_->size() );
        memcpy( displacement_grid_->vec3_value("I"), grid.vec3_value("I"), sizeof(vec3) * displacement_grid_->size() );
        memcpy( displacement_grid_->float_value("s"), grid.float_value("s"), sizeof(float) * displacement_grid_->size() );
        memcpy( displacement_grid_->float_value("t"), grid.float_value("t"), sizeof(float) * displacement_grid_->size() );

        add_coordinate_system( "current", math::identity() );
        add_coordinate_system( "shader", displacement_grid_->get_transform() );
        virtual_machine_->shade( *displacement_grid_, *displacement_shader_ );
        remove_coordinate_system( "shader" );
        remove_coordinate_system( "current" );

        memcpy( grid.vec3_value("P"), displacement_grid_->vec3_value("P"), sizeof(vec3) * grid.size() );
        grid.generate_normals( geometry_left_handed(), true );
    }
}

void Attributes::set_displacement_shader( Shader* displacement_shader, const math::mat4x4& camera_transform )
{
    displacement_grid_->clear();
    displacement_shader_ = displacement_shader;
    if ( displacement_shader_ )
    {
        displacement_grid_->set_shader( displacement_shader_ );
        displacement_grid_->set_transform( camera_transform * transforms_.back() );
        add_coordinate_system( "current", math::identity() );
        add_coordinate_system( "shader", displacement_grid_->get_transform() );
        virtual_machine_->initialize( *displacement_grid_, *displacement_shader_ );
        remove_coordinate_system( "shader" );
        remove_coordinate_system( "current" );
    }
}

Shader* Attributes::displacement_shader() const
{
    return displacement_shader_;
}

Grid& Attributes::displacement_parameters() const
{
    return *displacement_grid_;
}

void Attributes::surface_shade( Grid& grid )
{
    if ( surface_shader_ && !matte_ )
    {   
        grid.generate_normals( geometry_left_handed() );
        light_shade( grid );

        vec3* incident_color = grid.vec3_value( "Ci" );
        REYES_ASSERT( incident_color );
        memset( incident_color, 0, sizeof(vec3) * grid.size() );

        vec3* incident_opacity = grid.vec3_value( "Oi" );
        REYES_ASSERT( incident_opacity );
        memset( incident_opacity, 0, sizeof(vec3) * grid.size() );

        // @todo
        //  Adjust the 'I' value in a surface shader if 'P' is not in eye space.
        const vec3* position = grid.vec3_value( "P" );
        REYES_ASSERT( position );
        vec3* incident = grid.vec3_value( "I" );
        REYES_ASSERT( incident );
        memcpy( incident, position, sizeof(vec3) * grid.size() );
        
        vec3* colors = grid.vec3_value( "Cs" );
        vec3* opacities = grid.vec3_value( "Os" );
        for ( unsigned int i = 0; i < grid.size(); ++i )
        {
            colors[i] = color_;
            opacities[i] = opacity_;
        }

        add_coordinate_system( "current", math::identity() );
        add_coordinate_system( "shader", surface_grid_->get_transform() );        
        virtual_machine_->shade( *surface_grid_, *surface_shader_ );                
        remove_coordinate_system( "shader" );
        remove_coordinate_system( "current" );
    }
}

void Attributes::set_surface_shader( Shader* surface_shader, const math::mat4x4& camera_transform )
{
    surface_grid_->clear();
    surface_shader_ = surface_shader;
    if ( surface_shader_ )
    {
        surface_grid_->set_shader( surface_shader );
        surface_grid_->set_transform( camera_transform * transforms_.back() );
        add_coordinate_system( "current", math::identity() );
        add_coordinate_system( "shader", surface_grid_->get_transform() );
        virtual_machine_->initialize( *surface_grid_, *surface_shader_ );
        remove_coordinate_system( "shader" );
        remove_coordinate_system( "current" );
    }
}

Shader* Attributes::surface_shader() const
{
    return surface_shader_;
}

Grid& Attributes::surface_parameters() const
{
    return *surface_grid_;
}

void Attributes::light_shade( Grid& grid )
{
    grid.clear_lights();
    grid.reserve_lights( light_shaders_.size() );
    for ( vector<Grid*>::const_iterator i = active_light_shaders_.begin(); i != active_light_shaders_.end(); ++i )
    {
        Grid* light_grid = *i;
        REYES_ASSERT( light_grid );
        light_grid->clear_lights();
        light_grid->resize( grid.width(), grid.height() );
    
        Shader* shader = light_grid->shader();
        REYES_ASSERT( shader );
        
        vec3* Ps = light_grid->vec3_value( "Ps" );
        REYES_ASSERT( Ps );
        const vec3* P = grid.vec3_value( "P" );
        REYES_ASSERT( P );
        memcpy( Ps, P, sizeof(vec3) * light_grid->size() );
        
        add_coordinate_system( "current", math::identity() );
        add_coordinate_system( "shader", light_grid->get_transform() );
        virtual_machine_->shade( *light_grid, *shader );
        remove_coordinate_system( "shader" );
        remove_coordinate_system( "current" );
        
        const vector<shared_ptr<Light>>& lights = light_grid->lights();
        for ( vector<shared_ptr<Light>>::const_iterator i = lights.begin(); i != lights.end(); ++i )
        {
            const shared_ptr<Light>& light = *i;      
            grid.add_light( light );
        }
    }
}

Grid& Attributes::add_light_shader( Shader* light_shader, const math::mat4x4& camera_transform )
{
    REYES_ASSERT( light_shader );
    
    shared_ptr<Grid> light_parameters( new Grid(light_shader) );
    light_shaders_.push_back( make_pair(light_shader, light_parameters) );
    active_light_shaders_.push_back( light_parameters.get() );

    light_parameters->set_transform( camera_transform * transforms_.back() );
    add_coordinate_system( "current", math::identity() );
    add_coordinate_system( "shader", light_parameters->get_transform() );
    virtual_machine_->initialize( *light_parameters, *light_shader );
    remove_coordinate_system( "shader" );
    remove_coordinate_system( "current" );
    
    return *light_parameters;
}

void Attributes::activate_light_shader( Grid& grid )
{
    vector<Grid*>::iterator i = find_active_light_shader_by_grid( grid );
    if ( i == active_light_shaders_.end() )
    {  
        grid.clear_lights();
        active_light_shaders_.push_back( &grid );
    }
}

void Attributes::deactivate_light_shader( Grid& grid )
{
    vector<Grid*>::iterator i = find_active_light_shader_by_grid( grid );
    if ( i != active_light_shaders_.end() )
    {
        REYES_ASSERT( *i == &grid );
        swap( *i, active_light_shaders_.back() );
        active_light_shaders_.pop_back();
        grid.clear_lights();
    }
}

std::vector<Grid*>::iterator Attributes::find_active_light_shader_by_grid( const Grid& grid )
{
    vector<Grid*>::iterator i = active_light_shaders_.begin();
    while ( i != active_light_shaders_.end() && *i != &grid )
    {
        ++i;
    }
    return i;
}

void Attributes::push_transform()
{
    transforms_.push_back( transforms_.back() );
}

void Attributes::pop_transform()
{
    REYES_ASSERT( transforms_.size() > 1 );
    if ( transforms_.size() > 1 )
    {
        transforms_.pop_back();
    }
}

void Attributes::identity()
{
    transforms_.back() = math::identity();
}

void Attributes::transform( const math::mat4x4& transform )
{
    transforms_.back() = transform;

    if ( determinant(transform) < 0.0f )
    {
        transform_left_handed_ = !transform_left_handed_;
        geometry_left_handed_ = !geometry_left_handed_;
    }
}

void Attributes::concat_transform( const math::mat4x4& transform )
{
    Attributes::transform( transforms_.back() * transform );
}

void Attributes::perspective( float fov, float aspect_ratio, float near_clip_distance, float far_clip_distance )
{
    Attributes::transform( transforms_.back() * renderman_perspective(fov, aspect_ratio, near_clip_distance, far_clip_distance) );
}

const math::mat4x4& Attributes::transform() const
{
    REYES_ASSERT( !transforms_.empty() );
    return transforms_.back();
}

void Attributes::add_coordinate_system( const char* name, const math::mat4x4& transform )
{
    REYES_ASSERT( name );
    REYES_ASSERT( !transforms_.empty() );
    named_transforms_[name] = transform;
}

void Attributes::remove_coordinate_system( const char* name )
{
    REYES_ASSERT( name );
    REYES_ASSERT( named_transforms_.find(name) != named_transforms_.end() );
    named_transforms_.erase( name );
}

math::mat4x4 Attributes::transform_from( const std::string& name ) const
{
    REYES_ASSERT( named_transforms_.find(name) != named_transforms_.end() );
    map<string, math::mat4x4>::const_iterator i = named_transforms_.find( name );
    return i != named_transforms_.end() ? i->second : math::identity();    
}
