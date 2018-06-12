//
// lerp.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "lerp.hpp"
#include <sweet/math/scalar.ipp>

using namespace sweet::math;

namespace sweet
{
    
namespace fx
{
    
void lerp_v4_u4u4v1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 4 + 0] = lerp( x0[0], x1[0], t[i] );
        results[i * 4 + 1] = lerp( x0[1], x1[1], t[i] );
        results[i * 4 + 2] = lerp( x0[2], x1[2], t[i] );
        results[i * 4 + 3] = lerp( x0[3], x1[3], t[i] );
    }
}

void lerp_v3_v3v3v1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[i * 3 + 0], x1[i * 3 + 0], t[i] );
        results[i * 3 + 1] = lerp( x0[i * 3 + 1], x1[i * 3 + 1], t[i] );
        results[i * 3 + 2] = lerp( x0[i * 3 + 2], x1[i * 3 + 2], t[i] );
    }
}

void lerp_v3_v3u3v1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[i * 3 + 0], x1[0], t[i] );
        results[i * 3 + 1] = lerp( x0[i * 3 + 1], x1[1], t[i] );
        results[i * 3 + 2] = lerp( x0[i * 3 + 2], x1[2], t[i] );
    }
}

void lerp_v3_u3v3v1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[0], x1[i * 3 + 0], t[i] );
        results[i * 3 + 1] = lerp( x0[1], x1[i * 3 + 1], t[i] );
        results[i * 3 + 2] = lerp( x0[2], x1[i * 3 + 2], t[i] );
    }
}

void lerp_v3_u3u3v1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[0], x1[0], t[i] );
        results[i * 3 + 1] = lerp( x0[1], x1[1], t[i] );
        results[i * 3 + 2] = lerp( x0[2], x1[2], t[i] );
    }
}

void lerp_v3_v3u3u1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[i * 3 + 0], x1[0], t[0] );
        results[i * 3 + 1] = lerp( x0[i * 3 + 1], x1[1], t[0] );
        results[i * 3 + 2] = lerp( x0[i * 3 + 2], x1[2], t[0] );
    }
}

void lerp_v3_u3v3u1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[0], x1[i * 3 + 0], t[0] );
        results[i * 3 + 1] = lerp( x0[1], x1[i * 3 + 1], t[0] );
        results[i * 3 + 2] = lerp( x0[2], x1[i * 3 + 2], t[0] );
    }
}

void lerp_v3_u3u3u1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = lerp( x0[0], x1[0], t[0] );
        results[i * 3 + 1] = lerp( x0[1], x1[1], t[0] );
        results[i * 3 + 2] = lerp( x0[2], x1[2], t[0] );
    }
}

void lerp_v1_u1u1v1( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    float* results = arguments[0];
    const float* x0 = arguments[1];
    const float* x1 = arguments[2];
    const float* t = arguments[3];
    for ( int i = 0; i < length; ++i )
    {
        results[i] = lerp( x0[0], x1[0], t[i] );
    }
}

}

}
