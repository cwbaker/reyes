#pragma once

namespace math
{

struct vec3;

}

namespace reyes
{

void illuminance_illuminate( int dispatch, int* result, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length );

}
