#ifndef REYES_CTRANSFORM_HPP_INCLUDED
#define REYES_CTRANSFORM_HPP_INCLUDED

namespace math
{

struct vec3;

}

namespace reyes
{

void ctransform( int dispatch, math::vec3* result, const char* fromspace, const math::vec3* color, unsigned int length );

}

#endif
