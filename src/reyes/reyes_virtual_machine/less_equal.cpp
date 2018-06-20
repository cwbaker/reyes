//
// less_equal.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "less_equal.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{

void less_equal_u1u1( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] <= rhs[0];
}

void less_equal_u1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] <= rhs[i];
    }
}

void less_equal_v1u1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] <= rhs[0];
    }
}

void less_equal_v1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] <= rhs[i];
    }
}

void less_equal( int dispatch, int* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            less_equal_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            less_equal_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            less_equal_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            less_equal_v1v1( result, lhs, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
