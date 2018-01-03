//
// vec2.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_VEC2_HPP_INCLUDED
#define SWEET_MATH_VEC2_HPP_INCLUDED

namespace sweet
{

namespace math
{

struct vec3;
struct vec4;

/**
// A two element vector.
*/
struct vec2
{
    float x;
    float y;

    inline vec2();
    inline vec2( float xy );
    inline vec2( float xx, float yy );
    inline vec2( const vec3& v );    
    inline vec2( const vec4& v );
    vec2& operator+=( const vec2& v );    
    vec2 operator-() const;
    template <class Archive> void persist( Archive& archive );
};

inline vec2 operator+( const vec2& lhs, const vec2& rhs );
inline vec2 operator-( const vec2& lhs, const vec2& rhs );
inline vec2 operator*( float lhs, const vec2& rhs );
inline vec2 operator*( const vec2& lhs, float rhs );
inline vec2 operator/( const vec2& lhs, float rhs);
inline vec2 operator/( float rhs, const vec2& lhs );
inline vec2 operator/( const vec2& lhs, const vec2& rhs );
inline float length( const vec2& v );
inline float dot( const vec2& lhs, const vec2& rhs );
inline vec2 normalize( const vec2& v );
inline vec2 lerp( const vec2& v0, const vec2& v1, float t );

}

}

#endif
