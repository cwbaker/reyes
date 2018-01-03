//
// Light.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_LIGHT_HPP_INCLUDED
#define SWEET_RENDER_LIGHT_HPP_INCLUDED

#include "declspec.hpp"
#include "LightType.hpp"
#include <sweet/math/vec3.hpp>
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace render
{

class Value;

/**
// The results returned from a light shader for a diced grid of micropolygon
// vertices.
*/
class SWEET_RENDER_DECLSPEC Light
{
    LightType type_; ///< The type of light (@see LightType).
    ptr<Value> color_; ///< The color values returned by the light shader (assigned to "Cl").
    ptr<Value> opacity_; ///< The opacity values returned by the light shader (assigned to "Ol").
    math::vec3 position_; ///< The position of the light specified in its illuminate statement.
    math::vec3 axis_; ///< The axis of the light specified in its solar or illuminate statements.
    float angle_; ///< The angle of the cone of light specified in its illuminate statement.

public:
    Light( LightType type, ptr<Value> color, ptr<Value> opacity, const math::vec3& position, const math::vec3& axis, float angle );
    ~Light();
    
    LightType type() const;
    ptr<Value> color() const;
    ptr<Value> opacity() const;
    const math::vec3& position() const;
    const math::vec3& axis() const;
    float angle() const;
};

}

}

#endif