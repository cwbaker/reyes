//
// mat3x3.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_MAT3X3_HPP_INCLUDED
#define SWEET_MATH_MAT3X3_HPP_INCLUDED

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
struct mat3x3
{
    float m[9];

    inline mat3x3();
    inline mat3x3( float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 );
    inline mat3x3( const mat4x4& m );
    inline operator float*();
    inline operator const float*() const;
};

inline mat3x3 operator+( const mat3x3& lhs, const mat3x3& rhs );
inline mat3x3 operator-( const mat3x3& lhs, const mat3x3& rhs );
inline mat3x3 operator*( const mat3x3& lhs, const mat3x3& rhs );
inline mat3x3 operator*( float lhs, const mat3x3& rhs );
inline mat3x3 operator*( const mat3x3& lhs, float rhs );
inline vec3 operator*( const mat3x3& m, const vec3& v );
inline float determinant( const mat3x3& matrix );
inline mat3x3 inverse( const mat3x3& matrix );
inline mat3x3 transpose( const mat3x3& matrix );
inline mat3x3 identity3x3();
inline mat3x3 rotate3x3( const vec3& axis, float angle );
inline mat3x3 rotate3x3( const quat& q );
inline mat3x3 scale3x3( float sx, float sy, float sz );
inline mat3x3 shear3x3( float hx, float hy, float hz );
inline mat3x3 skew( const vec3& v );

}

}

#endif
