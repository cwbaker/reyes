//
// Disk.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Disk.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using std::min;
using std::max;
using std::list;
using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

Disk::Disk( float height, float radius, float thetamax )
: Geometry(vec2(0.0f, 1.0f), vec2(0.0f, 1.0f)),
  height_( height ),
  radius_( radius ),
  thetamax_( thetamax )
{   
}

Disk::Disk( const Disk& disk, const math::vec2& u_range, const math::vec2& v_range )
: Geometry(u_range, v_range),
  height_( disk.height_ ),
  radius_( disk.radius_ ),
  thetamax_( disk.thetamax_ )
{   
}

bool Disk::boundable() const
{
    return true;
}

void Disk::bound( const math::mat4x4& transform, vec3* minimum, vec3* maximum ) const
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

bool Disk::splittable() const
{
    return true;
}

void Disk::split( std::list<ptr<Geometry> >* primitives ) const
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

    ptr<Geometry> disk;
    disk.reset( new Disk(*this, vec2(u0, u1), vec2(v0, v1)) );
    primitives->push_back( disk );
    disk.reset( new Disk(*this, vec2(u0, u1), vec2(v1, v2)) );
    primitives->push_back( disk );
    disk.reset( new Disk(*this, vec2(u1, u2), vec2(v0, v1)) );
    primitives->push_back( disk );
    disk.reset( new Disk(*this, vec2(u1, u2), vec2(v1, v2)) );
    primitives->push_back( disk );
}

bool Disk::diceable() const
{
    return true;
}

void Disk::dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const
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
            positions[vertex] = transform * vec4( position(u, v), 1.0f );
            s[vertex] = u;
            t[vertex] = v;
            u = min( u + du, u_range.y );
            ++vertex;
        }        
        v = min( v + dv, v_range.y );
    }    
}

math::vec3 Disk::position( float u, float v ) const
{
    float theta = u * thetamax_;
    return vec3( radius_ * (1.0f - v) * cosf(theta), radius_ * (1.0f - v) * sinf(theta), height_ );
}
