//
// promote.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "promote.hpp"
#include "Instruction.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace fx
{

void promote_u1v1( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = rhs[0];
    }
}

void promote_u3v3( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = rhs[0];
        result[i + 1] = rhs[1];
        result[i + 2] = rhs[2];
    }
}

void promote_u4v4( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = rhs[0];
        result[i + 1] = rhs[1];
        result[i + 2] = rhs[2];
        result[i + 3] = rhs[3];
    }
}

void promote( int dispatch, float* result, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case INSTRUCTION_U1V1:
            promote_u1v1( result, rhs, length );
            break;

        case INSTRUCTION_U3V3:
            promote_u3v3( result, rhs, length );
            break;

        case INSTRUCTION_U4V4:
            promote_u4v4( result, rhs, length );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

}

}
