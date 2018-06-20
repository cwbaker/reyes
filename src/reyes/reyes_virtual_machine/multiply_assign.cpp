//
// multiply_assign.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "multiply_assign.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{
    
void multiply_assign_u1u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0] *= rhs[0];
}

void multiply_assign_u2u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] *= rhs[0];
    result[0 + 1] *= rhs[0];
}

void multiply_assign_u3u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] *= rhs[0];
    result[0 + 1] *= rhs[0];
    result[0 + 2] *= rhs[0];
}

void multiply_assign_u4u1( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] *= rhs[0];
    result[0 + 1] *= rhs[0];
    result[0 + 2] *= rhs[0];
    result[0 + 3] *= rhs[0];
}

void multiply_assign_u2u2( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] *= rhs[0 + 0];
    result[0 + 1] *= rhs[0 + 1];
}

void multiply_assign_u3u3( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] *= rhs[0 + 0];
    result[0 + 1] *= rhs[0 + 1];
    result[0 + 2] *= rhs[0 + 2];
}

void multiply_assign_u4u4( float* result, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] *= rhs[0 + 0];
    result[0 + 1] *= rhs[0 + 1];
    result[0 + 2] *= rhs[0 + 2];
    result[0 + 3] *= rhs[0 + 3];
}

void multiply_assign_v1u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] *= rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i] *= rhs[0];
            }
        }
    }
}

void multiply_assign_v2u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 2 + 0] *= rhs[0];
            result[i * 2 + 1] *= rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 2 + 0] *= rhs[0];
                result[i * 2 + 1] *= rhs[0];
            }
        }
    }
}

void multiply_assign_v3u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 3 + 0] *= rhs[0];
            result[i * 3 + 1] *= rhs[0];
            result[i * 3 + 2] *= rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 3 + 0] *= rhs[0];
                result[i * 3 + 1] *= rhs[0];
                result[i * 3 + 2] *= rhs[0];
            }
        }
    }
}

void multiply_assign_v4u1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] *= rhs[0];
            result[i * 4 + 1] *= rhs[0];
            result[i * 4 + 2] *= rhs[0];
            result[i * 4 + 3] *= rhs[0];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] *= rhs[0];
                result[i * 4 + 1] *= rhs[0];
                result[i * 4 + 2] *= rhs[0];
                result[i * 4 + 3] *= rhs[0];
            }
        }
    }
}

void multiply_assign_v1v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] *= rhs[i];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i] *= rhs[i];
            }
        }
    }
}

void multiply_assign_v2v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 2 + 0] *= rhs[i];
            result[i * 2 + 1] *= rhs[i];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 2 + 0] *= rhs[i];
                result[i * 2 + 1] *= rhs[i];
            }
        }
    }
}

void multiply_assign_v3v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 3 + 0] *= rhs[i];
            result[i * 3 + 1] *= rhs[i];
            result[i * 3 + 2] *= rhs[i];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 3 + 0] *= rhs[i];
                result[i * 3 + 1] *= rhs[i];
                result[i * 3 + 2] *= rhs[i];
            }
        }
    }
}

void multiply_assign_v4v1( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] *= rhs[i];
            result[i * 4 + 1] *= rhs[i];
            result[i * 4 + 2] *= rhs[i];
            result[i * 4 + 3] *= rhs[i];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] *= rhs[i];
                result[i * 4 + 1] *= rhs[i];
                result[i * 4 + 2] *= rhs[i];
                result[i * 4 + 3] *= rhs[i];
            }
        }
    }
}

void multiply_assign_v2u2( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 2 + 0] *= rhs[0 + 0];
            result[i * 2 + 1] *= rhs[0 + 1];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 2 + 0] *= rhs[0 + 0];
                result[i * 2 + 1] *= rhs[0 + 1];
            }
        }
    }
}

void multiply_assign_v3u3( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 3 + 0] *= rhs[0 + 0];
            result[i * 3 + 1] *= rhs[0 + 1];
            result[i * 3 + 2] *= rhs[0 + 2];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 3 + 0] *= rhs[0 + 0];
                result[i * 3 + 1] *= rhs[0 + 1];
                result[i * 3 + 2] *= rhs[0 + 2];
            }
        }
    }
}

void multiply_assign_v4u4( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] *= rhs[0 + 0];
            result[i * 4 + 1] *= rhs[0 + 1];
            result[i * 4 + 2] *= rhs[0 + 2];
            result[i * 4 + 3] *= rhs[0 + 3];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] *= rhs[0 + 0];
                result[i * 4 + 1] *= rhs[0 + 1];
                result[i * 4 + 2] *= rhs[0 + 2];
                result[i * 4 + 3] *= rhs[0 + 3];
            }
        }
    }
}

void multiply_assign_v2v2( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 2 + 0] *= rhs[i * 2 + 0];
            result[i * 2 + 1] *= rhs[i * 2 + 1];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 2 + 0] *= rhs[i * 2 + 0];
                result[i * 2 + 1] *= rhs[i * 2 + 1];
            }
        }
    }
}

void multiply_assign_v3v3( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 3 + 0] *= rhs[i * 3 + 0];
            result[i * 3 + 1] *= rhs[i * 3 + 1];
            result[i * 3 + 2] *= rhs[i * 3 + 2];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 3 + 0] *= rhs[i * 3 + 0];
                result[i * 3 + 1] *= rhs[i * 3 + 1];
                result[i * 3 + 2] *= rhs[i * 3 + 2];
            }
        }
    }
}

void multiply_assign_v4v4( float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    if ( !mask )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i * 4 + 0] *= rhs[i * 4 + 0];
            result[i * 4 + 1] *= rhs[i * 4 + 1];
            result[i * 4 + 2] *= rhs[i * 4 + 2];
            result[i * 4 + 3] *= rhs[i * 4 + 3];
        }
    }
    else
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            if ( mask[i] )
            {
                result[i * 4 + 0] *= rhs[i * 4 + 0];
                result[i * 4 + 1] *= rhs[i * 4 + 1];
                result[i * 4 + 2] *= rhs[i * 4 + 2];
                result[i * 4 + 3] *= rhs[i * 4 + 3];
            }
        }
    }
}

void multiply_assign( int dispatch, float* result, const float* rhs, const unsigned char* mask, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            multiply_assign_u1u1( result, rhs, length );
            break;

        case DISPATCH_U2U1:
            multiply_assign_u2u1( result, rhs, length );
            break;

        case DISPATCH_U3U1:
            multiply_assign_u3u1( result, rhs, length );
            break;

        case DISPATCH_U4U1:
            multiply_assign_u4u1( result, rhs, length );
            break;

        case DISPATCH_U2U2:
            multiply_assign_u2u2( result, rhs, length );
            break;

        case DISPATCH_U3U3:
            multiply_assign_u3u3( result, rhs, length );
            break;

        case DISPATCH_U4U4:
            multiply_assign_u4u4( result, rhs, length );
            break;

        case DISPATCH_V1U1:
            multiply_assign_v1u1( result, rhs, mask, length );
            break;

        case DISPATCH_V2U1:
            multiply_assign_v2u1( result, rhs, mask, length );
            break;

        case DISPATCH_V3U1:
            multiply_assign_v3u1( result, rhs, mask, length );
            break;

        case DISPATCH_V4U1:
            multiply_assign_v4u1( result, rhs, mask, length );
            break;

        case DISPATCH_V2U2:
            multiply_assign_v2u2( result, rhs, mask, length );
            break;

        case DISPATCH_V3U3:
            multiply_assign_v3u3( result, rhs, mask, length );
            break;

        case DISPATCH_V4U4:
            multiply_assign_v4u4( result, rhs, mask, length );
            break;

        case DISPATCH_V1V1:
            multiply_assign_v1v1( result, rhs, mask, length );
            break;

        case DISPATCH_V2V1:
            multiply_assign_v2v1( result, rhs, mask, length );
            break;

        case DISPATCH_V3V1:
            multiply_assign_v3v1( result, rhs, mask, length );
            break;

        case DISPATCH_V4V1:
            multiply_assign_v4v1( result, rhs, mask, length );
            break;

        case DISPATCH_V2V2:
            multiply_assign_v2v2( result, rhs, mask, length );
            break;

        case DISPATCH_V3V3:
            multiply_assign_v3v3( result, rhs, mask, length );
            break;

        case DISPATCH_V4V4:
            multiply_assign_v4v4( result, rhs, mask, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
