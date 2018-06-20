//
// transform.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "transform.hpp"
#include "Dispatch.hpp"
#include "Instruction.hpp"
#include <reyes/assert.hpp>
#include <math/mat4x4.ipp>
#include <math/vec4.ipp>
#include <math/vec3.ipp>

using namespace math;

namespace reyes
{

void transform_v3( math::vec3* results, const math::mat4x4& m, const math::vec3* points, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        results[i] = vec3( m * vec4(points[i], 1.0f) );
    }
}

void transform( int dispatch, math::vec3* results, const math::mat4x4& m, const math::vec3* points, unsigned int length )
{
	switch ( dispatch )
	{
		case DISPATCH_U3:
			transform_v3( results, m, points, 1 );
			break;

		case DISPATCH_V3:
			transform_v3( results, m, points, length );
			break;

		default:
			break;
	}
}

}
