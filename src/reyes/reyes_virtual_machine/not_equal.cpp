//
// not_equal.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "not_equal.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{

void not_equal_u1u1( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] != rhs[0];
}

void not_equal_u2u2( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] != rhs[0 + 0];
    result[0 + 1] = lhs[0 + 1] != rhs[0 + 1];
}

void not_equal_u3u3( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] != rhs[0 + 0];
    result[0 + 1] = lhs[0 + 1] != rhs[0 + 1];
    result[0 + 2] = lhs[0 + 2] != rhs[0 + 2];
}

void not_equal_u4u4( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] != rhs[0 + 0];
    result[0 + 1] = lhs[0 + 1] != rhs[0 + 1];
    result[0 + 2] = lhs[0 + 2] != rhs[0 + 2];
    result[0 + 3] = lhs[0 + 3] != rhs[0 + 3];
}

void not_equal_u1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] != rhs[i];
    }
}

void not_equal_u2v2( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] != rhs[i + 0];
        result[i + 1] = lhs[0 + 1] != rhs[i + 1];
    }
}

void not_equal_u3v3( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] != rhs[i + 0];
        result[i + 1] = lhs[0 + 1] != rhs[i + 1];
        result[i + 2] = lhs[0 + 2] != rhs[i + 2];
    }
}

void not_equal_u4v4( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] != rhs[i + 0];
        result[i + 1] = lhs[0 + 1] != rhs[i + 1];
        result[i + 2] = lhs[0 + 2] != rhs[i + 2];
        result[i + 3] = lhs[0 + 3] != rhs[i + 3];
    }
}

void not_equal_v1u1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] != rhs[0];
    }
}

void not_equal_v2u2( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] != rhs[0 + 0];
        result[i + 1] = lhs[i + 1] != rhs[0 + 1];
    }
}

void not_equal_v3u3( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] != rhs[0 + 0];
        result[i + 1] = lhs[i + 1] != rhs[0 + 1];
        result[i + 2] = lhs[i + 2] != rhs[0 + 2];
    }
}

void not_equal_v4u4( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] != rhs[0 + 0];
        result[i + 1] = lhs[i + 1] != rhs[0 + 1];
        result[i + 2] = lhs[i + 2] != rhs[0 + 2];
        result[i + 3] = lhs[i + 3] != rhs[0 + 3];
    }
}

void not_equal_v1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] != rhs[i];
    }
}

void not_equal_v2v2( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] != rhs[i + 0];
        result[i + 1] = lhs[i + 1] != rhs[i + 1];
    }
}

void not_equal_v3v3( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] != rhs[i + 0];
        result[i + 1] = lhs[i + 1] != rhs[i + 1];
        result[i + 2] = lhs[i + 2] != rhs[i + 2];
    }
}

void not_equal_v4v4( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] != rhs[i + 0];
        result[i + 1] = lhs[i + 1] != rhs[i + 1];
        result[i + 2] = lhs[i + 2] != rhs[i + 2];
        result[i + 3] = lhs[i + 3] != rhs[i + 3];
    }
}

void not_equal( int dispatch, int* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            not_equal_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2U2:
            not_equal_u2u2( result, lhs, rhs, length );
            break;

        case DISPATCH_U3U3:
            not_equal_u3u3( result, lhs, rhs, length );
            break;

        case DISPATCH_U4U4:
            not_equal_u4u4( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            not_equal_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2V2:
            not_equal_u2v2( result, lhs, rhs, length );
            break;

        case DISPATCH_U3V3:
            not_equal_u3v3( result, lhs, rhs, length );
            break;

        case DISPATCH_U4V4:
            not_equal_u4v4( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            not_equal_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2U2:
            not_equal_v2u2( result, lhs, rhs, length );
            break;

        case DISPATCH_V3U3:
            not_equal_v3u3( result, lhs, rhs, length );
            break;

        case DISPATCH_V4U4:
            not_equal_v4u4( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            not_equal_v1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2V2:
            not_equal_v2v2( result, lhs, rhs, length );
            break;

        case DISPATCH_V3V3:
            not_equal_v3v3( result, lhs, rhs, length );
            break;

        case DISPATCH_V4V4:
            not_equal_v4v4( result, lhs, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
