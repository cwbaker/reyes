//
// mat4x4.ipp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_MAT4X4_IPP_INCLUDED
#define SWEET_MATH_MAT4X4_IPP_INCLUDED

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
inline mat4x4::mat4x4()
{
}

/**
// Constructor.
//
// @param m00, m01, ..., m33
//  The values to initialize the elements of the matrix to.
*/
inline mat4x4::mat4x4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 )
{
    m[0] = m00;
    m[1] = m01;
    m[2] = m02;
    m[3] = m03;
    m[4] = m10;
    m[5] = m11;
    m[6] = m12;
    m[7] = m13;
    m[8] = m20;
    m[9] = m21;
    m[10] = m22;
    m[11] = m23;
    m[12] = m30;
    m[13] = m31;
    m[14] = m32;
    m[15] = m33;
}

/**
// Converts this matrix into the array of 16 floats that make up its 
// elements.
*/
inline mat4x4::operator float*()
{
    return m;
}

/**
// Converts this matrix into the array of 16 floats that make up its 
// elements.
*/
inline mat4x4::operator const float*() const
{
    return m;
}

/**
// \e lhs + \e rhs.
*/
inline mat4x4 operator+( const mat4x4& lhs, const mat4x4& rhs )
{
    return mat4x4( 
        lhs.m[0]  + rhs.m[0],  lhs.m[1]  + rhs.m[1],  lhs.m[2]  + rhs.m[2],  lhs.m[3]  + rhs.m[3], 
        lhs.m[4]  + rhs.m[4],  lhs.m[5]  + rhs.m[5],  lhs.m[6]  + rhs.m[6],  lhs.m[7]  + rhs.m[7], 
        lhs.m[8]  + rhs.m[8],  lhs.m[9]  + rhs.m[9],  lhs.m[10] + rhs.m[10], lhs.m[11] + rhs.m[11], 
        lhs.m[12] + rhs.m[12], lhs.m[13] + rhs.m[13], lhs.m[14] + rhs.m[14], lhs.m[15] + rhs.m[15] 
    );
}

/**
// \e lhs - \e rhs.
*/
inline mat4x4 operator-( const mat4x4& lhs, const mat4x4& rhs )
{
    return mat4x4( 
        lhs.m[0]  - rhs.m[0],  lhs.m[1]  - rhs.m[1],  lhs.m[2]  - rhs.m[2],  lhs.m[3]  - rhs.m[3], 
        lhs.m[4]  - rhs.m[4],  lhs.m[5]  - rhs.m[5],  lhs.m[6]  - rhs.m[6],  lhs.m[7]  - rhs.m[7], 
        lhs.m[8]  - rhs.m[8],  lhs.m[9]  - rhs.m[9],  lhs.m[10] - rhs.m[10], lhs.m[11] - rhs.m[11], 
        lhs.m[12] - rhs.m[12], lhs.m[13] - rhs.m[13], lhs.m[14] - rhs.m[14], lhs.m[15] - rhs.m[15] 
    );
}

/**
// \e lhs * \e rhs.
*/
inline mat4x4 operator*( const mat4x4& lhs, const mat4x4& rhs )
{
    return mat4x4(
        lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[8]  + lhs.m[3] * rhs.m[12],
        lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[9]  + lhs.m[3] * rhs.m[13],
        lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[6] + lhs.m[2] * rhs.m[10] + lhs.m[3] * rhs.m[14],
        lhs.m[0] * rhs.m[3] + lhs.m[1] * rhs.m[7] + lhs.m[2] * rhs.m[11] + lhs.m[3] * rhs.m[15],

        lhs.m[4] * rhs.m[0] + lhs.m[5] * rhs.m[4] + lhs.m[6] * rhs.m[8]  + lhs.m[7] * rhs.m[12],
        lhs.m[4] * rhs.m[1] + lhs.m[5] * rhs.m[5] + lhs.m[6] * rhs.m[9]  + lhs.m[7] * rhs.m[13],
        lhs.m[4] * rhs.m[2] + lhs.m[5] * rhs.m[6] + lhs.m[6] * rhs.m[10] + lhs.m[7] * rhs.m[14],
        lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[7] + lhs.m[6] * rhs.m[11] + lhs.m[7] * rhs.m[15],

        lhs.m[8] * rhs.m[0] + lhs.m[9] * rhs.m[4] + lhs.m[10] * rhs.m[8]  + lhs.m[11] * rhs.m[12],
        lhs.m[8] * rhs.m[1] + lhs.m[9] * rhs.m[5] + lhs.m[10] * rhs.m[9]  + lhs.m[11] * rhs.m[13],
        lhs.m[8] * rhs.m[2] + lhs.m[9] * rhs.m[6] + lhs.m[10] * rhs.m[10] + lhs.m[11] * rhs.m[14],
        lhs.m[8] * rhs.m[3] + lhs.m[9] * rhs.m[7] + lhs.m[10] * rhs.m[11] + lhs.m[11] * rhs.m[15],

        lhs.m[12] * rhs.m[0] + lhs.m[13] * rhs.m[4] + lhs.m[14] * rhs.m[8]  + lhs.m[15] * rhs.m[12],
        lhs.m[12] * rhs.m[1] + lhs.m[13] * rhs.m[5] + lhs.m[14] * rhs.m[9]  + lhs.m[15] * rhs.m[13],
        lhs.m[12] * rhs.m[2] + lhs.m[13] * rhs.m[6] + lhs.m[14] * rhs.m[10] + lhs.m[15] * rhs.m[14],
        lhs.m[12] * rhs.m[3] + lhs.m[13] * rhs.m[7] + lhs.m[14] * rhs.m[11] + lhs.m[15] * rhs.m[15]
    );
}

/**
// \e scalar * \e rhs.
*/
inline mat4x4 operator*( float scalar, const mat4x4& rhs )
{
    return mat4x4( 
        scalar * rhs.m[0],  scalar * rhs.m[1],  scalar * rhs.m[2],  scalar * rhs.m[3], 
        scalar * rhs.m[4],  scalar * rhs.m[5],  scalar * rhs.m[6],  scalar * rhs.m[7], 
        scalar * rhs.m[8],  scalar * rhs.m[9],  scalar * rhs.m[10], scalar * rhs.m[11], 
        scalar * rhs.m[12], scalar * rhs.m[13], scalar * rhs.m[14], scalar * rhs.m[15] 
    );
}

/**
// \e lhs * \e scalar.
*/
inline mat4x4 operator*( const mat4x4& lhs, float scalar )
{
    return sweet::math::operator*( scalar, lhs );
}

/**
// \e m * \e v.
*/
inline vec4 operator*( const mat4x4& m, const vec4& v )
{
    return vec4(
        m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
        m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
        m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
        m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w
    );
}

/**
// \e v * \e m.
*/
inline vec4 operator*( const vec4& v, const mat4x4& m )
{
    return vec4(
        v.x * m[0] + v.y * m[4] + v.z * m[8] + v.w * m[12],
        v.x * m[1] + v.y * m[5] + v.z * m[9] + v.w * m[13],
        v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14],
        v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15]
    );
}


/**
// Determinant of \e m.
*/
inline float determinant( const mat4x4& m )
{
    return 
        m.m[3] * m.m[6] * m.m[9]  * m.m[12] - m.m[2] * m.m[7] * m.m[9]  * m.m[12] - 
        m.m[3] * m.m[5] * m.m[10] * m.m[12] + m.m[1] * m.m[7] * m.m[10] * m.m[12] +
        m.m[2] * m.m[5] * m.m[11] * m.m[12] - m.m[1] * m.m[6] * m.m[11] * m.m[12] - 
        m.m[3] * m.m[6] * m.m[8]  * m.m[13] + m.m[2] * m.m[7] * m.m[8]  * m.m[13] +
        m.m[3] * m.m[4] * m.m[10] * m.m[13] - m.m[0] * m.m[7] * m.m[10] * m.m[13] - 
        m.m[2] * m.m[4] * m.m[11] * m.m[13] + m.m[0] * m.m[6] * m.m[11] * m.m[13] +
        m.m[3] * m.m[5] * m.m[8]  * m.m[14] - m.m[1] * m.m[7] * m.m[8]  * m.m[14] - 
        m.m[3] * m.m[4] * m.m[9]  * m.m[14] + m.m[0] * m.m[7] * m.m[9]  * m.m[14] +
        m.m[1] * m.m[4] * m.m[11] * m.m[14] - m.m[0] * m.m[5] * m.m[11] * m.m[14] - 
        m.m[2] * m.m[5] * m.m[8]  * m.m[15] + m.m[1] * m.m[6] * m.m[8]  * m.m[15] +
        m.m[2] * m.m[4] * m.m[9]  * m.m[15] - m.m[0] * m.m[6] * m.m[9]  * m.m[15] - 
        m.m[1] * m.m[4] * m.m[10] * m.m[15] + m.m[0] * m.m[5] * m.m[10] * m.m[15]
    ;
}

/**
// Invert \e m.
*/
inline mat4x4 inverse( const mat4x4& m )
{
    float s = 1.0f / determinant( m );
    return mat4x4(
        s * (m.m[6] * m.m[11] * m.m[13] - m.m[7] * m.m[10] * m.m[13] + m.m[7] * m.m[9] * m.m[14] - m.m[5] * m.m[11] * m.m[14] - m.m[6] * m.m[9] * m.m[15] + m.m[5] * m.m[10] * m.m[15]),
        s * (m.m[3] * m.m[10] * m.m[13] - m.m[2] * m.m[11] * m.m[13] - m.m[3] * m.m[9] * m.m[14] + m.m[1] * m.m[11] * m.m[14] + m.m[2] * m.m[9] * m.m[15] - m.m[1] * m.m[10] * m.m[15]),
        s * (m.m[2] * m.m[7]  * m.m[13] - m.m[3] * m.m[6]  * m.m[13] + m.m[3] * m.m[5] * m.m[14] - m.m[1] * m.m[7]  * m.m[14] - m.m[2] * m.m[5] * m.m[15] + m.m[1] * m.m[6]  * m.m[15]),
        s * (m.m[3] * m.m[6]  * m.m[9]  - m.m[2] * m.m[7]  * m.m[9]  - m.m[3] * m.m[5] * m.m[10] + m.m[1] * m.m[7]  * m.m[10] + m.m[2] * m.m[5] * m.m[11] - m.m[1] * m.m[6]  * m.m[11]),
        s * (m.m[7] * m.m[10] * m.m[12] - m.m[6] * m.m[11] * m.m[12] - m.m[7] * m.m[8] * m.m[14] + m.m[4] * m.m[11] * m.m[14] + m.m[6] * m.m[8] * m.m[15] - m.m[4] * m.m[10] * m.m[15]),
        s * (m.m[2] * m.m[11] * m.m[12] - m.m[3] * m.m[10] * m.m[12] + m.m[3] * m.m[8] * m.m[14] - m.m[0] * m.m[11] * m.m[14] - m.m[2] * m.m[8] * m.m[15] + m.m[0] * m.m[10] * m.m[15]),
        s * (m.m[3] * m.m[6]  * m.m[12] - m.m[2] * m.m[7]  * m.m[12] - m.m[3] * m.m[4] * m.m[14] + m.m[0] * m.m[7]  * m.m[14] + m.m[2] * m.m[4] * m.m[15] - m.m[0] * m.m[6]  * m.m[15]),
        s * (m.m[2] * m.m[7]  * m.m[8]  - m.m[3] * m.m[6]  * m.m[8]  + m.m[3] * m.m[4] * m.m[10] - m.m[0] * m.m[7]  * m.m[10] - m.m[2] * m.m[4] * m.m[11] + m.m[0] * m.m[6]  * m.m[11]),
        s * (m.m[5] * m.m[11] * m.m[12] - m.m[7] * m.m[9]  * m.m[12] + m.m[7] * m.m[8] * m.m[13] - m.m[4] * m.m[11] * m.m[13] - m.m[5] * m.m[8] * m.m[15] + m.m[4] * m.m[9]  * m.m[15]),
        s * (m.m[3] * m.m[9]  * m.m[12] - m.m[1] * m.m[11] * m.m[12] - m.m[3] * m.m[8] * m.m[13] + m.m[0] * m.m[11] * m.m[13] + m.m[1] * m.m[8] * m.m[15] - m.m[0] * m.m[9]  * m.m[15]),
        s * (m.m[1] * m.m[7]  * m.m[12] - m.m[3] * m.m[5]  * m.m[12] + m.m[3] * m.m[4] * m.m[13] - m.m[0] * m.m[7]  * m.m[13] - m.m[1] * m.m[4] * m.m[15] + m.m[0] * m.m[5]  * m.m[15]),
        s * (m.m[3] * m.m[5]  * m.m[8]  - m.m[1] * m.m[7]  * m.m[8]  - m.m[3] * m.m[4] * m.m[9]  + m.m[0] * m.m[7]  * m.m[9]  + m.m[1] * m.m[4] * m.m[11] - m.m[0] * m.m[5]  * m.m[11]),
        s * (m.m[6] * m.m[9]  * m.m[12] - m.m[5] * m.m[10] * m.m[12] - m.m[6] * m.m[8] * m.m[13] + m.m[4] * m.m[10] * m.m[13] + m.m[5] * m.m[8] * m.m[14] - m.m[4] * m.m[9]  * m.m[14]),
        s * (m.m[1] * m.m[10] * m.m[12] - m.m[2] * m.m[9]  * m.m[12] + m.m[2] * m.m[8] * m.m[13] - m.m[0] * m.m[10] * m.m[13] - m.m[1] * m.m[8] * m.m[14] + m.m[0] * m.m[9]  * m.m[14]),
        s * (m.m[2] * m.m[5]  * m.m[12] - m.m[1] * m.m[6]  * m.m[12] - m.m[2] * m.m[4] * m.m[13] + m.m[0] * m.m[6]  * m.m[13] + m.m[1] * m.m[4] * m.m[14] - m.m[0] * m.m[5]  * m.m[14]),
        s * (m.m[1] * m.m[6]  * m.m[8]  - m.m[2] * m.m[5]  * m.m[8]  + m.m[2] * m.m[4] * m.m[9]  - m.m[0] * m.m[6]  * m.m[9]  - m.m[1] * m.m[4] * m.m[10] + m.m[0] * m.m[5]  * m.m[10])
    );
}

/**
// Transpose \e m.
*/
inline mat4x4 transpose( const mat4x4& m )
{
    return mat4x4(
        m[0], m[4], m[8],  m[12],
        m[1], m[5], m[9],  m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    );
}

/**
// Identity.
*/
inline mat4x4 identity()
{
    return mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/**
// Rotation by \e angle radians about \e axis (ccw as you look from +ive to 
// -ive along the axis).
*/
inline mat4x4 rotate( const vec3& axis, float angle )
{
    const float x = axis.x;
    const float y = axis.y;
    const float z = axis.z;

    float s = sinf( angle );
    float c = cosf( angle );
    float t = 1.0f - c;

    return mat4x4(
        t * x * x + c,     
        t * x * y - s * z, 
        t * x * z + s * y, 
        0.0f,
        t * x * y + s * z, 
        t * y * y + c,     
        t * y * z - s * x, 
        0.0f,
        t * x * z - s * y, 
        t * y * z + s * x, 
        t * z * z + c,     
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );
}

/**
// Rotation by \e q.
*/
inline mat4x4 rotate( const quat& q )
{
    SWEET_ASSERT( q.norm() > 0.9999f && q.norm() < 1.0001f );
    return mat4x4(
        1.0f - 2.0f * (q.y * q.y + q.z * q.z), 
        2.0f * (q.x * q.y - q.w * q.z),  
        2.0f * (q.x * q.z + q.w * q.y), 
        0.0f,
	    2.0f * (q.x * q.y + q.w * q.z),
        1.0f - 2.0f * (q.x * q.x + q.z * q.z), 
        2.0f * (q.y * q.z - q.w * q.x), 
        0.0f,
	    2.0f * (q.x * q.z - q.w * q.y),
        2.0f * (q.y * q.z + q.w * q.x), 
        1.0f - 2.0f * (q.x * q.x + q.y * q.y), 
        0.0f,
        0.0f, 
        0.0f, 
        0.0f, 
        1.0f 
    );
}

/**
// Translation by \e x, \e y, and \e z.
//
// The translation is placed in the rightmost column of the matrix.  
// Transformation of vectors occurs correctly when the vectors are 
// multiplied on the right of the matrix.
*/
inline mat4x4 translate( float x, float y, float z )
{
    return mat4x4(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

inline mat4x4 translate( const vec3& tt )
{
    return mat4x4(
        1.0f, 0.0f, 0.0f, tt.x,
        0.0f, 1.0f, 0.0f, tt.y,
        0.0f, 0.0f, 1.0f, tt.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/**
// Scale by \e sx, \e sy, and \e sz.
*/
inline mat4x4 scale( float sx, float sy, float sz )
{
    return mat4x4( 
        sx,   0.0f, 0.0f, 0.0f, 
        0.0f,   sy, 0.0f, 0.0f, 
        0.0f, 0.0f,   sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/**
// Scale by \e ss.
*/
inline mat4x4 scale( const vec3& ss )
{
    return mat4x4( 
        ss.x, 0.0f, 0.0f, 0.0f,
        0.0f, ss.y, 0.0f, 0.0f, 
        0.0f, 0.0f, ss.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
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
inline mat4x4 shear( float hx, float hy, float hz )
{
    return mat4x4( 
        1.0f, 0.0f, hx, 0.0f, 
        0.0f, 1.0f, hy, 0.0f, 
        0.0f, 0.0f, hz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/**
// Look at \e at from \e eye with \e up as up.
*/
inline mat4x4 look_at( const vec3& at, const vec3& eye, const vec3& up )
{
    vec3 z = normalize( at - eye );
    vec3 x = normalize( cross(up, z) );
    vec3 y = cross( z, x );

    return mat4x4(
        x.x,   x.y,  x.z, -dot(x, eye), 
        y.x,   y.y,  y.z, -dot(y, eye),
        z.x,   z.y,  z.z, -dot(z, eye),
        0.0f, 0.0f, 0.0f,         1.0f
    );
}

/**
// Orthogonal projection.
*/
inline mat4x4 orthogonal( float l, float r, float b, float t, float n, float f )
{
    return mat4x4(
        2.0f / (r - l), 0.0f,           0.0f,           -(r + l) / (r - l),
        0.0f,           2.0f / (t - b), 0.0f,           -(t + b) / (t - b),
        0.0f,           0.0f,           2.0f / (f - n), -(f + n) / (f - n),
        0.0f,           0.0f,           0.0f,           1.0f
    );
}

/**
// Perspective projection.
//
// This is a projection of right handed view space into a left handed 
// normalized device coordinate space with z in the range of [-1, 1] 
// ala OpenGL.
*/
inline mat4x4 perspective( float l, float r, float b, float t, float n, float f )
{
    return mat4x4(
        2 * n / (r - l),            0.0f,  (r + l) / (r - l),                 0.0f,
                   0.0f, 2 * n / (t - b),  (t + b) / (t - b),                 0.0f,
                   0.0f,            0.0f, -(f + n) / (f - n), -2 * f * n / (f - n),
                   0.0f,            0.0f,              -1.0f,                 0.0f
    );
}

/**
// Perspective projection.
//
// This is a projection of right handed view space looking down +ive z
// into a left handed normalized device coordinate space with z in the 
// range of [0, 1] ala Direct3D.
*/
inline mat4x4 direct3d_perspective( float fovy, float aspect, float n, float f )
{
    float half_height = n * tanf( fovy / 2.0f );
    float half_width = half_height * aspect;
    return direct3d_perspective( -half_width, half_width, -half_height, half_height, n, f );
}

/**
// Perspective projection.
//
// This is a projection of right handed view space looking down +ive z
// into a left handed normalized device coordinate space with z in the 
// range of [0, 1] ala Direct3D.
*/
inline mat4x4 direct3d_perspective( float l, float r, float b, float t, float n, float f )
{
    return mat4x4(
        -2.0f * n / (r - l),               0.0f, -(r + l) / (r - l),             0.0f,
                       0.0f, 2.0f * n / (t - b), -(t + b) / (t - b),             0.0f,
                       0.0f,               0.0f,        f / (f - n), -f * n / (f - n),
                       0.0f,               0.0f,               1.0f,             0.0f
    );    
}

/**
// Orthographic projection ala RenderMan.
//
// This is a projection of a left handed camera space looking down +ive z into
// a left handed screen space with z in the range [0, 1] ala the screen space
// specified in the RenderMan standard.
*/
inline mat4x4 renderman_orthographic( float l, float r, float b, float t, float n, float f )
{
    return mat4x4(
        2.0f / (r - l), 0.0f,           0.0f,           -(r + l) / (r - l),
        0.0f,           2.0f / (t - b), 0.0f,           -(t + b) / (t - b),
        0.0f,           0.0f,           1.0f / (f - n), -n / (f - n),
        0.0f,           0.0f,           0.0f,           1.0f
    );
}

/**
// Perspective projection ala RenderMan.
//
// This is a projection of left handed camera space looking down +ive z into
// a left handed screen space with z in the range [0, 1] ala the screen space 
// specified in the RenderMan standard.
*/
inline mat4x4 renderman_perspective( float fovx, float aspect, float n, float f )
{
    float half_width = n * tanf( fovx / 2.0f );
    float half_height = half_width / aspect;
    return renderman_perspective( -half_width, half_width, -half_height, half_height, n, f );
}

/**
// Perspective projection ala RenderMan.
//
// This is a projection of left handed camera space looking down +ive z into
// a left handed screen space with z in the range [0, 1] ala the screen space 
// specified in the RenderMan standard.
*/
inline mat4x4 renderman_perspective( float l, float r, float b, float t, float n, float f )
{
    return mat4x4(
         2.0f * n / (r - l),               0.0f,        0.0f,             0.0f,
                       0.0f, 2.0f * n / (t - b),        0.0f,             0.0f,
                       0.0f,               0.0f, f / (f - n), -f * n / (f - n),
                       0.0f,               0.0f,        1.0f,             0.0f
    );    
}

inline vec4 renderman_project( const math::mat4x4& screen_transform, float width, float height, const math::vec3& point )
{
    vec4 xx = screen_transform * vec4( point, 1.0f );
    return vec4(
        (xx.x / (2.0f * xx.w) + 0.5f) * width,
        (1.0f - (xx.y / (2.0f * xx.w) + 0.5f)) * height,
        xx.z / xx.w,
        xx.w
    );
}

}

}

#endif