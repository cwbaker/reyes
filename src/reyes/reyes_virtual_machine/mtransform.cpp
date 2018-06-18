//
// mtransform.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "mtransform.hpp"
#include "Dispatch.hpp"
#include <reyes/Instruction.hpp>
#include <reyes/assert.hpp>
#include <math/mat4x4.ipp>

using namespace math;

namespace reyes
{

void mtransform_v16( math::mat4x4* results, const math::mat4x4& m, const math::mat4x4* matrices, unsigned int length )
{
    for ( unsigned int i = 0; i < length; ++i )
    {
        results[i] = m * matrices[i];
    }
}

void mtransform( int dispatch, math::mat4x4* results, const math::mat4x4& m, const math::mat4x4* matrices, unsigned int length )
{
	switch ( dispatch )
	{
		case DISPATCH_U16:
			mtransform_v16( results, m, matrices, 1 );
			break;

		case DISPATCH_V16:
			mtransform_v16( results, m, matrices, length );
			break;

		default:
			break;
	}
}

}
