//
// vec4.ipp
// Copyright (c) 2008 - 2014 Charles Baker.  All rights reserved.
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

inline vec4::vec4( float xyzw )
: x( xyzw ),
  y( xyzw ),
  z( xyzw ),
  w( xyzw )
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

inline vec4& vec4::operator*=( const vec4& v )
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
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

inline vec4 vec4::operator-() const
{
    return vec4( -x, -y, -z, -w );
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

inline vec4 operator*( const vec4& lhs, const vec4& rhs )
{
    return vec4( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w );
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

inline vec4 srgb( const vec4& rgba )
{
    const float A = 0.055f;
    const float C = 0.0031308f;
    const float GAMMA = 2.4f;
    float red = rgba.x;
    float green = rgba.y;
    float blue = rgba.z;
    float alpha = rgba.w;
    return vec4(
        red < C ? 12.92f * red : (1.0f + A) * powf( red, 1.0f / GAMMA ) - A,
        green < C ? 12.92f * green : (1.0f + A) * powf( green, 1.0f / GAMMA ) - A,
        blue < C ? 12.92f * blue : (1.0f + A) * powf( blue, 1.0f / GAMMA ) - A,
        alpha
    );
}

inline unsigned int rgba( const vec4& color )
{
    int red = int(color.x * 255.0f);
    int green = int(color.y * 255.0f);
    int blue = int(color.z * 255.0f);
    int alpha = int(color.w * 255.0f);
    return alpha << 24 | blue << 16 | green << 8 | red;
}

}

}

#endif
