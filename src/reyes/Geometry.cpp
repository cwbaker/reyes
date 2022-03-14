//
// Geometry.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Geometry.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"
#include <algorithm>
#include <vector>

using std::min;
using std::max;
using std::list;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

Geometry::Geometry( const math::vec2& u_range, const math::vec2& v_range )
: u_range_( u_range )
, v_range_( v_range )
{   
}

Geometry::~Geometry()
{
}

const math::vec2& Geometry::u_range() const
{
    return u_range_;
}

const math::vec2& Geometry::v_range() const
{
    return v_range_;
}

bool Geometry::boundable() const
{
    return false;
}

void Geometry::bound( const math::mat4x4& /*transform*/, vec3* /*minimum*/, vec3* /*maximum*/, Grid* /*grid*/ ) const
{
}

bool Geometry::splittable() const
{
    return false;
}

void Geometry::split( std::list<std::shared_ptr<Geometry>>* /*primitives*/ ) const
{
    REYES_ASSERT( false );
}

bool Geometry::diceable() const
{
    return false;
}

void Geometry::dice( const math::mat4x4& /*transform*/, int /*width*/, int /*height*/, Grid* /*grid*/ ) const
{
}
