//
// promote.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "promote.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>

namespace reyes
{

void promote_v1u1( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = rhs[0];
    }
}

void promote_v3u3( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = rhs[0];
        result[i * 3 + 1] = rhs[1];
        result[i * 3 + 2] = rhs[2];
    }
}

void promote_v4u4( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = rhs[0];
        result[i * 4 + 1] = rhs[1];
        result[i * 4 + 2] = rhs[2];
        result[i * 4 + 3] = rhs[3];
    }
}

void promote( int dispatch, float* result, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_V1U1:
            promote_v1u1( result, rhs, length );
            break;

        case DISPATCH_V3U3:
            promote_v3u3( result, rhs, length );
            break;

        case DISPATCH_V4U4:
            promote_v4u4( result, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
