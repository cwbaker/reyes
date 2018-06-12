
#include "divide.hpp"
#include "Instruction.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace fx
{

void divide_u1u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] / rhs[0];
}

void divide_u2u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] / rhs[0];
    result[0 + 1] = lhs[0 + 1] / rhs[0];
}

void divide_u3u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] / rhs[0];
    result[0 + 1] = lhs[0 + 1] / rhs[0];
    result[0 + 2] = lhs[0 + 2] / rhs[0];
}

void divide_u4u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0 + 0] = lhs[0 + 0] / rhs[0];
    result[0 + 1] = lhs[0 + 1] / rhs[0];
    result[0 + 2] = lhs[0 + 2] / rhs[0];
    result[0 + 3] = lhs[0 + 3] / rhs[0];
}

void divide_u1v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[0] / rhs[i];
    }
}

void divide_u2v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] / rhs[i];
        result[i + 1] = lhs[0 + 1] / rhs[i];
    }
}

void divide_u3v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] / rhs[i];
        result[i + 1] = lhs[0 + 1] / rhs[i];
        result[i + 2] = lhs[0 + 2] / rhs[i];
    }
}

void divide_u4v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[0 + 0] / rhs[i];
        result[i + 1] = lhs[0 + 1] / rhs[i];
        result[i + 2] = lhs[0 + 2] / rhs[i];
        result[i + 3] = lhs[0 + 3] / rhs[i];
    }
}

void divide_v1u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] / rhs[0];
    }
}

void divide_v2u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] / rhs[0];
        result[i + 1] = lhs[i + 1] / rhs[0];
    }
}

void divide_v3u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] / rhs[0];
        result[i + 1] = lhs[i + 1] / rhs[0];
        result[i + 2] = lhs[i + 2] / rhs[0];
    }
}

void divide_v4u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] / rhs[0];
        result[i + 1] = lhs[i + 1] / rhs[0];
        result[i + 2] = lhs[i + 2] / rhs[0];
        result[i + 3] = lhs[i + 3] / rhs[0];
    }
}

void divide_v1v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = lhs[i] / rhs[i];
    }
}

void divide_v2v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] / rhs[i];
        result[i + 1] = lhs[i + 1] / rhs[i];
    }
}

void divide_v3v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] / rhs[i];
        result[i + 1] = lhs[i + 1] / rhs[i];
        result[i + 2] = lhs[i + 2] / rhs[i];
    }
}

void divide_v4v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i + 0] = lhs[i + 0] / rhs[i];
        result[i + 1] = lhs[i + 1] / rhs[i];
        result[i + 2] = lhs[i + 2] / rhs[i];
        result[i + 3] = lhs[i + 3] / rhs[i];
    }
}

void divide( int dispatch, float* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case INSTRUCTION_U1U1:
            divide_u1u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U2U1:
            divide_u2u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U3U1:
            divide_u3u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U4U1:
            divide_u4u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U1V1:
            divide_u1v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U2V1:
            divide_u2v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U3V1:
            divide_u3v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_U4V1:
            divide_u4v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V1U1:
            divide_v1u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V2U1:
            divide_v2u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V3U1:
            divide_v3u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V4U1:   
            divide_v4u1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V1V1:
            divide_v1v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V2V1:
            divide_v2v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V3V1:
            divide_v3v1( result, lhs, rhs, length );
            break;

        case INSTRUCTION_V4V1:
            divide_v4v1( result, lhs, rhs, length );
            break;

        default:
            SWEET_ASSERT( false );
            break;        
    }
}

}

}
