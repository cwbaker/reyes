//
// vec3.ipp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_VEC3_IPP_INCLUDED
#define SWEET_MATH_VEC3_IPP_INCLUDED

#include "vec3.hpp"
#include "vec2.ipp"
#include "vec4.ipp"
#include <sweet/assert/assert.hpp>
#include <float.h>
#include <math.h>
#include <algorithm>

namespace sweet
{

namespace math
{

inline vec3::vec3()
{
}

inline vec3::vec3( float xx, float yy, float zz )
: x( xx ),
  y( yy ),
  z( zz )
{
}

inline vec3::vec3( float xyz )
: x( xyz ),
  y( xyz ),
  z( xyz )
{
}

inline vec3::vec3( const vec4& v )
: x( v.x ),
  y( v.y ),
  z( v.z )
{
}

inline vec3& vec3::operator+=( const vec3& v )
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

inline vec3& vec3::operator*=( const vec3& v )
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

inline vec3 vec3::operator-() const
{
    return vec3( -x, -y, -z );
}

inline bool operator==( const vec3& lhs, const vec3& rhs )
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator!=( const vec3& lhs, const vec3& rhs )
{
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

inline vec3 operator+( const vec3& lhs, const vec3& rhs )
{
    return vec3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

inline vec3 operator-( const vec3& lhs, const vec3& rhs )
{
    return vec3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

inline vec3 operator*( float lhs, const vec3& rhs )
{
    return vec3( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z );
}

inline vec3 operator*( const vec3& lhs, float rhs )
{
    return sweet::math::operator*( rhs, lhs );
}

inline vec3 operator*( const vec3& lhs, const vec3& rhs )
{
    return vec3(
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z
    );
}

inline vec3 operator/( float lhs, const vec3& rhs )
{
    return (1.0f / lhs) * rhs;
}

inline vec3 operator/( const vec3& lhs, float rhs )
{
    return (1.0f / rhs) * lhs;
}

/**
// Convert a color in HSV space to RGB space (H is [0, 360]).
//
// @param hsv
//  The HSV color to convert to RGB.  The hue value is expected to 
//  be in [0, 360].
//
// @return
//  The RGB color.
*/
inline vec3 rgb_from_hsv( const vec3& hsv )
{
    const float c = hsv.y * hsv.z;
    const float h = hsv.x / 60.0f;
    const float x = c * (1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
    const float m = hsv.z - c;
    
    vec3 rgb( m, m, m );
    switch ( int(floorf(h)) )
    {
        case 0:
            rgb += vec3( c, x, 0.0f );
            break;
            
        case 1:
            rgb += vec3( x, c, 0.0f  );
            break;
            
        case 2:
            rgb += vec3( 0.0f, c, x );
            break;
            
        case 3:
            rgb += vec3( 0.0f, x, c );
            break;
            
        case 4:
            rgb += vec3( x, 0.0f, c );
            break;
            
        case 5:
            rgb += vec3( c, 0.0f, x );
            break;
            
        default:
            SWEET_ASSERT( false );
            break;
    }
    return rgb;
}

/**
// Convert a color in HSL space to RGB space (H is [0, 360]).
//
// @param hsl
//  The HSL color to convert to RGB.  The hue value is expected to 
//  be in [0, 360].
//
// @return
//  The RGB color.
*/
inline vec3 rgb_from_hsl( const vec3& hsl )
{
    const float c = (1.0f - fabsf(2.0f * hsl.z - 1.0f)) * hsl.y;
    const float h = hsl.x / 60.0f;
    const float x = c * (1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
    const float m = hsl.z - 0.5f * c;
    
    vec3 rgb( m, m, m );
    switch ( int(floorf(h)) )
    {
        case 0:
            rgb += vec3( c, x, 0.0f );
            break;
            
        case 1:
            rgb += vec3( x, c, 0.0f  );
            break;
            
        case 2:
            rgb += vec3( 0.0f, c, x );
            break;
            
        case 3:
            rgb += vec3( 0.0f, x, c );
            break;
            
        case 4:
            rgb += vec3( x, 0.0f, c );
            break;
            
        case 5:
            rgb += vec3( c, 0.0f, x );
            break;
            
        default:
            SWEET_ASSERT( false );
            break;
    }
    return rgb;
}

inline float length( const vec3& v )
{
    return ::sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
}

inline float dot( const vec3& lhs, const vec3& rhs )
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline vec3 cross( const vec3& lhs, const vec3& rhs )
{
    return vec3( 
        lhs.y * rhs.z - lhs.z * rhs.y, 
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

inline vec3 normalize( const vec3& v )
{
    return (1.0f / length(v)) * v;
}

inline vec3 lerp( const vec3& v0, const vec3& v1, float t )
{
    SWEET_ASSERT( t >= 0.0f && t <= 1.0f );
    return vec3(
        v0.x + (v1.x - v0.x) * t,
        v0.y + (v1.y - v0.y) * t,
        v0.z + (v1.z - v0.z) * t
    );
}

inline vec3 round( const vec3& v0 )
{
    return vec3( floorf(v0.x + 0.5f), floorf(v0.y + 0.5f), floorf(v0.z + 0.5f) );
}

inline vec3 floor( const vec3& v0 )
{
    return vec3( floorf(v0.x), floorf(v0.y), floorf(v0.z) );
}

inline vec3 ceil( const vec3& v0 )
{
    return vec3( ceilf(v0.x), ceilf(v0.y), ceilf(v0.z) );
}

inline float intersect_line_plane( const vec3& p, const vec3& n, const vec3& o, const vec3& d )
{
    return dot( d, n ) != 0.0f ? (dot(p, n) - dot(o, n)) / dot(d, n) : FLT_MAX;
}

/**
// Calculate the intersection between a line from origin \e o in direction 
// \e d as a parameter of the line.
//
// @param p
//  The centre of the sphere.
//
// @param r
//  The radius of the sphere.
//
// @param o
//  The origin of the line to intersect with the sphere.
//
// @param d
//  The direction of the line to intersect with the sphere.
//
// @return
//  The parameter of the line that the intersection occurs at or FLT_MAX if 
//  the line and the sphere do not intersect.
*/
inline float intersect_line_sphere( const vec3& p, float r, const vec3& o, const vec3& d )
{
    using std::min;
    vec3 c = p - o;
    float discriminant = dot(d, c) * dot(d, c) - dot(c, c) + r * r;
    return discriminant >= 0.0f ? min( dot(d, c) + sqrtf(discriminant), dot(d, c) + sqrtf(discriminant) ) : FLT_MAX;
}

/**
// Project a line onto a plane (to become a point).
//
// @param p
//  A point on the plane (the origin of the 2d coordinate system that the
//  line is projected into).
//
// @param i
//  The x-axis of the 2d coordinate system in 3d space.
//
// @param j
//  The y-axis of the 2d coordinate system in 3d space.
//
// @param k
//  The normal to the plane in 3d space.
//
// @param o
//  The origin of the line to project into the plane.
//
// @param d
//  The direction of the line to project into the plane.
//
// @return
//  The coordinates of the point that the line intersects the plane or
//  (\e FLT_MAX, \e FLT_MAX) if the line doesn't intersect the plane (when
//  the plane is parallel to the line).
*/
inline vec2 project_line_plane( const math::vec3& p, const math::vec3& i, const math::vec3& j, const math::vec3& k, const math::vec3& o, const math::vec3& d )
{
    float t = intersect_line_plane( p, k, o, d );
    if ( t > 0.0f && t < FLT_MAX )
    {
        vec3 pp = o + t * d - p;
        return vec2( dot(pp, i), dot(pp, j) );
    }
    else
    {
        return vec2( FLT_MAX, FLT_MAX );
    }
}

/**
// Are \e a and \e b parallel?
//
// @param a
//  The first vector to compare for being parallel.
//
// @param b
//  The second vector to compare for being parallel.
//
// @return
//  True if a and b are parallel otherwise false.
*/
inline bool parallel( const math::vec3& a, const math::vec3& b )
{
    const float PARALLEL_THRESHOLD = 0.99f;
    return fabsf( dot(a, b) ) >= PARALLEL_THRESHOLD;
}

}

}

#endif
