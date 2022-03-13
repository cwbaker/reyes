//
// geometric_functions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "geometric_functions.hpp"
#include "mathematical_functions.hpp"
#include "transform.hpp"
#include "vtransform.hpp"
#include "ntransform.hpp"
#include "Dispatch.hpp"
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/Attributes.hpp>
#include <reyes/assert.hpp>
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <algorithm>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::max;
using std::string;
using std::vector;
using namespace math;

namespace reyes
{

void xcomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* x = reinterpret_cast<const vec3*>( arguments[1] );    
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = x[i].x;
    }
}

void ycomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* x = reinterpret_cast<const vec3*>( arguments[1] );    
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = x[i].y;
    }
}

void zcomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* x = reinterpret_cast<const vec3*>( arguments[1] );    
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = x[i].z;
    }
}

void setxcomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i].x = x[i];
    }
}

void setycomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const float* y = reinterpret_cast<const float*>( arguments[1] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i].y = y[i];
    }
}

void setzcomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const float* z = reinterpret_cast<const float*>( arguments[1] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i].z = z[i];
    }
}

void length( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* x = reinterpret_cast<const vec3*>( arguments[1] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = length( x[i] );
    }
}

void normalize( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* x = reinterpret_cast<const vec3*>( arguments[1] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = length( x[i] ) > 0.0f ? math::normalize( x[i] ) : x[i];
    }
}

void distance( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* p0 = reinterpret_cast<const vec3*>( arguments[1] ); 
    const vec3* p1 = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = length( p1[i] - p0[i] );
    }
}

void rotate( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* q = reinterpret_cast<const vec3*>( arguments[1] );
    const float angle = *reinterpret_cast<const float*>( arguments[2] );
    const vec3 p0 = *reinterpret_cast<const vec3*>( arguments[3] );
    const vec3 p1 = *reinterpret_cast<const vec3*>( arguments[4] );    
    const mat4x4 rotation = math::rotate( normalize(p1 - p0), angle );    
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = vec3( rotation * vec4(q[i], 0.0f) );
    }    
}

void area( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[1] );
    int i = 0;
    const int width = grid.width();
    const int height = grid.height();

    for ( int y = 0; y < height - 1; ++y )
    {
        for ( int x = 0; x < width - 1; ++x )
        {
            int i0 = i + x;
            int i1 = i + x + 1;
            int i3 = i + x + width;
            result[i0] = length( p[i1] - p[i0] ) * length( p[i3] - p[i0] );
        }
        result[i + width - 1] = result[i + width - 2];
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        result[i + x] = result[i - width + x];
    }
}

void faceforward_vv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    void* forwarded_arguments [] = { arguments[0], arguments[1], arguments[2], arguments[1] };
    faceforward_vvv( renderer, grid, dispatch, forwarded_arguments );
}

void faceforward_vvv( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* n = reinterpret_cast<const vec3*>( arguments[1] );
    const vec3* ii = reinterpret_cast<const vec3*>( arguments[2] );
    const vec3* nref = reinterpret_cast<const vec3*>( arguments[3] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = math::sign( dot(-ii[i], nref[i]) ) * n[i];
    }
}

void reflect( const Renderer& /*render*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* ii = reinterpret_cast<const vec3*>( arguments[1] );
    const vec3* n = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = ii[i] - 2.0f * dot(ii[i], n[i]) * n[i];
    }
}

void refract( const Renderer& /*render*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* incident = reinterpret_cast<const vec3*>( arguments[1] );
    const vec3* normal = reinterpret_cast<const vec3*>( arguments[2] );
    const float eta = *reinterpret_cast<const float*>( arguments[3] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        const vec3& I = incident[i];
        const vec3& N = normal[i];
        const float idotn = dot( I, N );
        const float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
        result[i] = k > 0.0f ? eta * I - (eta * idotn + sqrtf(k)) * N : vec3( 0.0f, 0.0f, 0.0f );
    }
}

void fresnel( const Renderer& /*render*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    // vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* incident = reinterpret_cast<const vec3*>( arguments[1] );
    const vec3* normal = reinterpret_cast<const vec3*>( arguments[2] );
    const float eta = *reinterpret_cast<const float*>( arguments[3] );
    float* Kr = reinterpret_cast<float*>( arguments[4] );
    float* Kt = reinterpret_cast<float*>( arguments[5] );
    const int size = grid.size();
    
    if ( eta >= 1.0f )
    {
        for ( int i = 0; i < size; ++i )
        {
            const vec3& I = incident[i];
            const vec3& N = normal[i];
            const float c = max( 0.0f, dot(I, N) );
            const float g = sqrtf( eta * eta + c * c - 1.0f );
            const float m = g - c;
            const float n = g + c;
            const float o = c * n - 1.0f;
            const float p = c * m + 1.0f;
            Kr[i] = 0.5f * (m * m / n * n) * (1.0f + (o * o / p * p));
            Kt[i] = 1.0f - Kr[i];
        }
    }
    else
    {
        for ( int i = 0; i < size; ++i )
        {
            Kr[i] = 1.0f;
            Kt[i] = 0.0f;
        }
    }
}

void transform_sv( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* tospace = reinterpret_cast<const char*>( arguments[1] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[2] );
    const math::mat4x4 transform = renderer.transform_to( string(tospace) );
    const int size = grid.size();

    memset( result, 0, sizeof(vec3) * size );    
    for ( int i = 0; i < size; ++i )
    {
        result[i] = vec3( transform * vec4(p[i], 1.0f) );
    }
}

void transform_ssv( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* fromspace = reinterpret_cast<const char*>( arguments[1] );
    const char* tospace = reinterpret_cast<const char*>( arguments[2] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[3] );
    const math::mat4x4 transform = renderer.transform_between( string(fromspace), string(tospace) );
    const int size = grid.size();

    memset( result, 0, sizeof(vec3) * size );    
    for ( int i = 0; i < size; ++i )
    {
        result[i] = vec3( transform * vec4(p[i], 1.0f) );
    }
}

void transform_mv( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const mat4x4 m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    transform( 
        dispatch,
        result,
        m,
        p,
        size
    );
}

void transform_smv( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* fromspace = reinterpret_cast<const char*>( arguments[1] );
    const mat4x4& to = *reinterpret_cast<const mat4x4*>( arguments[2] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[3] );
    const int size = grid.size();
    const mat4x4 transform = to * renderer.transform_from( string(fromspace) );

    memset( result, 0, sizeof(vec3) * size );    
    for ( int i = 0; i < size; ++i )
    {
        result[i] = vec3( transform * vec4(p[i], 1.0f) );
    }
}

void vtransform_sv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* tospace = reinterpret_cast<const char*>( arguments[1] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    vtransform( 
        dispatch,
        result,
        renderer.transform_to( string(tospace) ),
        p,
        size
    );    
}

void vtransform_ssv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* fromspace = reinterpret_cast<const char*>( arguments[1] );
    const char* tospace = reinterpret_cast<const char*>( arguments[2] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[3] );
    const int size = grid.size();
    vtransform( 
        dispatch,
        result,
        renderer.transform_between( string(fromspace), string(tospace) ),
        p,
        size
    );
}

void vtransform_mv( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    vtransform( 
        dispatch,
        result,
        m,
        p,
        size
    );    
}

void vtransform_smv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* fromspace = reinterpret_cast<const char*>( arguments[1] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[2] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[3] );
    const int size = grid.size();
    vtransform( 
        dispatch,
        result,
        m * renderer.transform_from( string(fromspace) ),
        p,
        size
    );    
}

void ntransform_sv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* tospace = reinterpret_cast<const char*>( arguments[1] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    ntransform(
        dispatch,
        result,
        renderer.transform_to( string(tospace) ),
        p,
        size
    );
}

void ntransform_ssv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* fromspace = reinterpret_cast<const char*>( arguments[1] );
    const char* tospace = reinterpret_cast<const char*>( arguments[2] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[3] );
    const int size = grid.size();
    ntransform(
        dispatch,
        result,
        renderer.transform_between( string(fromspace), string(tospace) ),
        p,
        size
    );
}

void ntransform_mv( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[1] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[2] );
    const int size = grid.size();
    ntransform(
        dispatch,
        result,
        m,
        p,
        size
    );
}

void ntransform_smv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    REYES_ASSERT( result );
    REYES_ASSERT( fromspace );
    REYES_ASSERT( fromspace->type() == TYPE_STRING );
    REYES_ASSERT( m );
    REYES_ASSERT( m->type() == TYPE_MATRIX );
    REYES_ASSERT( m->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( p );

    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const char* fromspace = reinterpret_cast<const char*>( arguments[1] );
    const mat4x4& m = *reinterpret_cast<const mat4x4*>( arguments[2] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[3] );
    const int size = grid.size();
    ntransform(
        dispatch,
        result,
        m * renderer.transform_from( string(fromspace) ),
        p,
        size
    );
}

void depth( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[1] );
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = renderer.raster(p[i]).z; 
    }
}

void calculatenormal( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* p = reinterpret_cast<const vec3*>( arguments[1] );
    const int size = grid.size();
    void* args [2] = { nullptr, const_cast<vec3*>(p) };
    vector<vec3> dpdu( size, vec3(0.0f) );
    args[0] = dpdu.data();
    du_vec3( renderer, grid, dispatch, args );
    vector<vec3> dpdv( size, vec3(0.0f) );
    args[0] = dpdv.data();
    dv_vec3( renderer, grid, dispatch, args );
    
    if ( renderer.attributes().geometry_left_handed() )
    {
        for ( int i = 0; i < size; ++i )
        {
            result[i] = cross( dpdu[i], dpdv[i] );
        }
    }
    else
    {
        for ( int i = 0; i < size; ++i )
        {
            result[i] = cross( dpdv[i], dpdu[i] );
        }
    }
}

}
