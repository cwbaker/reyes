#ifndef REYES_TRANSFORM_HPP_INCLUDED
#define REYES_TRANSFORM_HPP_INCLUDED

namespace math
{

struct vec3;
struct mat4x4;

}

namespace reyes
{

void transform( int dispatch, math::vec3* results, const math::mat4x4& m, const math::vec3* points, unsigned int length );

}

#endif
