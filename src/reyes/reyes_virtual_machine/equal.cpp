//
// equal.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "equal.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{

void equal_u1u1( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] == rhs[0];
}

void equal_u2u2( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] == rhs[0];
    result[1] = lhs[1] == rhs[1];
}

void equal_u3u3( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] == rhs[0];
    result[1] = lhs[1] == rhs[1];
    result[2] = lhs[2] == rhs[2];
}

void equal_u4u4( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] == rhs[0];
    result[1] = lhs[1] == rhs[1];
    result[2] = lhs[2] == rhs[2];
    result[3] = lhs[3] == rhs[3];
}

void equal_u1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] == rhs[i];
    }
}

void equal_u2v2( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = lhs[0] == rhs[i * 2 + 0];
        result[i * 2 + 1] = lhs[1] == rhs[i * 2 + 1];
    }
}

void equal_u3v3( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[0] == rhs[i * 3 + 0];
        result[i * 3 + 1] = lhs[1] == rhs[i * 3 + 1];
        result[i * 3 + 2] = lhs[2] == rhs[i * 3 + 2];
    }
}

void equal_u4v4( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[0] == rhs[i * 4 + 0];
        result[i * 4 + 1] = lhs[1] == rhs[i * 4 + 1];
        result[i * 4 + 2] = lhs[2] == rhs[i * 4 + 2];
        result[i * 4 + 3] = lhs[3] == rhs[i * 4 + 3];
    }
}

void equal_v1u1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] == rhs[0];
    }
}

void equal_v2u2( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = lhs[i * 2 + 0] == rhs[0];
        result[i * 2 + 1] = lhs[i * 2 + 1] == rhs[1];
    }
}

void equal_v3u3( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[i * 3 + 0] == rhs[0];
        result[i * 3 + 1] = lhs[i * 3 + 1] == rhs[1];
        result[i * 3 + 2] = lhs[i * 3 + 2] == rhs[2];
    }
}

void equal_v4u4( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[i * 4 + 0] == rhs[0];
        result[i * 4 + 1] = lhs[i * 4 + 1] == rhs[1];
        result[i * 4 + 2] = lhs[i * 4 + 2] == rhs[2];
        result[i * 4 + 3] = lhs[i * 4 + 3] == rhs[3];
    }
}

void equal_v1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] == rhs[i];
    }
}

void equal_v2v2( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 2 + 0] = lhs[i * 2 + 0] == rhs[i * 2 + 0];
        result[i * 2 + 1] = lhs[i * 2 + 1] == rhs[i * 2 + 1];
    }
}

void equal_v3v3( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[i * 3 + 0] == rhs[i * 3 + 0];
        result[i * 3 + 1] = lhs[i * 3 + 1] == rhs[i * 3 + 1];
        result[i * 3 + 2] = lhs[i * 3 + 2] == rhs[i * 3 + 2];
    }
}

void equal_v4v4( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[i * 4 + 0] == rhs[i * 4 + 0];
        result[i * 4 + 1] = lhs[i * 4 + 1] == rhs[i * 4 + 1];
        result[i * 4 + 2] = lhs[i * 4 + 2] == rhs[i * 4 + 2];
        result[i * 4 + 3] = lhs[i * 4 + 3] == rhs[i * 4 + 3];
    }
}

void equal( int dispatch, int* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            equal_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2U2:
            equal_u2u2( result, lhs, rhs, length );
            break;

        case DISPATCH_U3U3:
            equal_u3u3( result, lhs, rhs, length );
            break;

        case DISPATCH_U4U4:
            equal_u4u4( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            equal_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2V2:
            equal_u2v2( result, lhs, rhs, length );
            break;

        case DISPATCH_U3V3:
            equal_u3v3( result, lhs, rhs, length );
            break;

        case DISPATCH_U4V4:
            equal_u4v4( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            equal_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2U2:
            equal_v2u2( result, lhs, rhs, length );
            break;

        case DISPATCH_V3U3:
            equal_v3u3( result, lhs, rhs, length );
            break;

        case DISPATCH_V4U4:
            equal_v4u4( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            equal_v1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2V2:
            equal_v2v2( result, lhs, rhs, length );
            break;

        case DISPATCH_V3V3:
            equal_v3v3( result, lhs, rhs, length );
            break;

        case DISPATCH_V4V4:
            equal_v4v4( result, lhs, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
