//
// assign.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "assign.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>

namespace reyes
{

void assign_u1u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
}

void assign_u2u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
    result[1] = rhs[0];
}

void assign_u3u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
    result[1] = rhs[0];
    result[2] = rhs[0];
}

void assign_u4u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
    result[1] = rhs[0];
    result[2] = rhs[0];
    result[3] = rhs[0];
}

void assign_u2u2( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
    result[1] = rhs[1];
}

void assign_u3u3( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
    result[1] = rhs[1];
    result[2] = rhs[2];
}

void assign_u4u4( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] = rhs[0];
    result[1] = rhs[1];
    result[2] = rhs[2];
    result[3] = rhs[3];
}

void assign_v1u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i] = rhs[0];
            }
        }
    }
}

void assign_v2u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[0];
            result[i * 4 + 1] = rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[0];
                result[i * 4 + 1] = rhs[0];
            }
        }
    }
}

void assign_v3u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[0];
            result[i * 4 + 1] = rhs[0];
            result[i * 4 + 2] = rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[0];
                result[i * 4 + 1] = rhs[0];
                result[i * 4 + 2] = rhs[0];
            }
        }
    }
}

void assign_v4u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[0];
            result[i * 4 + 1] = rhs[0];
            result[i * 4 + 2] = rhs[0];
            result[i * 4 + 3] = rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[0];
                result[i * 4 + 1] = rhs[0];
                result[i * 4 + 2] = rhs[0];
                result[i * 4 + 3] = rhs[0];
            }
        }
    }
}

void assign_v2u2( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 2 + 0] = rhs[0];
            result[i * 2 + 1] = rhs[1];
        }
    }
    else 
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 2 + 0] = rhs[0];
                result[i * 2 + 1] = rhs[1];
            }
        }
    }
}

void assign_v3u3( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 3 + 0] = rhs[0];
            result[i * 3 + 1] = rhs[1];
            result[i * 3 + 2] = rhs[2];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 3 + 0] = rhs[0];
                result[i * 3 + 1] = rhs[1];
                result[i * 3 + 2] = rhs[2];
            }
        }
    }
}

void assign_v4u4( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[0];
            result[i * 4 + 1] = rhs[1];
            result[i * 4 + 2] = rhs[2];
            result[i * 4 + 3] = rhs[3];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[0];
                result[i * 4 + 1] = rhs[1];
                result[i * 4 + 2] = rhs[2];
                result[i * 4 + 3] = rhs[3];
            }
        }
    }
}

void assign_v1v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = rhs[i];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i] = rhs[i];
            }
        }
    }
}

void assign_v2v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[i + 0];
            result[i * 4 + 1] = rhs[i + 0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[i + 0];
                result[i * 4 + 1] = rhs[i + 0];
            }
        }
    }
}

void assign_v3v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[i + 0];
            result[i * 4 + 1] = rhs[i + 0];
            result[i * 4 + 2] = rhs[i + 0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[i + 0];
                result[i * 4 + 1] = rhs[i + 0];
                result[i * 4 + 2] = rhs[i + 0];
            }
        }
    }
}

void assign_v4v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[i + 0];
            result[i * 4 + 1] = rhs[i + 0];
            result[i * 4 + 2] = rhs[i + 0];
            result[i * 4 + 3] = rhs[i + 0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[i + 0];
                result[i * 4 + 1] = rhs[i + 0];
                result[i * 4 + 2] = rhs[i + 0];
                result[i * 4 + 3] = rhs[i + 0];
            }
        }
    }
}

void assign_v2v2( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 2 + 0] = rhs[i * 2 + 0];
            result[i * 2 + 1] = rhs[i * 2 + 1];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 2 + 0] = rhs[i * 2 + 0];
                result[i * 2 + 1] = rhs[i * 2 + 1];
            }
        }
    }
}

void assign_v3v3( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 3 + 0] = rhs[i * 3 + 0];
            result[i * 3 + 1] = rhs[i * 3 + 1];
            result[i * 3 + 2] = rhs[i * 3 + 2];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 3 + 0] = rhs[i * 3 + 0];
                result[i * 3 + 1] = rhs[i * 3 + 1];
                result[i * 3 + 2] = rhs[i * 3 + 2];
            }
        }
    }
}

void assign_v4v4( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] = rhs[i * 4 + 0];
            result[i * 4 + 1] = rhs[i * 4 + 1];
            result[i * 4 + 2] = rhs[i * 4 + 2];
            result[i * 4 + 3] = rhs[i * 4 + 3];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] = rhs[i * 4 + 0];
                result[i * 4 + 1] = rhs[i * 4 + 1];
                result[i * 4 + 2] = rhs[i * 4 + 2];
                result[i * 4 + 3] = rhs[i * 4 + 3];
            }
        }
    }
}

void assign( int dispatch, float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    REYES_ASSERT( result );
    REYES_ASSERT( rhs );

    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            assign_u1u1( result, rhs, length );
            break;

        case DISPATCH_U2U1:
            assign_u2u1( result, rhs, length );
            break;

        case DISPATCH_U3U1:
            assign_u3u1( result, rhs, length );
            break;

        case DISPATCH_U4U1:
            assign_u4u1( result, rhs, length );
            break;

        case DISPATCH_U2U2:
            assign_u2u2( result, rhs, length );
            break;

        case DISPATCH_U3U3:
            assign_u3u3( result, rhs, length );
            break;

        case DISPATCH_U4U4:
            assign_u4u4( result, rhs, length );
            break;

        case DISPATCH_V1U1:
            assign_v1u1( result, rhs, mask, length );
            break;

        case DISPATCH_V2U1:
            assign_v2u1( result, rhs, mask, length );
            break;

        case DISPATCH_V3U1:
            assign_v3u1( result, rhs, mask, length );
            break;

        case DISPATCH_V4U1:
            assign_v4u1( result, rhs, mask, length );
            break;

        case DISPATCH_V2U2:
            assign_v2u2( result, rhs, mask, length );
            break;

        case DISPATCH_V3U3:
            assign_v3u3( result, rhs, mask, length );
            break;

        case DISPATCH_V4U4:
            assign_v4u4( result, rhs, mask, length );
            break;

        case DISPATCH_V1V1:
            assign_v1v1( result, rhs, mask, length );
            break;

        case DISPATCH_V2V1:
            assign_v2v1( result, rhs, mask, length );
            break;

        case DISPATCH_V3V1:
            assign_v3v1( result, rhs, mask, length );
            break;

        case DISPATCH_V4V1:
            assign_v4v1( result, rhs, mask, length );
            break;

        case DISPATCH_V2V2:
            assign_v2v2( result, rhs, mask, length );
            break;

        case DISPATCH_V3V3:
            assign_v3v3( result, rhs, mask, length );
            break;

        case DISPATCH_V4V4:
            assign_v4v4( result, rhs, mask, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
