//
// greater_equal.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "greater_equal.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>

namespace reyes
{

void greater_equal_u1u1( int* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] >= rhs[0];
}

void greater_equal_u1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] >= rhs[i];
    }
}

void greater_equal_v1u1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] >= rhs[0];
    }
}

void greater_equal_v1v1( int* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] >= rhs[i];
    }
}

void greater_equal( int dispatch, int* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            greater_equal_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            greater_equal_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            greater_equal_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            greater_equal_v1v1( result, lhs, rhs, length );
            break;
    
        default:
            REYES_ASSERT( false );
            break;
    }  
}

}
