//
// mat3x3.ipp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_MAT3X3_IPP_INCLUDED
#define SWEET_MATH_MAT3X3_IPP_INCLUDED

#include "mat3x3.hpp"
#include "mat4x4.hpp"
#include "vec3.ipp"
#include "quat.ipp"

namespace sweet
{

namespace math
{

/**
// Constructor.
//
// The elements of the matrix are left uninitialized.
*/
inline mat3x3::mat3x3()
{
}

/**
// Constructor.
//
// @param m00, m01, ..., m33
//  The values to initialize the elements of the matrix to.
*/
inline mat3x3::mat3x3( float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 )
{
    m[0] = m00;
    m[1] = m01;
    m[2] = m02;
    m[3] = m10;
    m[4] = m11;
    m[5] = m12;
    m[6] = m20;
    m[7] = m21;
    m[8] = m22;
}

/**
// Construct a mat3x3 from the upper 3x3 entries of a mat4x4.
//
// @param m
//  The mat4x4 to initialize this mat3x3 from.
*/
inline mat3x3::mat3x3( const mat4x4& mm )
{
    m[0] = mm.m[0];
    m[1] = mm.m[1];
    m[2] = mm.m[2];
    m[3] = mm.m[4];
    m[4] = mm.m[5];
    m[5] = mm.m[6];
    m[6] = mm.m[8];
    m[7] = mm.m[9];
    m[8] = mm.m[10];
}

/**
// Converts this matrix into the array of 16 floats that make up its 
// elements.
*/
inline mat3x3::operator float*()
{
    return m;
}

/**
// Converts this matrix into the array of 16 floats that make up its 
// elements.
*/
inline mat3x3::operator const float*() const
{
    return m;
}

/**
// \e lhs + \e rhs.
*/
inline mat3x3 operator+( const mat3x3& lhs, const mat3x3& rhs )
{
    return mat3x3( 
        lhs.m[0]  + rhs.m[0], lhs.m[1]  + rhs.m[1], lhs.m[2] + rhs.m[2], 
        lhs.m[3] + rhs.m[3], lhs.m[4]  + rhs.m[4], lhs.m[5]  + rhs.m[5], 
        lhs.m[6] + rhs.m[6], lhs.m[7] + rhs.m[7], lhs.m[8]  + rhs.m[8]
    );
}

/**
// \e lhs - \e rhs.
*/
inline mat3x3 operator-( const mat3x3& lhs, const mat3x3& rhs )
{
    return mat3x3( 
        lhs.m[0] - rhs.m[0], lhs.m[1] - rhs.m[1], lhs.m[2] - rhs.m[2], 
        lhs.m[3] - rhs.m[3], lhs.m[4] - rhs.m[4], lhs.m[5] - rhs.m[5], 
        lhs.m[6] - rhs.m[6], lhs.m[7] - rhs.m[7], lhs.m[8] - rhs.m[8]
    );
}

/**
// \e lhs * \e rhs.
*/
inline mat3x3 operator*( const mat3x3& lhs, const mat3x3& rhs )
{
    return mat3x3(
        lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[3] + lhs.m[2] * rhs.m[6],
        lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[7],
        lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[8],

        lhs.m[3] * rhs.m[0] + lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[6],
        lhs.m[3] * rhs.m[1] + lhs.m[4] * rhs.m[4] + lhs.m[5] * rhs.m[7],
        lhs.m[3] * rhs.m[2] + lhs.m[4] * rhs.m[5] + lhs.m[5] * rhs.m[8],

        lhs.m[6] * rhs.m[0] + lhs.m[7] * rhs.m[3] + lhs.m[8] * rhs.m[6],
        lhs.m[6] * rhs.m[1] + lhs.m[7] * rhs.m[4] + lhs.m[8] * rhs.m[7],
        lhs.m[6] * rhs.m[2] + lhs.m[7] * rhs.m[5] + lhs.m[8] * rhs.m[8]
    );
}

/**
// \e scalar * \e rhs.
*/
inline mat3x3 operator*( float scalar, const mat3x3& rhs )
{
    return mat3x3( 
        scalar * rhs.m[0], scalar * rhs.m[1], scalar * rhs.m[2],
        scalar * rhs.m[3], scalar * rhs.m[4], scalar * rhs.m[5], 
        scalar * rhs.m[6], scalar * rhs.m[7], scalar * rhs.m[8]
    );
}

/**
// \e lhs * \e scalar.
*/
inline mat3x3 operator*( const mat3x3& lhs, float scalar )
{
    return operator*( scalar, lhs );
}

/**
// \m * \e v.
*/
inline vec3 operator*( const mat3x3& m, const vec3& v )
{
    return vec3(
        m[0] * v.x + m[1] * v.y + m[2] * v.z,
        m[3] * v.x + m[4] * v.y + m[5] * v.z,
        m[6] * v.x + m[7] * v.y + m[8] * v.z
    );
}

/**
// Determinant of \e m.
*/
inline float determinant( const mat3x3& m )
{
    return 
        m[0] * (m[8] * m[4] - m[7] * m[5]) - 
        m[3] * (m[8] * m[1] - m[7] * m[2]) +
        m[6] * (m[5] * m[1] - m[4] * m[2])
    ;
}

/**
// Invert \e m.
*/
inline mat3x3 inverse( const mat3x3& m )
{
    float s = 1.0f / determinant( m );
    return mat3x3(
        s * (m[8] * m[4] - m[7] * m[5]), 
        s * (m[7] * m[2] - m[8] * m[1]), 
        s * (m[5] * m[1] - m[4] * m[2]),
        s * (m[6] * m[5] - m[8] * m[3]),
        s * (m[8] * m[0] - m[6] * m[2]),
        s * (m[3] * m[2] - m[5] * m[0]),
        s * (m[7] * m[3] - m[6] * m[4]),
        s * (m[6] * m[1] - m[7] * m[0]),
        s * (m[4] * m[0] - m[3] * m[1])
    );
}

/**
// Transpose \e m.
*/
inline mat3x3 transpose( const mat3x3& m )
{
    return mat3x3(
        m[0], m[3], m[6],
        m[1], m[4], m[7],
        m[2], m[5], m[8]
    );
}

/**
// Identity.
*/
inline mat3x3 identity3x3()
{
    return mat3x3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
}

/**
// Rotation by \e angle radians about \e axis (ccw as you look from +ive to 
// -ive along the axis).
*/
inline mat3x3 rotate3x3( const vec3& axis, float angle )
{
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;

    float s = sinf( angle );
    float c = cosf( angle );
    float t = 1.0f - c;

    return mat3x3(
        t * x * x + c,     
        t * x * y - s * z, 
        t * x * z + s * y, 
        t * x * y + s * z, 
        t * y * y + c,     
        t * y * z - s * x, 
        t * x * z - s * y, 
        t * y * z + s * x, 
        t * z * z + c
    );
}

/**
// Rotation by \e q.
*/
inline mat3x3 rotate3x3( const quat& q )
{
    SWEET_ASSERT( q.norm() > 0.9999f && q.norm() < 1.0001f );
    return mat3x3(
        1.0f - 2.0f * (q.y * q.y + q.z * q.z), 
        2.0f * (q.x * q.y - q.w * q.z),  
        2.0f * (q.x * q.z + q.w * q.y), 
	    2.0f * (q.x * q.y + q.w * q.z),
        1.0f - 2.0f * (q.x * q.x + q.z * q.z), 
        2.0f * (q.y * q.z - q.w * q.x), 
	    2.0f * (q.x * q.z - q.w * q.y),
        2.0f * (q.y * q.z + q.w * q.x), 
        1.0f - 2.0f * (q.x * q.x + q.y * q.y)
    );
}

/**
// Scale by \e sx, \e sy, and \e sz.
*/
inline mat3x3 scale3x3( float sx, float sy, float sz )
{
    return mat3x3( 
        sx,   0.0f, 0.0f,
        0.0f,   sy, 0.0f,
        0.0f, 0.0f,   sz
    );
}

/**
// Shear by \e hx, \e hy, and \e hz.
//
// The shear is placed in the third column of the matrix and corresponds
// to shear by the z-element of any vector that is multiplied.  Transformation
// of vectors occurs correctly when vectors are multiplied on the right of the
// matrix.
*/
inline mat3x3 shear3x3( float hx, float hy, float hz )
{
    return mat3x3( 
        1.0f, 0.0f, hx,
        0.0f, 1.0f, hy,
        0.0f, 0.0f, hz
    );
}

/**
// Generate a skew matrix from \e v.
//
// A skew matrix is a cross product in matrix form.  For example
// 'cross(a, b) == skew(a) * b'.
//
// @param v
//  The vector to generate a skew matrix from.
//
// @return
//  The skew matrix for \e v.
*/
inline mat3x3 skew( const vec3& v )
{
    return mat3x3(
        0.0f, -v.z, v.y,
        v.z, 0.0f, -v.x,
        -v.y, v.x, 0.0f
    );
}

}

}

#endif