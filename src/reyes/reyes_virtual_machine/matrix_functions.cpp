
#include "matrix_functions.hpp"
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/assert.hpp>
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>

using namespace math;

namespace reyes
{

void comp_matrix( const Renderer& /*renderer*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const int row = int( *reinterpret_cast<const float*>(arguments[2]) );
    const int column = int( *reinterpret_cast<const float*>(arguments[3]) );
    result[0] = m.m[row * 4 + column];
}

void setcomp_matrix( const Renderer& /*renderer*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    mat4x4* matrix = reinterpret_cast<mat4x4*>( arguments[1] );
    const int row = int( *reinterpret_cast<const float*>(arguments[2]) );
    const int column = int( *reinterpret_cast<const float*>(arguments[3]) );
    const float* value = reinterpret_cast<const float*>( arguments[4] );
    matrix[0].m[row * 4 + column] = value[0];
}

void determinant( const Renderer& /*render*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    result[0] = math::determinant( m );
}

void translate_matrix( const Renderer& /*render*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    mat4x4* result = reinterpret_cast<mat4x4*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const vec3& translation = *reinterpret_cast<const vec3*>( arguments[2] );
    result[0] = m * math::translate( translation );
}

void rotate_matrix( const Renderer& /*render*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    mat4x4* result = reinterpret_cast<mat4x4*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const float angle = *reinterpret_cast<const float*>( arguments[2] );
    const vec3& axis = *reinterpret_cast<const vec3*>( arguments[3] );
    result[0] = m * math::rotate( axis, angle );
}

void scale_matrix( const Renderer& /*render*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    mat4x4* result = reinterpret_cast<mat4x4*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const vec3& scale = *reinterpret_cast<const vec3*>( arguments[2] );
    result[0] = m * math::translate( scale );
}

}
