//
// vec3.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_VEC3_HPP_INCLUDED
#define SWEET_MATH_VEC3_HPP_INCLUDED

#include "vec2.hpp"

namespace sweet
{

namespace math
{

struct vec4;
struct mat3x3;

/**
// A three element vector.
*/
struct vec3
{
    float x;
    float y;
    float z;

    inline vec3();
    inline vec3( float xx, float yy, float zz );
    explicit inline vec3( float xyz );
    explicit inline vec3( const vec4& v );    
    vec3& operator+=( const vec3& v );
    vec3& operator*=( const vec3& v );
    vec3 operator-() const;
    template <class Archive> void persist( Archive& archive );
};

inline bool operator==( const vec3& lhs, const vec3& rhs );
inline bool operator!=( const vec3& lhs, const vec3& rhs );
inline vec3 operator+( const vec3& lhs, const vec3& rhs );
inline vec3 operator-( const vec3& lhs, const vec3& rhs );
inline vec3 operator*( float lhs, const vec3& rhs );
inline vec3 operator*( const vec3& lhs, float rhs );
inline vec3 operator*( const vec3& lhs, const vec3& rhs );
inline vec3 operator/( float scalar, const vec3& rhs );
inline vec3 operator/( const vec3& lhs, float scalar );
inline vec3 rgb_from_hsv( const vec3& hsv );
inline vec3 rgb_from_hsl( const vec3& hsl );
inline float length( const vec3& v );
inline float dot( const vec3& lhs, const vec3& rhs );
inline vec3 cross( const vec3& lhs, const vec3& rhs );
inline vec3 normalize( const vec3& v );
inline vec3 lerp( const vec3& v0, const vec3& v1, float t );
inline vec3 round( const vec3& v0 );
inline vec3 floor( const vec3& v0 );
inline vec3 ceil( const vec3& v0 );
inline float intersect_line_plane( const vec3& p, const vec3& n, const vec3& o, const vec3& d );
inline float intersect_line_sphere( const vec3& p, float r, const vec3& o, const vec3& d );
inline vec2 project_line_plane( const math::vec3& p, const math::vec3& i, const math::vec3& j, const math::vec3& k, const math::vec3& o, const math::vec3& d );
inline bool parallel( const math::vec3& a, const math::vec3& b );

}

}

#endif
