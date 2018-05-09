//
// Light.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Light.hpp"
#include "assert.hpp"

using namespace sweet;
using namespace sweet::reyes;

Light::Light( LightType type, std::shared_ptr<Value> color, std::shared_ptr<Value> opacity, const math::vec3& position, const math::vec3& axis, float angle )
: type_( type ),
  color_( color ),
  opacity_( opacity ),
  position_( position ),
  axis_( axis ),
  angle_( angle )
{
    REYES_ASSERT( type_ >= LIGHT_NULL && type_ < LIGHT_COUNT );
}

Light::~Light()
{
}

LightType Light::type() const
{
    return type_;
}

std::shared_ptr<Value> Light::color() const
{
    return color_;
}

std::shared_ptr<Value> Light::opacity() const
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
