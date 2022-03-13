//
// LinearPatch.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "LinearPatch.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include "assert.hpp"
#include <vector>
#include <memory.h>
#define _USE_MATH_DEFINES
#include <math.h>

using std::min;
using std::max;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

LinearPatch::LinearPatch( const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates )
: Geometry(vec2(0.0f, 1.0f), vec2(0.0f, 1.0f))
  //positions_(),
  //normals_(),
  //texture_coordinates_()
{
    REYES_ASSERT( positions );
    memcpy( positions_, positions, sizeof(positions_) );
    memcpy( normals_, normals, sizeof(normals_) );
    memcpy( texture_coordinates_, texture_coordinates, sizeof(texture_coordinates_) );
}

LinearPatch::LinearPatch( const LinearPatch& patch, const math::vec2& u_range, const math::vec2& v_range )
: Geometry(u_range, v_range)
  //positions_(),
  //normals_(),
  //texture_coordinates_()
{
    memcpy( positions_, patch.positions_, sizeof(positions_) );
    memcpy( normals_, patch.normals_, sizeof(normals_) );
    memcpy( texture_coordinates_, patch.texture_coordinates_, sizeof(texture_coordinates_) );
}

bool LinearPatch::boundable() const
{
    return true;
}

void LinearPatch::bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum, Grid* grid ) const
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

bool LinearPatch::splittable() const
{
    return true;
}

void LinearPatch::split( std::list<std::shared_ptr<Geometry>>* primitives ) const
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

    shared_ptr<Geometry> linear_patch;
    linear_patch.reset( new LinearPatch(*this, vec2(u0, u1), vec2(v0, v1)) );
    primitives->push_back( linear_patch );
    linear_patch.reset( new LinearPatch(*this, vec2(u0, u1), vec2(v1, v2)) );
    primitives->push_back( linear_patch );
    linear_patch.reset( new LinearPatch(*this, vec2(u1, u2), vec2(v0, v1)) );
    primitives->push_back( linear_patch );
    linear_patch.reset( new LinearPatch(*this, vec2(u1, u2), vec2(v1, v2)) );
    primitives->push_back( linear_patch );
}

bool LinearPatch::diceable() const
{
    return true;
}

void LinearPatch::dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const
{
    REYES_ASSERT( width > 0 );
    REYES_ASSERT( height > 0 );
    REYES_ASSERT( grid );
    
    const vec2& u_range = Geometry::u_range();
    const vec2& v_range = Geometry::v_range();

    grid->resize( width, height );
    grid->set_du( (u_range.y - u_range.x) / float(width - 1) );
    grid->set_dv( (v_range.y - v_range.x) / float(height - 1) );
    grid->set_normals_generated( true );
    
    vec3* positions = grid->vec3_value( "P" );
    vec3* normals = grid->vec3_value( "N" );
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
            positions[vertex] = vec3( transform * vec4(bilerp(positions_, u, v), 1.0f) );
            normals[vertex] = vec3( transform * vec4(bilerp(normals_, u, v), 1.0f) );
            vec2 st = bilerp( texture_coordinates_, u, v );
            s[vertex] = st.x;
            t[vertex] = st.y;
            ++vertex;
            u = min( u + du, u_range.y );
        }
        v = min( v + dv, v_range.y );
    }
}

math::vec3 LinearPatch::bilerp( const math::vec3* x, float u, float v ) const
{
    REYES_ASSERT( x );
    REYES_ASSERT( u >= 0.0f && u <= 1.0f );
    REYES_ASSERT( v >= 0.0f && v <= 1.0f );

    return lerp(
        lerp( x[0], x[1], u ),
        lerp( x[3], x[2], u ),
        v
    );
}

math::vec2 LinearPatch::bilerp( const math::vec2* x, float u, float v ) const
{
    REYES_ASSERT( x );
    REYES_ASSERT( u >= 0.0f && u <= 1.0f );
    REYES_ASSERT( v >= 0.0f && v <= 1.0f );

    return lerp(
        lerp( x[0], x[1], u ),
        lerp( x[3], x[2], u ),
        v
    );
}
