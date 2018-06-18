#ifndef REYES_VTRANSFORM_HPP_INCLUDED
#define REYES_VTRANSFORM_HPP_INCLUDED

namespace math
{

struct vec3;
struct mat4x4;

}

namespace reyes
{

void vtransform( int dispatch, math::vec3* results, const math::mat4x4& m, const math::vec3* points, unsigned int length );

}

#endif
