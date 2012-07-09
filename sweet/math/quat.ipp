//
// quat.ipp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_QUAT_IPP_INCLUDED
#define SWEET_MATH_QUAT_IPP_INCLUDED

#include <math.h>
#include "quat.hpp"

namespace sweet
{

namespace math
{

/**
*/
inline quat::quat()
{
}

/**
*/
inline quat::quat( float xx, float yy, float zz, float ww )
: x( xx ),
  y( yy ),
  z( zz ),
  w( ww )
{
}

inline quat::quat( const vec3& v )
: x( v.x ),
  y( v.y ),
  z( v.z ),
  w( 0.0f )
{
}

/**
*/
inline quat::quat( const vec3& axis, float angle )
{
    float s = sinf( angle / 2.0f );
    vec3 aaxis = normalize( axis );
    x = s * aaxis.x;
    y = s * aaxis.y;
    z = s * aaxis.z;
    w = cosf( angle / 2.0f );
}

/**
*/
inline float quat::norm() const
{
    return ::sqrtf( x * x + y * y + z * z + w * w );
}

/**
*/
inline quat operator*( const quat& lhs, const quat& rhs )
{
    return quat(
         lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y + lhs.w * rhs.x,
        -lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x + lhs.w * rhs.y,
         lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w + lhs.w * rhs.z,
        -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z + lhs.w * rhs.w
    );
}

/**
*/
inline quat operator+( const quat& lhs, const quat& rhs )
{
    return quat(
        lhs.x + rhs.x, 
        lhs.y + rhs.y, 
        lhs.z + rhs.z, 
        lhs.w + rhs.w 
    );
}

/**
*/
inline quat operator*( float lhs, const quat& rhs )
{
    return quat(
        lhs * rhs.x,
        lhs * rhs.y,
        lhs * rhs.z,
        lhs * rhs.w
    );
}

/**
*/
inline quat operator*( const quat& lhs, float rhs )
{
    return quat(
        rhs * lhs.x,
        rhs * lhs.y,
        rhs * lhs.z,
        rhs * lhs.w
    );
}

/**
*/
inline float dot( const quat& lhs, const quat& rhs )
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/**
*/
inline quat conjugate( const quat& q )
{
    return quat( -q.x, -q.y, -q.z, q.w );
}

/**
*/
inline quat normalize( const quat& q )
{
    return (1.0f / q.norm()) * q;
}

/**
*/
inline quat slerp( const quat& q0, const quat& q1, float t )
{
    float theta = ::acosf( dot(q0, q1) );
    return ::sinf( theta * (1.0f - t) ) / ::sinf( theta ) * q0 + ::sinf( theta * t ) / ::sinf( theta ) * q1;
}

}

}

#endif
