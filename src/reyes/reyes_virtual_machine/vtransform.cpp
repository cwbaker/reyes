//
// vtransform.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "vtransform.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>
#include <math/mat4x4.ipp>
#include <math/vec4.ipp>
#include <math/vec3.ipp>

using namespace math;

namespace reyes
{

void vtransform_v3( math::vec3* results, const math::mat4x4& m, const math::vec3* vectors, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        results[i] = vec3( m * vec4(vectors[i], 0.0f) );
    }
}

void vtransform( int dispatch, math::vec3* results, const math::mat4x4& m, const math::vec3* vectors, unsigned int length )
{
	switch ( dispatch )
	{
		case DISPATCH_U3:
			vtransform_v3( results, m, vectors, 1 );
			break;

		case DISPATCH_V3:
			vtransform_v3( results, m, vectors, length );
			break;

		default:
			break;
	}
}

}
