//
// subtract.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "subtract.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>

namespace reyes
{

void subtract_u1u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] - rhs[0];
}

void subtract_u2u2( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] - rhs[0];
    result[1] = lhs[1] - rhs[1];
}

void subtract_u3u3( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] - rhs[0];
    result[1] = lhs[1] - rhs[1];
    result[2] = lhs[2] - rhs[2];
}

void subtract_u4u4( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] - rhs[0];
    result[1] = lhs[1] - rhs[1];
    result[2] = lhs[2] - rhs[2];
    result[3] = lhs[3] - rhs[3];
}

void subtract_u1v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] - rhs[i];
    }
}

void subtract_u2v2( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = lhs[0] - rhs[i * 2 + 0];
        result[i * 2 + 1] = lhs[1] - rhs[i * 2 + 1];
    }
}

void subtract_u3v3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[0] - rhs[i * 3 + 0];
        result[i * 3 + 1] = lhs[1] - rhs[i * 3 + 1];
        result[i * 3 + 2] = lhs[2] - rhs[i * 3 + 2];
    }
}

void subtract_u4v4( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[0] - rhs[i * 4 + 0];
        result[i * 4 + 1] = lhs[1] - rhs[i * 4 + 1];
        result[i * 4 + 2] = lhs[2] - rhs[i * 4 + 2];
        result[i * 4 + 3] = lhs[3] - rhs[i * 4 + 3];
    }
}

void subtract_v1u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] - rhs[0];
    }
}

void subtract_v2u2( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = lhs[i * 2 + 0] - rhs[0];
        result[i * 2 + 1] = lhs[i * 2 + 1] - rhs[1];
    }
}

void subtract_v3u3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[i * 3 + 0] - rhs[0];
        result[i * 3 + 1] = lhs[i * 3 + 1] - rhs[1];
        result[i * 3 + 2] = lhs[i * 3 + 2] - rhs[2];
    }
}

void subtract_v4u4( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[i * 4 + 0] - rhs[0];
        result[i * 4 + 1] = lhs[i * 4 + 1] - rhs[1];
        result[i * 4 + 2] = lhs[i * 4 + 2] - rhs[2];
        result[i * 4 + 3] = lhs[i * 4 + 3] - rhs[3];
    }
}

void subtract_v1v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] - rhs[i];
    }
}

void subtract_v2v2( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = lhs[i * 2 + 0] - rhs[i * 2 + 0];
        result[i * 2 + 1] = lhs[i * 2 + 1] - rhs[i * 2 + 1];
    }
}

void subtract_v3v3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[i * 3 + 0] - rhs[i * 3 + 0];
        result[i * 3 + 1] = lhs[i * 3 + 1] - rhs[i * 3 + 1];
        result[i * 3 + 2] = lhs[i * 3 + 2] - rhs[i * 3 + 2];
    }
}

void subtract_v4v4( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[i * 4 + 0] - rhs[i * 4 + 0];
        result[i * 4 + 1] = lhs[i * 4 + 1] - rhs[i * 4 + 1];
        result[i * 4 + 2] = lhs[i * 4 + 2] - rhs[i * 4 + 2];
        result[i * 4 + 3] = lhs[i * 4 + 3] - rhs[i * 4 + 3];
    }
}

void subtract( int dispatch, float* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            subtract_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2U2:
            subtract_u2u2( result, lhs, rhs, length );
            break;

        case DISPATCH_U3U3:
            subtract_u3u3( result, lhs, rhs, length );
            break;

        case DISPATCH_U4U4:
            subtract_u4u4( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            subtract_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2V2:
            subtract_u2v2( result, lhs, rhs, length );
            break;

        case DISPATCH_U3V3:
            subtract_u3v3( result, lhs, rhs, length );
            break;

        case DISPATCH_U4V4:
            subtract_u4v4( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            subtract_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2U2:
            subtract_v2u2( result, lhs, rhs, length );
            break;

        case DISPATCH_V3U3:
            subtract_v3u3( result, lhs, rhs, length );
            break;

        case DISPATCH_V4U4:
            subtract_v4u4( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            subtract_v1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2V2:
            subtract_v2v2( result, lhs, rhs, length );
            break;

        case DISPATCH_V3V3:
            subtract_v3v3( result, lhs, rhs, length );
            break;

        case DISPATCH_V4V4:
            subtract_v4v4( result, lhs, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }    
}

}
