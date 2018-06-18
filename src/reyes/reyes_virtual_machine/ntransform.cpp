//
// ntransform.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ntransform.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>
#include <math/mat3x3.ipp>
#include <math/mat4x4.ipp>
#include <math/vec4.ipp>
#include <math/vec3.ipp>

using namespace math;

namespace reyes
{

void ntransform_v3( math::vec3* results, const math::mat4x4& mm, const math::vec3* normals, unsigned int length )
{
    mat3x3 m( transpose(inverse(mat3x3(mm))) );
    for ( unsigned int i = 0; i < length; ++i )
    {
        results[i] = m * normals[i];
    }
}

void ntransform( int dispatch, math::vec3* results, const math::mat4x4& m, const math::vec3* normals, unsigned int length )
{
	switch ( dispatch )
	{
		case DISPATCH_U3:
			ntransform_v3( results, m, normals, 1 );
			break;

		case DISPATCH_V3:
			ntransform_v3( results, m, normals, length );
			break;

		default:
			break;
	}
}

}
