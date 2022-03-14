#pragma once

#include "LightType.hpp"
#include <math/vec3.hpp>
#include <memory>

namespace reyes
{

/**
// The results returned from a light shader for a diced grid of micropolygon
// vertices.
*/
class Light
{
    LightType type_; ///< The type of light (@see LightType).
    math::vec3* color_; ///< The color values returned by the light shader (assigned to "Cl").
    float* opacity_; ///< The opacity values returned by the light shader (assigned to "Ol").
    math::vec3 position_; ///< The position of the light specified in its illuminate statement.
    math::vec3 axis_; ///< The axis of the light specified in its solar or illuminate statements.
    float angle_; ///< The angle of the cone of light specified in its illuminate statement.

public:
    Light( LightType type, math::vec3* color, float* opacity, const math::vec3& position, const math::vec3& axis, float angle );
    ~Light();    
    LightType type() const;
    math::vec3* color() const;
    float* opacity() const;
    const math::vec3& position() const;
    const math::vec3& axis() const;
    float angle() const;
    void light_to_surface_vector( const math::vec3* position, math::vec3* light_to_surface, int length ) const;
    void surface_to_light_vector( const math::vec3* position, math::vec3* surface_to_light, int length ) const;
    void illuminance_axis_angle( const math::vec3& axis, float angle, const math::vec3* position, int* mask, int length ) const;
};

}
