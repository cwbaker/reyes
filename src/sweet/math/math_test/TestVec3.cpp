//
// TestVec3.cpp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved
//

#include "stdafx.hpp"
#include <unit/UnitTest.h>
#include <sweet/math/vec3.ipp>

using namespace sweet::math;

static const float TOLERANCE = 0.01f;

SUITE( TestVec3 )
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

    TEST( TestScalarMultiplication )
    {
    }    

    TEST( TestLength )
    {
    }
    
    TEST( TestDot )
    {
    }
    
    TEST( TestCross )
    {
    }

    TEST( TestNormalize )
    {
    }    

    TEST( TestLerp )
    {
    }
    
    TEST( rgb_from_hsv )
    {
        struct HsvRgbMapping
        {
            vec3 hsv;
            vec3 rgb;
        };
        
        HsvRgbMapping HSV_RGB_MAPPINGS[] =
        {
            { vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(0.0f, 0.5f, 1.0f), vec3(1.0f, 0.5f, 0.5f) },
            { vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f) },
            { vec3(0.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(0.0f, 0.5f, 0.5f), vec3(0.5f, 0.25f, 0.25f) },
            { vec3(0.0f, 1.0f, 0.5f), vec3(0.5f, 0.0f, 0.0f) },
            { vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },

            { vec3(60.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(60.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 0.5f) },
            { vec3(60.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f) },
            { vec3(60.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(60.0f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.25f) },
            { vec3(60.0f, 1.0f, 0.5f), vec3(0.5f, 0.5f, 0.0f) },
            { vec3(60.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(60.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(60.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },

            { vec3(120.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(120.0f, 0.5f, 1.0f), vec3(0.5f, 1.0f, 0.5f) },
            { vec3(120.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) },
            { vec3(120.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(120.0f, 0.5f, 0.5f), vec3(0.25f, 0.5f, 0.25f) },
            { vec3(120.0f, 1.0f, 0.5f), vec3(0.0f, 0.5f, 0.0f) },
            { vec3(120.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(120.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(120.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },

            { vec3(180.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(180.0f, 0.5f, 1.0f), vec3(0.5f, 1.0f, 1.0f) },
            { vec3(180.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f) },
            { vec3(180.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(180.0f, 0.5f, 0.5f), vec3(0.25f, 0.5f, 0.5f) },
            { vec3(180.0f, 1.0f, 0.5f), vec3(0.0f, 0.5f, 0.5f) },
            { vec3(180.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(180.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(180.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            
            { vec3(240.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(240.0f, 0.5f, 1.0f), vec3(0.5f, 0.5f, 1.0f) },
            { vec3(240.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f) },
            { vec3(240.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(240.0f, 0.5f, 0.5f), vec3(0.25f, 0.25f, 0.5f) },
            { vec3(240.0f, 1.0f, 0.5f), vec3(0.0f, 0.0f, 0.5f) },
            { vec3(240.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(240.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(240.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            
            { vec3(300.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(300.0f, 0.5f, 1.0f), vec3(1.0f, 0.5f, 1.0f) },
            { vec3(300.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f) },
            { vec3(300.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(300.0f, 0.5f, 0.5f), vec3(0.5f, 0.25f, 0.5f) },
            { vec3(300.0f, 1.0f, 0.5f), vec3(0.5f, 0.0f, 0.5f) },
            { vec3(300.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(300.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(300.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            
            { vec3(360.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(360.0f, 0.5f, 1.0f), vec3(1.0f, 0.5f, 0.5f) },
            { vec3(360.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f) },
            { vec3(360.0f, 0.0f, 0.5f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(360.0f, 0.5f, 0.5f), vec3(1.0f, 0.5f, 0.5f) },
            { vec3(360.0f, 1.0f, 0.5f), vec3(1.0f, 0.0f, 0.0f) },
            { vec3(360.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(360.0f, 0.5f, 0.0f), vec3(1.0f, 0.5f, 0.5f) },
            { vec3(360.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },
        };
        
        for ( int i = 0; i < 54; ++i )
        {
            vec3 rgb = rgb_from_hsv( HSV_RGB_MAPPINGS[i].hsv );
            SWEET_ASSERT( fabsf(HSV_RGB_MAPPINGS[i].rgb.x - rgb.x) <= TOLERANCE );
            CHECK_CLOSE( HSV_RGB_MAPPINGS[i].rgb.x, rgb.x, TOLERANCE );
            SWEET_ASSERT( fabsf(HSV_RGB_MAPPINGS[i].rgb.y - rgb.y) <= TOLERANCE );
            CHECK_CLOSE( HSV_RGB_MAPPINGS[i].rgb.y, rgb.y, TOLERANCE );
            SWEET_ASSERT( fabsf(HSV_RGB_MAPPINGS[i].rgb.z - rgb.z) <= TOLERANCE );            
            CHECK_CLOSE( HSV_RGB_MAPPINGS[i].rgb.z, rgb.z, TOLERANCE );
        }
    }
    
    TEST( rgb_from_hsl )
    {
        struct HslRgbMapping
        {
            vec3 hsl;
            vec3 rgb;
        };
        
        HslRgbMapping HSL_RGB_MAPPINGS[] =
        {
            { vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(0.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(0.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(0.0f, 0.5f, 0.5f), vec3(0.75f, 0.25f, 0.25f) },
            { vec3(0.0f, 1.0f, 0.5f), vec3(1.0f, 0.0f, 0.0f) },
            { vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },

            { vec3(60.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(60.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(60.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(60.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(60.0f, 0.5f, 0.5f), vec3(0.75f, 0.75f, 0.25f) },
            { vec3(60.0f, 1.0f, 0.5f), vec3(1.0f, 1.0f, 0.0f) },
            { vec3(60.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(60.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(60.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },

            { vec3(120.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(120.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(120.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(120.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(120.0f, 0.5f, 0.5f), vec3(0.25f, 0.75f, 0.25f) },
            { vec3(120.0f, 1.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f) },
            { vec3(120.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(120.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(120.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },

            { vec3(180.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(180.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(180.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(180.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(180.0f, 0.5f, 0.5f), vec3(0.25f, 0.75f, 0.75f) },
            { vec3(180.0f, 1.0f, 0.5f), vec3(0.0f, 1.0f, 1.0f) },
            { vec3(180.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(180.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(180.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            
            { vec3(240.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(240.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(240.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(240.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(240.0f, 0.5f, 0.5f), vec3(0.25f, 0.25f, 0.75f) },
            { vec3(240.0f, 1.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f) },
            { vec3(240.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(240.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(240.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            
            { vec3(300.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(300.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(300.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(300.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(300.0f, 0.5f, 0.5f), vec3(0.75f, 0.25f, 0.75f) },
            { vec3(300.0f, 1.0f, 0.5f), vec3(1.0f, 0.0f, 1.0f) },
            { vec3(300.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(300.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(300.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            
            { vec3(360.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(360.0f, 0.5f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(360.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f) },
            { vec3(360.0f, 0.0f, 0.5f), vec3(0.5f, 0.5f, 0.5f) },
            { vec3(360.0f, 0.5f, 0.5f), vec3(0.75f, 0.25f, 0.25f) },
            { vec3(360.0f, 1.0f, 0.5f), vec3(1.0f, 0.0f, 0.0f) },
            { vec3(360.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(360.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
            { vec3(360.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f) },
        };
        
        for ( int i = 0; i < 54; ++i )
        {
            vec3 rgb = rgb_from_hsl( HSL_RGB_MAPPINGS[i].hsl );
            SWEET_ASSERT( fabsf(HSL_RGB_MAPPINGS[i].rgb.x - rgb.x) <= TOLERANCE );
            CHECK_CLOSE( HSL_RGB_MAPPINGS[i].rgb.x, rgb.x, TOLERANCE );
            SWEET_ASSERT( fabsf(HSL_RGB_MAPPINGS[i].rgb.y - rgb.y) <= TOLERANCE );
            CHECK_CLOSE( HSL_RGB_MAPPINGS[i].rgb.y, rgb.y, TOLERANCE );
            SWEET_ASSERT( fabsf(HSL_RGB_MAPPINGS[i].rgb.z - rgb.z) <= TOLERANCE );            
            CHECK_CLOSE( HSL_RGB_MAPPINGS[i].rgb.z, rgb.z, TOLERANCE );
        }
    }    
}
