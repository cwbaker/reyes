//
// convert.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "convert.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>

namespace reyes
{

void convert_v3v1( float* result, const float* rhs, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        float value = rhs[i];
        result[i * 3 + 0] = value;
        result[i * 3 + 1] = value;
        result[i * 3 + 2] = value;
    }
}

void convert_v16v1( float* result, const float* rhs, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        float value = rhs[i];
        result[i * 16 + 0] = value;
        result[i * 16 + 1] = 0.0f;
        result[i * 16 + 2] = 0.0f;
        result[i * 16 + 3] = 0.0f;
        result[i * 16 + 4] = 0.0f;
        result[i * 16 + 5] = value;
        result[i * 16 + 6] = 0.0f;
        result[i * 16 + 7] = 0.0f;
        result[i * 16 + 8] = 0.0f;
        result[i * 16 + 9] = 0.0f;
        result[i * 16 + 10] = value;
        result[i * 16 + 11] = 0.0f;
        result[i * 16 + 12] = 0.0f;
        result[i * 16 + 13] = 0.0f;
        result[i * 16 + 14] = 0.0f;
        result[i * 16 + 15] = value;
    }
}

void convert( int dispatch, float* result, const float* rhs, int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U3U1:
            convert_v3v1( result, rhs, 1 );
            break;

        case DISPATCH_U16U1:
            convert_v16v1( result, rhs, 1 );
            break;

        case DISPATCH_V3V1:
            convert_v3v1( result, rhs, length );
            break;

        case DISPATCH_V16V1:
            convert_v16v1( result, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
