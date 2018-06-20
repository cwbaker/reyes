//
// logical_and.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "logical_and.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{

void logical_and_u1u1( int* result, const int* lhs, const int* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] && rhs[0];
}

void logical_and_u1v1( int* result, const int* lhs, const int* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] && rhs[i];
    }
}

void logical_and_v1u1( int* result, const int* lhs, const int* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] && rhs[0];
    }
}

void logical_and_v1v1( int* result, const int* lhs, const int* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] && rhs[i];
    }
}

void logical_and( int dispatch, int* result, const int* lhs, const int* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            logical_and_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            logical_and_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            logical_and_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            logical_and_v1v1( result, lhs, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
