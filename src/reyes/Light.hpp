#ifndef REYES_LIGHT_HPP_INCLUDED
#define REYES_LIGHT_HPP_INCLUDED

#include "LightType.hpp"
#include <math/vec3.hpp>
#include <memory>

namespace sweet
{

namespace render
{

class Value;

/**
// The results returned from a light shader for a diced grid of micropolygon
// vertices.
*/
class Light
{
    LightType type_; ///< The type of light (@see LightType).
    std::shared_ptr<Value> color_; ///< The color values returned by the light shader (assigned to "Cl").
    std::shared_ptr<Value> opacity_; ///< The opacity values returned by the light shader (assigned to "Ol").
    math::vec3 position_; ///< The position of the light specified in its illuminate statement.
    math::vec3 axis_; ///< The axis of the light specified in its solar or illuminate statements.
    float angle_; ///< The angle of the cone of light specified in its illuminate statement.

public:
    Light( LightType type, std::shared_ptr<Value> color, std::shared_ptr<Value> opacity, const math::vec3& position, const math::vec3& axis, float angle );
    ~Light();
    
    LightType type() const;
    std::shared_ptr<Value> color() const;
    std::shared_ptr<Value> opacity() const;
    const math::vec3& position() const;
    const math::vec3& axis() const;
    float angle() const;
};

}

}

#endif