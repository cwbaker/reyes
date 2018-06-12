//
// convert.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "convert.hpp"
#include "Instruction.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace fx
{

void convert_u1u3( float* result, const float* rhs )
{
    result[0] = rhs[0];
    result[1] = rhs[0];
    result[2] = rhs[0];
}

void convert_u1u4( float* result, const float* rhs )
{
    result[0] = rhs[0];
    result[1] = rhs[0];
    result[2] = rhs[0];
    result[3] = rhs[0];
}

void convert( int dispatch, float* result, const float* rhs )
{
    switch ( dispatch )
    {
        case INSTRUCTION_U1U3:
            convert_u1u3( result, rhs );
            break;

        case INSTRUCTION_U1U4:
            convert_u1u4( result, rhs );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

}

}
