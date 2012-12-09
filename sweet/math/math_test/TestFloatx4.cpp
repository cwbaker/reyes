//
// TestFloatx4.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved
//


#include "stdafx.hpp"

#if defined(BUILD_PLATFORM_MSVC)
#include <sweet/unit/UnitTest.h>
#include <sweet/math/vec3.ipp>
#include <sweet/math/floatx4.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sweet::math;

SUITE( Floatx4 )
{
    static const float TOLERANCE = 0.001f;

    TEST( zero_returns_zero )
    {
        float r[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        floatx4 zero = zero_floatx4();
        store_unaligned_floatx4( r, zero );
        CHECK( r[0] == 0.0f );
        CHECK( r[1] == 0.0f );
        CHECK( r[2] == 0.0f );
        CHECK( r[3] == 0.0f );
    }   
    
    TEST( load_aligned_values )
    {
        __declspec(align(16)) float a[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        float r[4] = { 0 };
        store_unaligned_floatx4( r, load_floatx4(a) );
        CHECK( r[0] = 1.0f );
        CHECK( r[1] = 2.0f );
        CHECK( r[2] = 3.0f );
        CHECK( r[3] = 4.0f );
    }
    
    TEST( load_unaligned_values )
    {
        float a[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        float r[4] = { 0 };
        store_unaligned_floatx4( r, load_unaligned_floatx4(a) );
        CHECK( r[0] = 1.0f );
        CHECK( r[1] = 2.0f );
        CHECK( r[2] = 3.0f );
        CHECK( r[3] = 4.0f );
    }
    
    TEST( store_aligned_values )
    {
        __declspec(align(16)) float r[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        store_floatx4( r, zero_floatx4() );
        CHECK( r[0] == 0.0f );
        CHECK( r[1] == 0.0f );
        CHECK( r[2] == 0.0f );
        CHECK( r[3] == 0.0f );
    }
    
    TEST( store_unaligned_values )
    {
        float r[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        store_unaligned_floatx4( r, zero_floatx4() );
        CHECK( r[0] == 0.0f );
        CHECK( r[1] == 0.0f );
        CHECK( r[2] == 0.0f );
        CHECK( r[3] == 0.0f );
    }
    
    TEST( add )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f };
        store_unaligned_floatx4( r, a + a );        
        CHECK_CLOSE( r[0], 2.0f, TOLERANCE );
        CHECK_CLOSE( r[1], 4.0f, TOLERANCE );
        CHECK_CLOSE( r[2], 6.0f, TOLERANCE );
        CHECK_CLOSE( r[3], 8.0f, TOLERANCE );
    }
    
    TEST( subtract )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        store_unaligned_floatx4( r, a - a );        
        CHECK_CLOSE( r[0], 0.0f, TOLERANCE );
        CHECK_CLOSE( r[1], 0.0f, TOLERANCE );
        CHECK_CLOSE( r[2], 0.0f, TOLERANCE );
        CHECK_CLOSE( r[3], 0.0f, TOLERANCE );
    }        
    
    TEST( multiply )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, a * a );        
        CHECK_CLOSE( r[0], 1.0f, TOLERANCE );
        CHECK_CLOSE( r[1], 4.0f, TOLERANCE );
        CHECK_CLOSE( r[2], 9.0f, TOLERANCE );
        CHECK_CLOSE( r[3], 16.0f, TOLERANCE );
    }

    TEST( divide )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, a / a );        
        CHECK_CLOSE( r[0], 1.0f, TOLERANCE );
        CHECK_CLOSE( r[1], 1.0f, TOLERANCE );
        CHECK_CLOSE( r[2], 1.0f, TOLERANCE );
        CHECK_CLOSE( r[3], 1.0f, TOLERANCE );
    }
    
    TEST( sqrt )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, sqrt(a) );
        CHECK_CLOSE( r[0], sqrtf(1.0f), TOLERANCE );
        CHECK_CLOSE( r[1], sqrtf(2.0f), TOLERANCE );
        CHECK_CLOSE( r[2], sqrtf(3.0f), TOLERANCE );
        CHECK_CLOSE( r[3], sqrtf(4.0f), TOLERANCE );
    }    

    TEST( rsqrt )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, rsqrt(a) );
        CHECK_CLOSE( r[0], 1.0f / sqrtf(1.0f), TOLERANCE );
        CHECK_CLOSE( r[1], 1.0f / sqrtf(2.0f), TOLERANCE );
        CHECK_CLOSE( r[2], 1.0f / sqrtf(3.0f), TOLERANCE );
        CHECK_CLOSE( r[3], 1.0f / sqrtf(4.0f), TOLERANCE );
    }    

    TEST( rcp )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, rcp(a) );
        CHECK_CLOSE( r[0], 1.0f / 1.0f, TOLERANCE );
        CHECK_CLOSE( r[1], 1.0f / 2.0f, TOLERANCE );
        CHECK_CLOSE( r[2], 1.0f / 3.0f, TOLERANCE );
        CHECK_CLOSE( r[3], 1.0f / 4.0f, TOLERANCE );
    }
    
    TEST( dot )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        floatx4 b = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, dot(a, b) );
        CHECK_CLOSE( r[0], 30.0f, TOLERANCE );
        CHECK_CLOSE( r[1], 30.0f, TOLERANCE );
        CHECK_CLOSE( r[2], 30.0f, TOLERANCE );
        CHECK_CLOSE( r[3], 30.0f, TOLERANCE );
    }
    
    TEST( x_from_floatx4 )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        CHECK_CLOSE( x_from_floatx4(a), 1.0f, TOLERANCE );
    }
    
    TEST( y_from_floatx4 )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        CHECK_CLOSE( y_from_floatx4(a), 2.0f, TOLERANCE );
    }
    
    TEST( z_from_floatx4 )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        CHECK_CLOSE( z_from_floatx4(a), 3.0f, TOLERANCE );
    }
    
    TEST( w_from_floatx4 )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        CHECK_CLOSE( w_from_floatx4(a), 4.0f, TOLERANCE );
    }
    
    TEST( length )
    {
        __declspec(align(16)) float aa[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, length(a) );
        CHECK_CLOSE( length(vec3(1.0f, 2.0f, 3.0f)), r[0], TOLERANCE );
        CHECK_CLOSE( length(vec3(1.0f, 2.0f, 3.0f)), r[1], TOLERANCE );
        CHECK_CLOSE( length(vec3(1.0f, 2.0f, 3.0f)), r[2], TOLERANCE );
    }
    
    TEST( normalize )
    {
        __declspec(align(16)) float aa[4] = { 0.0f, 0.0f, 2.0f, 0.0f };
        floatx4 a = load_floatx4( aa );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, normalize(a) );
        CHECK_CLOSE( 0.0f, r[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, r[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, r[2], TOLERANCE );
    }
    
    TEST( cross )
    {
        __declspec(align(16)) float xx[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
        __declspec(align(16)) float yy[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
        floatx4 x = load_floatx4( xx );
        floatx4 y = load_floatx4( yy );
        float r[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        store_unaligned_floatx4( r, cross(x, y) );
        CHECK_CLOSE( 0.0f, r[0], TOLERANCE );
        CHECK_CLOSE( 0.0f, r[1], TOLERANCE );
        CHECK_CLOSE( 1.0f, r[2], TOLERANCE );
        
        __declspec(align(16)) float uu[4] = { 0.03f, 0.24f, 0.0f, 0.0f };
        __declspec(align(16)) float vv[4] = { 0.0015f, 0.00002f, 0.124f, 0.0f };
        floatx4 u = load_floatx4( uu );
        floatx4 v = load_floatx4( vv );
        vec3 zz = cross( vec3(uu[0], uu[1], uu[2]), vec3(vv[0], vv[1], vv[2]) );
        store_unaligned_floatx4( r, cross(u, v) );
        CHECK_CLOSE( 0.029f, r[0], TOLERANCE );
        CHECK_CLOSE( -0.004f, r[1], TOLERANCE );
        CHECK_CLOSE( -0.001f, r[2], TOLERANCE );
    }
}

#endif
