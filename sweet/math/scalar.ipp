//
// scalar.ipp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_SCALAR_IPP_INCLUDED
#define SWEET_MATH_SCALAR_IPP_INCLUDED

#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

namespace sweet
{

namespace math
{

/**
*/
inline float clamp( float x, float minimum, float maximum )
{
    return x < minimum ? minimum
         : x > maximum ? maximum
         : x
         ;
}

inline float lerp( float x, float y, float t )
{
    SWEET_ASSERT( t >= 0.0f && t <= 1.0f );
    return x + (y - x) * t;
}

inline float sign( float x )
{
    return x >= 0.0f ? 1.0f : -1.0f;
}

inline float round( float x )
{
    return floorf( x + 0.5f );
}

inline float radians( float degrees )
{
    return degrees * float(M_PI) / 180.0f;
}

inline float degrees( float radians )
{
    return radians * 180.0f / float(M_PI);
}

}

}

#endif