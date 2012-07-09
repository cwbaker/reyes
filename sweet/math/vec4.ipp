//
// vec4.ipp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_VEC4_IPP_INCLUDED
#define SWEET_MATH_VEC4_IPP_INCLUDED

#include "vec4.hpp"
#include "vec3.ipp"
#include <sweet/assert/assert.hpp>
#include <math.h>

namespace sweet
{

namespace math
{

inline vec4::vec4()
{
}

inline vec4::vec4( float xx, float yy, float zz, float ww )
: x( xx ),
  y( yy ),
  z( zz ),
  w( ww )
{
}

inline vec4::vec4( const vec3& v, float ww )
: x( v.x ),
  y( v.y ),
  z( v.z ),
  w( ww )
{
}

inline vec4& vec4::operator+=( const vec4& v )
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

inline vec4& vec4::operator*=( float s )
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

inline vec4 operator+( const vec4& lhs, const vec4& rhs )
{
    return vec4( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w );
}

inline vec4 operator-( const vec4& lhs, const vec4& rhs )
{
    return vec4( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w );
}

inline vec4 operator*( float lhs, const vec4& rhs )
{
    return vec4( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w );
}

inline vec4 operator*( const vec4& lhs, float rhs )
{
    return vec4( rhs * lhs.x, rhs * lhs.y, rhs * lhs.z, rhs * lhs.w );
}

inline vec4 operator/( float lhs, const vec4& rhs )
{
    return (1.0f / lhs) * rhs;
}

inline vec4 operator/( const vec4& lhs, float rhs )
{
    return (1.0f / rhs) * lhs;
}

inline float length( const vec4& v )
{
    return ::sqrtf( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w );
}

inline float dot( const vec4& lhs, const vec4& rhs )
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

inline vec4 cross( const vec4& lhs, const vec4& rhs )
{
    return vec4( 
        lhs.y * rhs.z - lhs.z * rhs.y, 
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x, 
        0.0f 
    );
}

inline vec4 normalize( const vec4& v )
{
    return (1.0f / length(v)) * v;
}

inline vec4 lerp( const vec4& v0, const vec4& v1, float t )
{
    SWEET_ASSERT( t >= 0.0f && t <= 1.0f );
    return vec4(
        v0.x + (v1.x - v0.x) * t,
        v0.y + (v1.y - v0.y) * t,
        v0.z + (v1.z - v0.z) * t,
        v0.w + (v1.w - v0.w) * t
    );
}

}

}

#endif
