#pragma once

namespace math
{

struct mat4x4;

}

namespace reyes
{

void mtransform( int dispatch, math::mat4x4* results, const math::mat4x4& m, const math::mat4x4* matrices, unsigned int length );

}
