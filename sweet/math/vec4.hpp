//
// vec4.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_VEC4_HPP_INCLUDED
#define SWEET_MATH_VEC4_HPP_INCLUDED

namespace sweet
{

namespace math
{

struct vec3;

/**
// A four element vector.
*/
struct vec4
{
    float x;
    float y;
    float z;
    float w;

    inline vec4();
    inline vec4( float xx, float yy, float zz, float ww );
    inline vec4( const vec3& v, float ww );    
    vec4& operator+=( const vec4& v );
    vec4& operator*=( float s );
    template <class Archive> void persist( Archive& archive );
};

inline vec4 operator+( const vec4& lhs, const vec4& rhs );
inline vec4 operator-( const vec4& lhs, const vec4& rhs );
inline vec4 operator*( float lhs, const vec4& rhs );
inline vec4 operator*( const vec4& lhs, float rhs );
inline vec4 operator/( float lhs, const vec4& rhs );
inline vec4 operator/( const vec4& lhs, float rhs );
inline float length( const vec4& v );
inline float dot( const vec4& lhs, const vec4& rhs );
inline vec4 cross( const vec4& lhs, const vec4& rhs );
inline vec4 normalize( const vec4& v );
inline vec4 lerp( const vec4& v0, const vec4& v1, float t );

}

}

#endif
