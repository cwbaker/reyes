//
// dot.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "dot.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>
#include <math/vec3.ipp>

using math::dot;
using math::vec3;

namespace reyes
{

void dot_v3v3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    REYES_ASSERT( result );
    REYES_ASSERT( lhs );
    REYES_ASSERT( rhs );
    REYES_ASSERT( length >= 0 );

    const vec3* lhs_values = reinterpret_cast<const vec3*>( lhs );
    const vec3* rhs_values = reinterpret_cast<const vec3*>( rhs );
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = dot( lhs_values[i], rhs_values[i] );
    }
}

void dot_u3v3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
    REYES_ASSERT( result );
    REYES_ASSERT( lhs );
    REYES_ASSERT( rhs );
    REYES_ASSERT( length >= 0 );

    const vec3 lhs_value( lhs[0], lhs[1], lhs[2] );
    const vec3* rhs_values = reinterpret_cast<const vec3*>( rhs );
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = dot( lhs_value, rhs_values[i] );
    }
}

void dot_v3u3( float* result, const float* lhs, const float* rhs, unsigned int length )
{
	dot_u3v3( result, rhs, lhs, length );
}

void dot_u3u3( float* result, const float* lhs, const float* rhs )
{
	dot_v3v3( result, lhs, rhs, 1 );
}

void dot( int dispatch, float* result, const float* lhs, const float* rhs, unsigned int length )
{
	switch ( dispatch )
	{
	    case DISPATCH_U3U3:
	    	dot_u3u3( result, lhs, rhs );
	    	break;

	    case DISPATCH_U3V3:
	    	dot_u3v3( result, lhs, rhs, length );
	    	break;

	    case DISPATCH_V3U3:
	    	dot_v3u3( result, lhs, rhs, length );
	    	break;

	    case DISPATCH_V3V3:
	    	dot_v3v3( result, lhs, rhs, length );
	    	break;

	    default:
	    	REYES_ASSERT( false );
	    	break;
	}
}

}
