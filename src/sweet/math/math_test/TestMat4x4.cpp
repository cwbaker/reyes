//
// TestMat4x4.cpp
// Copyright (c) 2010 Charles Baker.  All rights reserved
//

#include "stdafx.hpp"
#include <UnitTest++/UnitTest++.h>
#include <sweet/math/mat4x4.ipp>
#include <sweet/math/vec3.ipp>

using namespace sweet::math;

SUITE( TestMat4x4 )
{
    TEST( TestAddition )
    {
    }    

    TEST( TestSubtraction )
    {
    }    

    TEST( TestMultiplication )
    {
    }    

    TEST( TestDeterminant )
    {
    }    

    TEST( TestInverse )
    {
    }    

    TEST( TestTranspose )
    {
    }    

    TEST( TestIdentity )
    {
    }    

    TEST( TestAxisAndAngleRotate )
    {
    }    

    TEST( TestQuaternionRotate )
    {
    }    

    TEST( TestTranslate )
    {
    }    

    TEST( TestScale )
    {
    }    

    TEST( TestShear )
    {
    }    
    
    TEST( TestLookAt )
    {
        mat4x4 a = look_at( vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -3.0f), vec3(0.0f, 1.0f, 0.0f) );
        mat4x4 b = translate( 0.0f, 0.0f, 3.0f );
        
        for ( int i = 0; i < 16; ++i )
        {
            CHECK_CLOSE( a.m[i], b.m[i], 0.01f );
        }
    }

    TEST( TestOrthogonal )
    {
    }    

    TEST( TestPerspective )
    {
    }    

    TEST( TestDirect3DPerspective )
    {
    }    
}
