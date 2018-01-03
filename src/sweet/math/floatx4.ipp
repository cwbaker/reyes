//
// floatx4.ipp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_FLOATX4_IPP_INCLUDED
#define SWEET_MATH_FLOATX4_IPP_INCLUDED

#include <sweet/assert/assert.hpp>
#include <xmmintrin.h>
#include <smmintrin.h>

// I'll try and keep things so that 0 -> w, 1 -> z, 2 -> y, and 3 -> x.  This
// is so that I can use _mm_move_ss(a, b) to set the w element alone from b 
// and keep the xyz elements from a.  That should allow me to deal with vec3 
// values that might get their w element screwed up by operations across all
// elements.
//
// 0 w
// 1 z
// 2 y
// 3 x

namespace sweet
{

namespace math
{

typedef __m128 floatx4;

const int X = 0;
const int Y = 1;
const int Z = 2;
const int W = 3;    

inline floatx4 zero_floatx4()
{
    return _mm_setzero_ps();
}

inline floatx4 one_floatx4()
{
    return _mm_set_ps( 1.0f, 1.0f, 1.0f, 1.0f );
}

inline floatx4 set_floatx4( float a )
{
    return _mm_set1_ps( a );
}

inline float float_from_floatx4( floatx4 a )
{
    return _mm_cvtss_f32( a );
}

inline float x_from_floatx4( floatx4 a )
{
    return _mm_cvtss_f32( a );
}

inline float y_from_floatx4( floatx4 a )
{
    return _mm_cvtss_f32( _mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1)) );
}

inline float z_from_floatx4( floatx4 a )
{
    return _mm_cvtss_f32( _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2)) );
}

inline float w_from_floatx4( floatx4 a )
{
    return _mm_cvtss_f32( _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3)) );
}

inline floatx4 xxxx_swizzle( floatx4 a )
{
    return _mm_shuffle_ps( a, a, _MM_SHUFFLE(0, 0, 0, 0) );
}

inline floatx4 yyyy_swizzle( floatx4 a )
{
    return _mm_shuffle_ps( a, a, _MM_SHUFFLE(1, 1, 1, 1) );
}

inline floatx4 zzzz_swizzle( floatx4 a )
{
    return _mm_shuffle_ps( a, a, _MM_SHUFFLE(2, 2, 2, 2) );
}

inline floatx4 wwww_swizzle( floatx4 a )
{
    return _mm_shuffle_ps( a, a, _MM_SHUFFLE(3, 3, 3, 3) );
}

inline floatx4 load_floatx4( const float* a )
{
    SWEET_ASSERT( a );
    SWEET_ASSERT( (int(a) & 0x0f) == 0 );
    return _mm_load_ps( a );
}

inline floatx4 load_unaligned_floatx4( const float* a )
{
    SWEET_ASSERT( a );
    return _mm_loadu_ps( a );
}

inline void store_floatx4( float* a, floatx4 b )
{
    SWEET_ASSERT( a );
    SWEET_ASSERT( (int(a) & 0x0f) == 0 );
    _mm_store_ps( a, b );
}

inline void store_unaligned_floatx4( float* a, floatx4 b )
{
    SWEET_ASSERT( a );
    _mm_storeu_ps( a, b );
}

inline floatx4 operator+( floatx4 a, floatx4 b )
{
    return _mm_add_ps( a, b );
}

inline floatx4 operator-( floatx4 a, floatx4 b )
{
    return _mm_sub_ps( a, b );
}

inline floatx4 operator*( floatx4 a, floatx4 b )
{
    return _mm_mul_ps( a, b );
}

inline floatx4 operator/( floatx4 a, floatx4 b )
{
    return _mm_div_ps( a, b );
}

inline floatx4 sqrt( floatx4 a )
{
    return _mm_sqrt_ps( a );
}

static const __m128 three = set_floatx4( 3.0f );
static const __m128 half = set_floatx4( 0.5f );
inline floatx4 rsqrt( floatx4 a )
{
    const __m128 rsqrt = _mm_rsqrt_ps( a );
    return half * rsqrt * (three - a * rsqrt * rsqrt);
}

inline floatx4 rcp( floatx4 a )
{
    return _mm_rcp_ps( a );
}

inline floatx4 min( floatx4 a, floatx4 b )
{
    return _mm_min_ps( a, b );
}

inline floatx4 max( floatx4 a, floatx4 b )
{
    return _mm_min_ps( a, b );
}

inline floatx4 operator==( floatx4 a, floatx4 b )
{
    return _mm_cmpeq_ps( a, b );
}

inline floatx4 operator!=( floatx4 a, floatx4 b )
{
    return _mm_cmpneq_ps( a, b );
}

inline floatx4 operator<( floatx4 a, floatx4 b )
{
    return _mm_cmplt_ps( a, b );
}

inline floatx4 operator<=( floatx4 a, floatx4 b )
{
    return _mm_cmple_ps( a, b );
}

inline floatx4 operator>( floatx4 a, floatx4 b )
{
    return _mm_cmpgt_ps( a, b );
}

inline floatx4 operator>=( floatx4 a, floatx4 b )
{
    return _mm_cmpge_ps( a, b );
}

inline floatx4 operator&( floatx4 a, floatx4 b )
{
    return _mm_and_ps( a, b );
}

inline floatx4 operator|( floatx4 a, floatx4 b )
{
    return _mm_or_ps( a, b );
}

inline floatx4 operator^( floatx4 a, floatx4 b )
{
    return _mm_xor_ps( a, b );
}

inline floatx4 andnot( floatx4 a, floatx4 b )
{
    return _mm_andnot_ps( a, b );
}

inline floatx4 dot3( floatx4 a, floatx4 b )
{
    return _mm_dp_ps( a, b, 0x7f );
}

inline floatx4 dot( floatx4 a, floatx4 b )
{
    return _mm_dp_ps( a, b, 0xff );
}

inline floatx4 lerp( floatx4 a0, floatx4 a1, floatx4 t )
{
    return a0 + (a1 - a0) * t;
}

inline floatx4 length( floatx4 a )
{
    const __m128 aa = dot3( a, a );
    return rsqrt( aa ) * aa;
}

inline floatx4 normalize( floatx4 a )
{
    return rsqrt( dot3(a, a) ) * a;
}

inline floatx4 cross( floatx4 a, floatx4 b )
{
    const floatx4 al = _mm_shuffle_ps( a, a, _MM_SHUFFLE(W, X, Z, Y) );
    const floatx4 bl = _mm_shuffle_ps( b, b, _MM_SHUFFLE(W, Y, X, Z) );
    const floatx4 ar = _mm_shuffle_ps( a, a, _MM_SHUFFLE(W, Y, X, Z) );
    const floatx4 br = _mm_shuffle_ps( b, b, _MM_SHUFFLE(W, X, Z, Y) );   
    return al * bl - ar * br;
}

inline floatx4 select( floatx4 mask, floatx4 a, floatx4 b )
{
    return (mask & a) | (andnot(mask, b));
}

inline int movemask( floatx4 mask )
{
    return _mm_movemask_ps( mask );
}

}

}

#endif
