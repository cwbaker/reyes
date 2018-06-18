//
// ctransform.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ctransform.hpp"
#include "Dispatch.hpp"
#include <math/vec3.ipp>
#include <reyes/assert.hpp>

namespace reyes
{

void ctransform_from_hsv_v3( math::vec3* result, const math::vec3* color, unsigned int length )
{
    REYES_ASSERT( result );
    REYES_ASSERT( color );
    REYES_ASSERT( length >= 0 );
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = rgb_from_hsv( color[i] );
    }
}

void ctransform_from_hsl_v3( math::vec3* result, const math::vec3* color, unsigned int length  )
{
    REYES_ASSERT( result );
    REYES_ASSERT( color );
    REYES_ASSERT( length >= 0 );
    for ( unsigned int i = 0; i < length; ++i )
    {
        result[i] = rgb_from_hsl( color[i] );
    }
}

void ctransform( int dispatch, math::vec3* result, const char* fromspace, const math::vec3* color, unsigned int length )
{
	if ( dispatch == DISPATCH_U3 )
	{
		length = 1;
	}

	if ( strcmp(fromspace, "hsv") == 0 )
	{
		ctransform_from_hsv_v3( result, color, length );
	}
	else
	{
		REYES_ASSERT( strcmp(fromspace, "hsl") == 0 );
		ctransform_from_hsl_v3( result, color, length );
	}
}

}
