//
// Light.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Light.hpp"
#include "assert.hpp"
#include <math/vec3.ipp>

using math::vec3;
using math::normalize;
using namespace reyes;

Light::Light( LightType type, math::vec3* color, float* opacity, const math::vec3& position, const math::vec3& axis, float angle )
: type_( type )
, color_( color )
, opacity_( opacity )
, position_( position )
, axis_( axis )
, angle_( angle )
{
    REYES_ASSERT( type_ >= LIGHT_NULL && type_ < LIGHT_COUNT );
    REYES_ASSERT( color_ );
    REYES_ASSERT( opacity_ );
}

Light::~Light()
{
}

LightType Light::type() const
{
    return type_;
}

math::vec3* Light::color() const
{
    return color_;
}

float* Light::opacity() const
{
    return opacity_;
}

const math::vec3& Light::position() const
{
    return position_;
}

const math::vec3& Light::axis() const
{
    return axis_;
}

float Light::angle() const
{
    return angle_;
}

/**
// Calculate the light direction from the light at \e light_position (the
// explicit position expression of an illuminate statement) to the surface 
// at \e position (the implicit "Ps" value passed to a light shader).
//
// This is used to calculate "L" in a light shader's illuminate statement.
*/
void Light::light_to_surface_vector( const math::vec3* position, math::vec3* light_to_surface, int length ) const
{
    REYES_ASSERT( position );
    REYES_ASSERT( light_direction );
    REYES_ASSERT( length >= 0 );
    vec3 light_position = Light::position();
    for ( int i = 0; i < length; ++i )
    {
        light_to_surface[i] = position[i] - light_position;
    }
}

/**
// Calculate the light direction from the surface (the implicit "P" value in
// a surface shader) \e position to \e light.
//
// This is used to calculate "L" in a surface shader's illuminance statement 
// from the surface position and the currently active light.
*/
void Light::surface_to_light_vector( const math::vec3* position, math::vec3* surface_to_light, int length ) const
{
    REYES_ASSERT( position );
    REYES_ASSERT( surface_to_light );
    REYES_ASSERT( length >= 0 );
    vec3 light_position = Light::position();
    for ( int i = 0; i < length; ++i )
    {
        surface_to_light[i] = light_position - position[i];
    }
}

/**
// Calculate a mask based on the axis and angle passed to an illuminance 
// statement and the light from \e light.
*/
void Light::illuminance_axis_angle( const math::vec3& axis, float angle, const math::vec3* position, int* mask, int length ) const
{
    REYES_ASSERT( position );
    REYES_ASSERT( mask );
    REYES_ASSERT( length >= 0 );
    
    const float angle_cosine = cosf( angle );
    switch ( type() )
    {
        case LIGHT_SOLAR_AXIS:
        case LIGHT_SOLAR_AXIS_ANGLE:
        {
            const vec3 light_direction = normalize( -Light::position() );
            for ( int i = 0; i < length; ++i )
            {
                mask[i] = dot( axis, light_direction ) >= angle_cosine;
            }
            break;
        }
            
        case LIGHT_ILLUMINATE:
        case LIGHT_ILLUMINATE_AXIS_ANGLE:
        {
            const vec3 light_position = Light::position();
            for ( int i = 0; i < length; ++i )
            {
                mask[i] = dot( axis, normalize(light_position - position[i]) ) >= angle_cosine;
            }
            break;
        }
    }        
}
