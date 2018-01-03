//
// quat.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_QUAT_HPP_INCLUDED
#define SWEET_MATH_QUAT_HPP_INCLUDED

namespace sweet
{

namespace math
{

struct vec3;

/**
// A quaternion.
*/
struct quat
{
    float x;
    float y;
    float z;
    float w;

    inline quat();
    inline quat( float xx, float yy, float zz, float ww );
    inline quat( const vec3& v );
    inline quat( const vec3& axis, float angle );
    inline float norm() const;    
    template <class Archive> void persist( Archive& archive );
};

inline quat operator*( const quat& lhs, const quat& rhs );
inline quat operator+( const quat& lhs, const quat& rhs );
inline quat operator*( float lhs, const quat& rhs );
inline quat operator*( const quat& lhs, float rhs );
inline float dot( const quat& lhs, const quat& rhs );
inline quat conjugate( const quat& q );
inline quat normalize( const quat& q );
inline quat slerp( const quat& q0, const quat& q1, float t );

}

}

#endif
