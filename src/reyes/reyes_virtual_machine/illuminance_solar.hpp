#pragma once

namespace math
{

struct vec3;

}

namespace reyes
{

void illuminance_solar( int dispatch, int* result, const math::vec3* light_position, const math::vec3* axis, const float* angle, int length );

}
