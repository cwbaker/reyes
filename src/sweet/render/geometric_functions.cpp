//
// geometric_functions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "geometric_functions.hpp"
#include "mathematical_functions.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include "Renderer.hpp"
#include "Attributes.hpp"
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

using std::max;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;

namespace sweet
{

namespace render
{

void xcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p );
    
    result->reset( TYPE_FLOAT, p->storage(), p->size() );
    
    const int size = p->size();
    float* values = result->float_values();
    const vec3* p_values = p->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = p_values[i].x;
    }
}

void ycomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p );
    
    result->reset( TYPE_FLOAT, p->storage(), p->size() );
    
    const int size = p->size();
    float* values = result->float_values();
    const vec3* p_values = p->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = p_values[i].y;
    }
}

void zcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p );
    
    result->reset( TYPE_FLOAT, p->storage(), p->size() );
    
    const int size = p->size();
    float* values = result->float_values();
    const vec3* p_values = p->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = p_values[i].z;
    }
}

void setxcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> p, std::shared_ptr<Value> x )
{
    SWEET_ASSERT( p );
    SWEET_ASSERT( x );
    SWEET_ASSERT( p->size() == x->size() );

    const int size = p->size();
    vec3* values = p->vec3_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i].x = x_values[i];
    }
}

void setycomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> p, std::shared_ptr<Value> y )
{
    SWEET_ASSERT( p );
    SWEET_ASSERT( y );
    SWEET_ASSERT( p->size() == y->size() );

    const int size = p->size();
    vec3* values = p->vec3_values();
    const float* y_values = y->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i].y = y_values[i];
    }
}

void setzcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> p, std::shared_ptr<Value> z )
{
    SWEET_ASSERT( p );
    SWEET_ASSERT( z );
    SWEET_ASSERT( p->size() == z->size() );

    const int size = p->size();
    vec3* values = p->vec3_values();
    const float* z_values = z->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i].z = z_values[i];
    }
}

void length( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( x );
    
    result->reset( TYPE_FLOAT, x->storage(), x->size() );
    
    const int size = x->size();
    float* values = result->float_values();
    const vec3* x_values = x->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = length( x_values[i] );
    }
}

void normalize( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( n );
    
    result->reset( n->type(), n->storage(), n->size() );

    switch ( result->type() )
    {
        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
        {
            const vec3* n_values = n->vec3_values();
            vec3* values = result->vec3_values();
            for ( int i = 0; i < n->size(); ++i )
            {
                values[i] = length( n_values[i] ) > 0.0f ? math::normalize( n_values[i] ) : n_values[i];
            }
            break;
        }
            
        default:
            SWEET_ASSERT( false );
            break;
    }    
}

void distance( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p0, std::shared_ptr<Value> p1 )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p0 );
    SWEET_ASSERT( p1 );
    SWEET_ASSERT( p0->size() == p1->size() );
    
    result->reset( TYPE_FLOAT, p0->storage(), p0->size() );
    
    const int size = p0->size();
    float* values = result->float_values();
    const vec3* p0_values = p0->vec3_values();
    const vec3* p1_values = p1->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = length( p1_values[i] - p0_values[i] );
    }
}

void rotate( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> q, std::shared_ptr<Value> angle, std::shared_ptr<Value> p0, std::shared_ptr<Value> p1 )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( q );
    SWEET_ASSERT( angle );
    SWEET_ASSERT( p0 );
    SWEET_ASSERT( p1 );
    SWEET_ASSERT( angle->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( p0->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( p1->storage() == STORAGE_UNIFORM );
    
    result->reset( q->type(), q->storage(), q->size() );
    
    const mat4x4 rotation = math::rotate( normalize(p1->vec3_value() - p0->vec3_value()), angle->float_value() );    
    const int size = q->size();
    vec3* values = result->vec3_values();
    const vec3* q_values = q->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = vec3( rotation * vec4(q_values[i], 0.0f) );
    }    
}

void area( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p );
    
    result->reset( TYPE_FLOAT, p->storage(), p->size() );
    
    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    float du = grid.du_;
    const vec3* other_values = p->vec3_values();    
    float* values = result->float_values();
    
    for ( int y = 0; y < height - 1; ++y )
    {
        for ( int x = 0; x < width - 1; ++x )
        {
            int i0 = i + x;
            int i1 = i + x + 1;
            int i3 = i + x + width;
            values[i0] = length(other_values[i1] - other_values[i0]) * length(other_values[i3] - other_values[i0]);
        }
        values[i + width - 1] = values[i + width - 2];
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        values[i + x] = values[i - width + x];
    }
}

void faceforward_vv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n, std::shared_ptr<Value> i )
{
    faceforward_vvv( renderer, grid, result, n, i, n );
}

void faceforward_vvv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n, std::shared_ptr<Value> i, std::shared_ptr<Value> nref )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( n );
    SWEET_ASSERT( i );
    SWEET_ASSERT( nref );
    SWEET_ASSERT( n->size() == i->size() );
    SWEET_ASSERT( n->size() == nref->size() );

    result->reset( n->type(), n->storage(), n->size() );

    switch ( result->type() )
    {
        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
        {
            const vec3* nref_values = nref->vec3_values();
            const vec3* n_values = n->vec3_values();
            const vec3* i_values = i->vec3_values();
            vec3* values = result->vec3_values();
            for ( int i = 0; i < n->size(); ++i )
            {
                values[i] = math::sign( dot(-i_values[i], nref_values[i]) ) * n_values[i];
            }
            break;
        }
            
        default:
            SWEET_ASSERT( false );
            break;
    }    
}

void reflect( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> i, std::shared_ptr<Value> n )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( i );
    SWEET_ASSERT( n );
    SWEET_ASSERT( n->size() == i->size() );

    result->reset( TYPE_VECTOR, n->storage(), n->size() );

    const vec3* n_values = n->vec3_values();
    const vec3* i_values = i->vec3_values();
    vec3* values = result->vec3_values();
    for ( int i = 0; i < n->size(); ++i )
    {
        values[i] = i_values[i] - 2.0f * dot(i_values[i], n_values[i]) * n_values[i];
    }
}

void refract( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> incident, std::shared_ptr<Value> normal, std::shared_ptr<Value> eta_value )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( incident );
    SWEET_ASSERT( normal );
    SWEET_ASSERT( eta_value );
    SWEET_ASSERT( eta_value->storage() == STORAGE_UNIFORM );
    
    result->reset( TYPE_VECTOR, STORAGE_VARYING, grid.size() );
    
    const vec3* normals = normal->vec3_values();
    const vec3* incidents = incident->vec3_values();
    const float eta = eta_value->float_value();
    const int size = grid.size();
    vec3* values = result->vec3_values();
    
    for ( int i = 0; i < size; ++i )
    {
        const vec3& I = incidents[i];
        const vec3& N = normals[i];
        const float idotn = dot( I, N );
        const float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
        values[i] = k > 0.0f ? eta * I - (eta * idotn + sqrtf(k)) * N : vec3( 0.0f, 0.0f, 0.0f );
    }
}

void fresnel( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> incident, std::shared_ptr<Value> normal, std::shared_ptr<Value> eta_value, std::shared_ptr<Value> Kr, std::shared_ptr<Value> Kt )
{
    SWEET_ASSERT( incident );
    SWEET_ASSERT( normal );
    SWEET_ASSERT( eta_value );
    SWEET_ASSERT( Kr );
    SWEET_ASSERT( Kt );
    
    Kr->reset( TYPE_FLOAT, STORAGE_VARYING, grid.size() );
    Kt->reset( TYPE_FLOAT, STORAGE_VARYING, grid.size() );
    
    const int size = grid.size();
    float* Krs = Kr->float_values();
    float* Kts = Kt->float_values();
    const float eta = eta_value->float_value();
    
    if ( eta >= 1.0f )
    {
        const vec3* normals = normal->vec3_values();
        const vec3* incidents = incident->vec3_values();
        
        for ( int i = 0; i < size; ++i )
        {
            const vec3& I = incidents[i];
            const vec3& N = normals[i];
            const float c = max( 0.0f, dot(I, N) );
            const float g = sqrtf( eta * eta + c * c - 1.0f );
            const float m = g - c;
            const float n = g + c;
            const float o = c * n - 1.0f;
            const float p = c * m + 1.0f;
            Krs[i] = 0.5f * (m * m / n * n) * (1.0f + (o * o / p * p));
            Kts[i] = 1.0f - Krs[i];
        }
    }
    else
    {
        for ( int i = 0; i < size; ++i )
        {
            Krs[i] = 1.0f;
            Kts[i] = 0.0f;
        }
    }
}

void transform_sv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( tospace );
    SWEET_ASSERT( tospace->type() == TYPE_STRING );
    SWEET_ASSERT( p );
    
    const int size = p->size();
    result->reset( TYPE_POINT, STORAGE_VARYING, size );
    result->zero();
    
    const math::mat4x4 transform = renderer.transform_to( tospace->string_value() );
    const vec3* p_values = p->vec3_values();
    vec3* return_values = result->vec3_values();
    for ( unsigned int i = 0; i < size; ++i )
    {
        return_values[i] = vec3( transform * vec4(p_values[i], 1.0f) );
    }
}

void transform_ssv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( tospace );
    SWEET_ASSERT( tospace->type() == TYPE_STRING );
    SWEET_ASSERT( p );
    
    const int size = p->size();
    result->reset( TYPE_POINT, STORAGE_VARYING, size );
    result->zero();
    
    const math::mat4x4 transform = renderer.transform_between( fromspace->string_value(), tospace->string_value() );
    const vec3* p_values = p->vec3_values();
    vec3* return_values = result->vec3_values();
    for ( unsigned int i = 0; i < size; ++i )
    {
        return_values[i] = vec3( transform * vec4(p_values[i], 1.0f) );
    }
}

void transform_mv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> m, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( m );
    SWEET_ASSERT( m->type() == TYPE_MATRIX );
    SWEET_ASSERT( p );
    SWEET_ASSERT( p->type() == TYPE_POINT || p->type() == TYPE_VECTOR || p->type() == TYPE_NORMAL );
    
    result->transform( m->mat4x4_values()[0], p );
}

void transform_smv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> m, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( m );
    SWEET_ASSERT( m->type() == TYPE_MATRIX );
    SWEET_ASSERT( p );
    
    const int size = p->size();
    result->reset( TYPE_POINT, STORAGE_VARYING, size );
    result->zero();
    
    const math::mat4x4& to = m->mat4x4_values()[0];
    const math::mat4x4 transform = to * renderer.transform_from( fromspace->string_value() );
    const vec3* p_values = p->vec3_values();
    vec3* return_values = result->vec3_values();
    for ( unsigned int i = 0; i < size; ++i )
    {
        return_values[i] = vec3( transform * vec4(p_values[i], 1.0f) );
    }
}

void vtransform_sv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( tospace );
    SWEET_ASSERT( tospace->type() == TYPE_STRING );
    SWEET_ASSERT( p );
    
    result->vtransform( renderer.transform_to(tospace->string_value()), p );
}

void vtransform_ssv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( tospace );
    SWEET_ASSERT( tospace->type() == TYPE_STRING );
    SWEET_ASSERT( p );
    
    result->vtransform( renderer.transform_between(fromspace->string_value(), tospace->string_value()), p );
}

void vtransform_mv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> m, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( m );
    SWEET_ASSERT( m->type() == TYPE_MATRIX );
    SWEET_ASSERT( m->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( p );
    
    result->vtransform( m->mat4x4_value(), p );
}

void vtransform_smv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> m, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( m );
    SWEET_ASSERT( m->type() == TYPE_MATRIX );
    SWEET_ASSERT( m->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( p );
    
    result->vtransform( m->mat4x4_value() * renderer.transform_from(fromspace->string_value()), p );
}

void ntransform_sv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( tospace );
    SWEET_ASSERT( tospace->type() == TYPE_STRING );
    SWEET_ASSERT( p );
    
    result->ntransform( renderer.transform_to(tospace->string_value()), p );
}

void ntransform_ssv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( tospace );
    SWEET_ASSERT( tospace->type() == TYPE_STRING );
    SWEET_ASSERT( p );
    
    result->ntransform( renderer.transform_between(fromspace->string_value(), tospace->string_value()), p );
}

void ntransform_mv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> m, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( m );
    SWEET_ASSERT( m->type() == TYPE_MATRIX );
    SWEET_ASSERT( m->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( p );
    
    result->ntransform( m->mat4x4_value(), p );
}

void ntransform_smv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> m, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( m );
    SWEET_ASSERT( m->type() == TYPE_MATRIX );
    SWEET_ASSERT( m->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( p );
    
    result->ntransform( m->mat4x4_value() * renderer.transform_from(fromspace->string_value()), p );
}

void depth( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p );
    
    result->reset( TYPE_FLOAT, p->storage(), p->size() );

    const int size = result->size();
    const vec3* positions = p->vec3_values();
    float* values = result->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = renderer.raster(positions[i]).z; 
    }
}

void calculatenormal( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( p );
    
    shared_ptr<Value> dpdu( new Value() );
    du_vec3( renderer, grid, dpdu, p );

    shared_ptr<Value> dpdv( new Value() );
    dv_vec3( renderer, grid, dpdv, p );
    
    result->reset( p->type(), p->storage(), p->size() );
    
    const int size = result->size();
    const vec3* dpdu_values = dpdu->vec3_values();
    const vec3* dpdv_values = dpdv->vec3_values();
    vec3* values = result->vec3_values();
    
    if ( renderer.attributes().geometry_left_handed() )
    {
        for ( unsigned int i = 0; i < size; ++i )
        {
            values[i] = cross( dpdu_values[i], dpdv_values[i] );
        }
    }
    else
    {
        for ( unsigned int i = 0; i < size; ++i )
        {
            values[i] = cross( dpdv_values[i], dpdu_values[i] );
        }
    }
}

}

}
