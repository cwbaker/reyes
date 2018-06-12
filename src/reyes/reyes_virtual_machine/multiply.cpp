//
// multiply.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "multiply.hpp"
#include "Instruction.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace fx
{

void multiply_u1u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] * rhs[0];
}

void multiply_u2u2( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] * rhs[0 + 0];
    result[0 + 1] = lhs[0 + 1] * rhs[0 + 1];
}

void multiply_u3u3( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] * rhs[0 + 0];
    result[0 + 1] = lhs[0 + 1] * rhs[0 + 1];
    result[0 + 2] = lhs[0 + 2] * rhs[0 + 2];
}

void multiply_u4u4( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] * rhs[0 + 0];
    result[0 + 1] = lhs[0 + 1] * rhs[0 + 1];
    result[0 + 2] = lhs[0 + 2] * rhs[0 + 2];
    result[0 + 3] = lhs[0 + 3] * rhs[0 + 3];
}

void multiply_u1v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] * rhs[i];
    }
}

void multiply_u2v2( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] * rhs[i + 0];
        result[i + 1] = lhs[0 + 1] * rhs[i + 1];
    }
}

void multiply_u3v3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] * rhs[i + 0];
        result[i + 1] = lhs[0 + 1] * rhs[i + 1];
        result[i + 2] = lhs[0 + 2] * rhs[i + 2];
    }
}

void multiply_u4v4( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] * rhs[i + 0];
        result[i + 1] = lhs[0 + 1] * rhs[i + 1];
        result[i + 2] = lhs[0 + 2] * rhs[i + 2];
        result[i + 3] = lhs[0 + 3] * rhs[i + 3];
    }
}

void multiply_v1u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] * rhs[0];
    }
}

void multiply_v2u2( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[0 + 0];
        result[i + 1] = lhs[i + 1] * rhs[0 + 1];
    }
}

void multiply_v3u3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[0 + 0];
        result[i + 1] = lhs[i + 1] * rhs[0 + 1];
        result[i + 2] = lhs[i + 2] * rhs[0 + 2];
    }
}

void multiply_v4u4( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[0 + 0];
        result[i + 1] = lhs[i + 1] * rhs[0 + 1];
        result[i + 2] = lhs[i + 2] * rhs[0 + 2];
        result[i + 3] = lhs[i + 3] * rhs[0 + 3];
    }
}

void multiply_v1v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] * rhs[i];
    }
}

void multiply_v2v2( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[i + 0];
        result[i + 1] = lhs[i + 1] * rhs[i + 1];
    }
}

void multiply_v3v3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[i + 0];
        result[i + 1] = lhs[i + 1] * rhs[i + 1];
        result[i + 2] = lhs[i + 2] * rhs[i + 2];
    }
}

void multiply_v4v4( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[i + 0];
        result[i + 1] = lhs[i + 1] * rhs[i + 1];
        result[i + 2] = lhs[i + 2] * rhs[i + 2];
        result[i + 3] = lhs[i + 3] * rhs[i + 3];
    }
}

void multiply_u2u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] * rhs[0];
    result[0 + 1] = lhs[0 + 1] * rhs[0];
}

void multiply_u3u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] * rhs[0];
    result[0 + 1] = lhs[0 + 1] * rhs[0];
    result[0 + 2] = lhs[0 + 2] * rhs[0];
}

void multiply_u4u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] * rhs[0];
    result[0 + 1] = lhs[0 + 1] * rhs[0];
    result[0 + 2] = lhs[0 + 2] * rhs[0];
    result[0 + 3] = lhs[0 + 3] * rhs[0];
}

void multiply_u2v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] * rhs[i];
        result[i + 1] = lhs[0 + 1] * rhs[i];
    }
}

void multiply_u3v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] * rhs[i];
        result[i + 1] = lhs[0 + 1] * rhs[i];
        result[i + 2] = lhs[0 + 2] * rhs[i];
    }
}

void multiply_u4v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] * rhs[i];
        result[i + 1] = lhs[0 + 1] * rhs[i];
        result[i + 2] = lhs[0 + 2] * rhs[i];
        result[i + 3] = lhs[0 + 3] * rhs[i];
    }
}

void multiply_v2u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[0];
        result[i + 1] = lhs[i + 1] * rhs[0];
    }
}

void multiply_v3u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[0];
        result[i + 1] = lhs[i + 1] * rhs[0];
        result[i + 2] = lhs[i + 2] * rhs[0];
    }
}

void multiply_v4u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[0];
        result[i + 1] = lhs[i + 1] * rhs[0];
        result[i + 2] = lhs[i + 2] * rhs[0];
        result[i + 3] = lhs[i + 3] * rhs[0];
    }
}

void multiply_v2v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[i];
        result[i + 1] = lhs[i + 1] * rhs[i];
    }
}

void multiply_v3v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[i];
        result[i + 1] = lhs[i + 1] * rhs[i];
        result[i + 2] = lhs[i + 2] * rhs[i];
    }
}

void multiply_v4v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] * rhs[i];
        result[i + 1] = lhs[i + 1] * rhs[i];
        result[i + 2] = lhs[i + 2] * rhs[i];
        result[i + 3] = lhs[i + 3] * rhs[i];
    }
}

void multiply( int dispatch, float* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case INSTRUCTION_U1U1:
            multiply_u1u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U2U2:
            multiply_u2u2( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U3U3:
            multiply_u3u3( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U4U4:
            multiply_u4u4( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U1V1:
            multiply_u1v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U2V2:
            multiply_u2v2( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U3V3:
            multiply_u3v3( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U4V4:
            multiply_u4v4( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V1U1:
            multiply_v1u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V2U2:
            multiply_v2u2( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V3U3:
            multiply_v3u3( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V4U4:
            multiply_v4u4( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V1V1:
            multiply_v1v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V2V2:
            multiply_v2v2( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V3V3:
            multiply_v3v3( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V4V4:
            multiply_v4v4( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U2U1:
            multiply_u2u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U3U1:
            multiply_u3u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U4U1:
            multiply_u4u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U2V1:
            multiply_u2v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U3V1:
            multiply_u3v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U4V1:
            multiply_u4v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V2U1:
            multiply_v2u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V3U1:
            multiply_v3u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V4U1:
            multiply_v4u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V2V1:
            multiply_v2v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V3V1:
            multiply_v3v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V4V1:
            multiply_v4v1( result, lhs, rhs, length );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

}

}
