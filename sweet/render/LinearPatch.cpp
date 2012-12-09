//
// LinearPatch.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "LinearPatch.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/math/mat4x4.ipp>
#include <sweet/math/scalar.ipp>
#include <sweet/assert/assert.hpp>
#include <vector>
#include <memory.h>
#define _USE_MATH_DEFINES
#include <math.h>

using std::min;
using std::max;
using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

LinearPatch::LinearPatch( const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates )
: Geometry(vec2(0.0f, 1.0f), vec2(0.0f, 1.0f))
  //positions_(),
  //normals_(),
  //texture_coordinates_()
{
    SWEET_ASSERT( positions );
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

void LinearPatch::bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum ) const
{
    SWEET_ASSERT( minimum );
    SWEET_ASSERT( maximum );
    
    *minimum = vec3( FLT_MAX, FLT_MAX, FLT_MAX );
    *maximum = vec3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
    
    Grid grid;
    dice( transform, 8, 8, &grid );
    const vec3* positions = grid["P"].vec3_values();
    const vec3* positions_end = positions + grid["P"].size();
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

void LinearPatch::split( std::list<ptr<Geometry> >* primitives ) const
{
    SWEET_ASSERT( primitives );
    SWEET_ASSERT( u_range().y >= u_range().x );
    SWEET_ASSERT( v_range().y >= v_range().x );

    const vec2& u_range = Geometry::u_range();
    float u0 = u_range.x;
    float u1 = (u_range.x + u_range.y) / 2.0f;
    float u2 = u_range.y;

    const vec2& v_range = Geometry::v_range();
    float v0 = v_range.x;
    float v1 = (v_range.x + v_range.y) / 2.0f;
    float v2 = v_range.y;

    ptr<Geometry> linear_patch;
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
    SWEET_ASSERT( width > 0 );
    SWEET_ASSERT( height > 0 );
    SWEET_ASSERT( grid );
    
    const vec2& u_range = Geometry::u_range();
    const vec2& v_range = Geometry::v_range();

    grid->resize( width, height );
    grid->du_ = (u_range.y - u_range.x) / float(width - 1);
    grid->dv_ = (v_range.y - v_range.x) / float(height - 1);
    
    vec3* positions = grid->value( "P", TYPE_POINT ).vec3_values();
    vec3* normals = grid->value( "N", TYPE_NORMAL ).vec3_values();
    float* s = grid->value( "s", TYPE_FLOAT ).float_values();
    float* t = grid->value( "t", TYPE_FLOAT ).float_values();
    
    int vertex = 0;
    float v = v_range.x;
    float dv = (v_range.y - v_range.x) / float(height - 1);
    for ( int j = 0; j < height; ++j )
    {
        float u = u_range.x;
        float du = (u_range.y - u_range.x) / float(width - 1);
        for ( int i = 0; i < width; ++i )
        {
            positions[vertex] = transform * vec4( bilerp(positions_, u, v), 1.0f );
            normals[vertex] = transform * vec4( bilerp(normals_, u, v), 1.0f );
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
    SWEET_ASSERT( x );
    SWEET_ASSERT( u >= 0.0f && u <= 1.0f );
    SWEET_ASSERT( v >= 0.0f && v <= 1.0f );

    return lerp(
        lerp( x[0], x[1], u ),
        lerp( x[3], x[2], u ),
        v
    );
}

math::vec2 LinearPatch::bilerp( const math::vec2* x, float u, float v ) const
{
    SWEET_ASSERT( x );
    SWEET_ASSERT( u >= 0.0f && u <= 1.0f );
    SWEET_ASSERT( v >= 0.0f && v <= 1.0f );

    return lerp(
        lerp( x[0], x[1], u ),
        lerp( x[3], x[2], u ),
        v
    );
}
