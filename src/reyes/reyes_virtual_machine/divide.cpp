
#include "divide.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>

namespace reyes
{

void divide_u1u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] / rhs[0];
}

void divide_u2u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] / rhs[0];
    result[1] = lhs[1] / rhs[0];
}

void divide_u3u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] / rhs[0];
    result[1] = lhs[1] / rhs[0];
    result[2] = lhs[2] / rhs[0];
}

void divide_u4u1( float* result, const float* lhs, const float* rhs, unsigned int /*length*/ )
{
    result[0] = lhs[0] / rhs[0];
    result[1] = lhs[1] / rhs[0];
    result[2] = lhs[2] / rhs[0];
    result[3] = lhs[3] / rhs[0];
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
        result[i * 2 + 0] = lhs[0] / rhs[i];
        result[i * 2 + 1] = lhs[1] / rhs[i];
    }
}

void divide_u3v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[0] / rhs[i];
        result[i * 3 + 1] = lhs[1] / rhs[i];
        result[i * 3 + 2] = lhs[2] / rhs[i];
    }
}

void divide_u4v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[0] / rhs[i];
        result[i * 4 + 1] = lhs[1] / rhs[i];
        result[i * 4 + 2] = lhs[2] / rhs[i];
        result[i * 4 + 3] = lhs[3] / rhs[i];
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
        result[i * 2 + 0] = lhs[i * 2 + 0] / rhs[0];
        result[i * 2 + 1] = lhs[i * 2 + 1] / rhs[0];
    }
}

void divide_v3u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[i * 3 + 0] / rhs[0];
        result[i * 3 + 1] = lhs[i * 3 + 1] / rhs[0];
        result[i * 3 + 2] = lhs[i * 3 + 2] / rhs[0];
    }
}

void divide_v4u1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 4 + 0] = lhs[i * 4 + 0] / rhs[0];
        result[i * 4 + 1] = lhs[i * 4 + 1] / rhs[0];
        result[i * 4 + 2] = lhs[i * 4 + 2] / rhs[0];
        result[i * 4 + 3] = lhs[i * 4 + 3] / rhs[0];
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
        result[i * 2 + 0] = lhs[i * 2 + 0] / rhs[i];
        result[i * 2 + 1] = lhs[i * 2 + 1] / rhs[i];
    }
}

void divide_v3v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i * 3 + 0] = lhs[i * 3 + 0] / rhs[i];
        result[i * 3 + 1] = lhs[i * 3 + 1] / rhs[i];
        result[i * 3 + 2] = lhs[i * 3 + 2] / rhs[i];
    }
}

void divide_v4v1( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        const float denominator = rhs[i];
        result[i * 4 + 0] = lhs[i * 4 + 0] / denominator;
        result[i * 4 + 1] = lhs[i * 4 + 1] / denominator;
        result[i * 4 + 2] = lhs[i * 4 + 2] / denominator;
        result[i * 4 + 3] = lhs[i * 4 + 3] / denominator;
    }
}

void divide( int dispatch, float* result, const float* lhs, const float* rhs, unsigned int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U1U1:
            divide_u1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2U1:
            divide_u2u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U3U1:
            divide_u3u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U4U1:
            divide_u4u1( result, lhs, rhs, length );
            break;

        case DISPATCH_U1V1:
            divide_u1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_U2V1:
            divide_u2v1( result, lhs, rhs, length );
            break;

        case DISPATCH_U3V1:
            divide_u3v1( result, lhs, rhs, length );
            break;

        case DISPATCH_U4V1:
            divide_u4v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1U1:
            divide_v1u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2U1:
            divide_v2u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V3U1:
            divide_v3u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V4U1:   
            divide_v4u1( result, lhs, rhs, length );
            break;

        case DISPATCH_V1V1:
            divide_v1v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V2V1:
            divide_v2v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V3V1:
            divide_v3v1( result, lhs, rhs, length );
            break;

        case DISPATCH_V4V1:
            divide_v4v1( result, lhs, rhs, length );
            break;

        default:
            REYES_ASSERT( false );
            break;        
    }
}

}
