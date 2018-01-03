//
// vec2.ipp
// Copyright (c) Charles Baker. All rights reserved.
//

#ifndef SWEET_MATH_VEC2_IPP_INCLUDED
#define SWEET_MATH_VEC2_IPP_INCLUDED

#include "vec2.hpp"
#include "vec3.ipp"
#include "vec4.ipp"
#include <math.h>
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace math
{

inline vec2::vec2()
{
}

inline vec2::vec2( float xy )
: x( xy ),
  y( xy )
{
}

inline vec2::vec2( float xx, float yy )
: x( xx ),
  y( yy )
{
}

inline vec2::vec2( const vec3& v )
: x( v.x ),
  y( v.y )
{
}

inline vec2::vec2( const vec4& v )
: x( v.x ),
  y( v.y )
{
}

inline vec2& vec2::operator+=( const vec2& v )
{
    x += v.x;
    y += v.y;
    return *this;
}

inline vec2 vec2::operator-() const
{
    return vec2( -x, -y );
}

inline vec2 operator+( const vec2& lhs, const vec2& rhs )
{
    return vec2( lhs.x + rhs.x, lhs.y + rhs.y );
}

inline vec2 operator-( const vec2& lhs, const vec2& rhs )
{
    return vec2( lhs.x - rhs.x, lhs.y - rhs.y );
}

inline vec2 operator*( float lhs, const vec2& rhs )
{
    return vec2( lhs * rhs.x, lhs * rhs.y );
}

inline vec2 operator*( const vec2& lhs, float rhs )
{
    return vec2( rhs * lhs.x, rhs * lhs.y );
}

inline vec2 operator/( const vec2& lhs, float rhs )
{
    return (1.0f / rhs) * lhs;
}

inline vec2 operator/( float lhs, const vec2& rhs )
{
    return (1.0f / lhs) * rhs;
}

inline vec2 operator/( const vec2& lhs, const vec2& rhs )
{
    return vec2( lhs.x / rhs.x, lhs.y / rhs.y );
}

inline float length( const vec2& v )
{
    return ::sqrtf( v.x * v.x + v.y * v.y );
}

inline float dot( const vec2& lhs, const vec2& rhs )
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline vec2 normalize( const vec2& v )
{
    return (1.0f / length(v)) * v;
}

inline vec2 lerp( const vec2& v0, const vec2& v1, float t )
{
    SWEET_ASSERT( t >= 0.0f && t <= 1.0f );
    return vec2(
        v0.x + (v1.x - v0.x) * t,
        v0.y + (v1.y - v0.y) * t
    );
}

}

}

#endif
