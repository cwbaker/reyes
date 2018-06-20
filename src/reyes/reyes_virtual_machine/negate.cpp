//
// negate.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "negate.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{

void negate_u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = -rhs[0];
}

void negate_u2( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = -rhs[0 + 0];
    result[0 + 1] = -rhs[0 + 1];
}

void negate_u3( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = -rhs[0 + 0];
    result[0 + 1] = -rhs[0 + 1];
    result[0 + 2] = -rhs[0 + 2];
}

void negate_u4( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = -rhs[0 + 0];
    result[0 + 1] = -rhs[0 + 1];
    result[0 + 2] = -rhs[0 + 2];
    result[0 + 3] = -rhs[0 + 3];
}

void negate_v1( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = -rhs[i];
    }
}

void negate_v2( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = -rhs[i * 2 + 0];
        result[i * 2 + 1] = -rhs[i * 2 + 1];
    }
}

void negate_v3( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = -rhs[i * 3 + 0];
        result[i * 3 + 1] = -rhs[i * 3 + 1];
        result[i * 3 + 2] = -rhs[i * 3 + 2];
    }
}

void negate_v4( float* result, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = -rhs[i * 4 + 0];
        result[i * 4 + 1] = -rhs[i * 4 + 1];
        result[i * 4 + 2] = -rhs[i * 4 + 2];
        result[i * 4 + 3] = -rhs[i * 4 + 3];
    }
}

void negate( unsigned int dispatch, float* result, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1:
            negate_u1( result, rhs, length );
            break;

        case DISPATCH_U2:
            negate_u2( result, rhs, length );
            break;

        case DISPATCH_U3:
            negate_u3( result, rhs, length );
            break;

        case DISPATCH_U4:
            negate_u4( result, rhs, length );
            break;

        case DISPATCH_V1:
            negate_v1( result, rhs, length );
            break;

        case DISPATCH_V2:
            negate_v2( result, rhs, length );
            break;

        case DISPATCH_V3:
            negate_v3( result, rhs, length );
            break;

        case DISPATCH_V4:
            negate_v4( result, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
