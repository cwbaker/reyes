//
// Geometry.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Geometry.hpp"
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>
#include <vector>

using std::min;
using std::max;
using std::list;
using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

Geometry::Geometry( const math::vec2& u_range, const math::vec2& v_range )
: u_range_( u_range ),
  v_range_( v_range )
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

void Geometry::bound( const math::mat4x4& /*transform*/, vec3* /*minimum*/, vec3* /*maximum*/ ) const
{
}

bool Geometry::splittable() const
{
    return false;
}

void Geometry::split( std::list<ptr<Geometry> >* /*primitives*/ ) const
{
    SWEET_ASSERT( false );
}

bool Geometry::diceable() const
{
    return false;
}

void Geometry::dice( const math::mat4x4& /*transform*/, int /*width*/, int /*height*/, Grid* /*grid*/ ) const
{
}
