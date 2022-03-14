//
// CubicPatch.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "CubicPatch.hpp"
#include "Grid.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using std::min;
using std::max;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

CubicPatch::CubicPatch( const math::vec3* p, const math::vec4* u_basis, const math::vec4* v_basis )
: Geometry(vec2(0.0f, 1.0f), vec2(0.0f, 1.0f))
, p_( p )
, u_basis_( u_basis )
, v_basis_( v_basis )
{
    REYES_ASSERT( p_ );
    REYES_ASSERT( u_basis_ );
    REYES_ASSERT( v_basis_ );
}

CubicPatch::CubicPatch( const CubicPatch& patch, const math::vec2& u_range, const math::vec2& v_range )
: Geometry(u_range, v_range)
, p_( patch.p_ )
, u_basis_( patch.u_basis_ )
, v_basis_( patch.v_basis_ )
{
}

bool CubicPatch::boundable() const
{
    return true;
}

void CubicPatch::bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum, Grid* grid ) const
{
    REYES_ASSERT( minimum );
    REYES_ASSERT( maximum );
    REYES_ASSERT( grid );
    
    *minimum = vec3( FLT_MAX, FLT_MAX, FLT_MAX );
    *maximum = vec3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
    
    dice( transform, 8, 8, grid );
    const vec3* positions = grid->vec3_value( "P" );
    const vec3* positions_end = positions + grid->size();
    for ( const vec3* i = positions; i != positions_end; ++i )
    {
        minimum->x = min( minimum->x, i->x );
        minimum->y = min( minimum->y, i->y );
        minimum->z = min( minimum->z, i->z );        
        maximum->x = max( maximum->x, i->x );
        maximum->y = max( maximum->y, i->y );
        maximum->z = max( maximum->z, i->z );
    }
}

bool CubicPatch::splittable() const
{
    return true;
}

void CubicPatch::split( std::list<std::shared_ptr<Geometry> >* primitives ) const
{
    REYES_ASSERT( primitives );
    REYES_ASSERT( u_range().y >= u_range().x );
    REYES_ASSERT( v_range().y >= v_range().x );

    const vec2& u_range = Geometry::u_range();
    float u0 = u_range.x;
    float u1 = (u_range.x + u_range.y) / 2.0f;
    float u2 = u_range.y;

    const vec2& v_range = Geometry::v_range();
    float v0 = v_range.x;
    float v1 = (v_range.x + v_range.y) / 2.0f;
    float v2 = v_range.y;

    shared_ptr<Geometry> cubic_patch;
    cubic_patch.reset( new CubicPatch(*this, vec2(u0, u1), vec2(v0, v1)) );
    primitives->push_back( cubic_patch );
    cubic_patch.reset( new CubicPatch(*this, vec2(u0, u1), vec2(v1, v2)) );
    primitives->push_back( cubic_patch );
    cubic_patch.reset( new CubicPatch(*this, vec2(u1, u2), vec2(v0, v1)) );
    primitives->push_back( cubic_patch );
    cubic_patch.reset( new CubicPatch(*this, vec2(u1, u2), vec2(v1, v2)) );
    primitives->push_back( cubic_patch );
}

bool CubicPatch::diceable() const
{
    return true;
}

void CubicPatch::dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const
{
    REYES_ASSERT( width > 0 );
    REYES_ASSERT( height > 0 );
    REYES_ASSERT( grid );
    
    const vec2& u_range = Geometry::u_range();
    const vec2& v_range = Geometry::v_range();

    grid->resize( width, height );
    grid->set_du( (u_range.y - u_range.x) / float(width - 1) );
    grid->set_dv( (v_range.y - v_range.x) / float(height - 1) );
    
    vec3* positions = grid->vec3_value( "P" );
    float* s = grid->float_value( "s" );
    float* t = grid->float_value( "t" );
    
    int vertex = 0;
    float v = v_range.x;
    float dv = (v_range.y - v_range.x) / float(height - 1);
    for ( int j = 0; j < height; ++j )
    {
        float u = u_range.x;
        float du = (u_range.y - u_range.x) / float(width - 1);
        for ( int i = 0; i < width; ++i )
        {
            positions[vertex] = vec3( transform * vec4(position(u, v), 1.0f) );
            s[vertex] = u;
            t[vertex] = v;
            ++vertex;
            u = min( u + du, u_range.y );
        }        
        v = min( v + dv, v_range.y );
    }
}

math::vec3 CubicPatch::position( float u, float v ) const
{
    REYES_ASSERT( u >= 0.0f && u <= 1.0f );
    REYES_ASSERT( v >= 0.0f && v <= 1.0f );

    vec4 uu( u * u * u, u * u, u, 1.0f );
    float b0_u = dot( u_basis_[0], uu );
    float b1_u = dot( u_basis_[1], uu );
    float b2_u = dot( u_basis_[2], uu );
    float b3_u = dot( u_basis_[3], uu );
    
    vec4 vv( v * v * v, v * v, v, 1.0f );
    float b0_v = dot( v_basis_[0], vv );
    float b1_v = dot( v_basis_[1], vv );
    float b2_v = dot( v_basis_[2], vv );
    float b3_v = dot( v_basis_[3], vv );

    return 
        b0_u * b0_v * p_[0] +
        b1_u * b0_v * p_[1] +
        b2_u * b0_v * p_[2] +
        b3_u * b0_v * p_[3] +
        b0_u * b1_v * p_[4] +
        b1_u * b1_v * p_[5] +
        b2_u * b1_v * p_[6] +
        b3_u * b1_v * p_[7] +
        b0_u * b2_v * p_[8] +
        b1_u * b2_v * p_[9] +
        b2_u * b2_v * p_[10] +
        b3_u * b2_v * p_[11] +
        b0_u * b3_v * p_[12] +
        b1_u * b3_v * p_[13] +
        b2_u * b3_v * p_[14] +
        b3_u * b3_v * p_[15]
    ;
}

math::vec3 CubicPatch::normal( float u, float v ) const
{
    REYES_ASSERT( u >= 0.0f && u <= 1.0f );
    REYES_ASSERT( v >= 0.0f && v <= 1.0f );
    return vec3( 0.0f, 0.0f, -1.0f );
}
