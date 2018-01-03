//
// mat4x4.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_MAT4X4_HPP_INCLUDED
#define SWEET_MATH_MAT4X4_HPP_INCLUDED

namespace sweet
{

namespace math
{

struct vec3;
struct vec4;
struct quat;

/**
// A 4x4 matrix of floats.
*/
struct mat4x4
{
    float m[16];

    inline mat4x4();
    inline mat4x4( float mij );
    inline mat4x4( float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33 );
    inline mat4x4( const vec4& c0, const vec4& c1, const vec4& c2, const vec4& c3);
    inline operator float*();
    inline operator const float*() const;
};

inline mat4x4 operator+( const mat4x4& lhs, const mat4x4& rhs );
inline mat4x4 operator-( const mat4x4& lhs, const mat4x4& rhs );
inline mat4x4 operator*( const mat4x4& lhs, const mat4x4& rhs );
inline mat4x4 operator*( float lhs, const mat4x4& rhs );
inline mat4x4 operator*( const mat4x4& lhs, float rhs );
inline vec4 operator*( const mat4x4& m, const vec4& v );
inline vec4 operator*( const vec4& v, const mat4x4& m );
inline float determinant( const mat4x4& matrix );
inline mat4x4 inverse( const mat4x4& matrix );
inline mat4x4 transpose( const mat4x4& matrix );
inline mat4x4 identity();
inline mat4x4 rotate( const vec3& axis, float angle );
inline mat4x4 rotate( const quat& q );
inline mat4x4 translate( float tx, float ty, float tz );
inline mat4x4 translate( const vec3& tt );
inline mat4x4 scale( float sx, float sy, float sz );
inline mat4x4 scale( const vec3& ss );
inline mat4x4 shear( float hx, float hy, float hz );
inline mat4x4 orient( const math::vec3& xx, const math::vec3& yy, const math::vec3& zz);
inline mat4x4 look_at( const vec3& at, const vec3& eye, const vec3& up );
inline mat4x4 orthogonal( float l, float r, float b, float t, float n, float f );
inline mat4x4 perspective( float fovy, float aspect, float n, float f );
inline mat4x4 perspective( float l, float r, float b, float t, float n, float f );
inline mat4x4 direct3d_perspective( float fovy, float aspect, float n, float f );
inline mat4x4 direct3d_perspective( float l, float r, float b, float t, float n, float f );
inline mat4x4 renderman_orthographic( float l, float r, float b, float t, float n, float f );
inline mat4x4 renderman_perspective( float fovx, float aspect, float n, float f );
inline mat4x4 renderman_perspective( float l, float r, float b, float t, float n, float f );
inline vec4 renderman_project( const math::mat4x4& transform, float width, float height, const math::vec4& point );

}

}

#endif
