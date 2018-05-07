//
// Torus.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Torus.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using std::min;
using std::max;
using std::list;
using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

Torus::Torus( float rmajor, float rminor, float phimin, float phimax, float thetamax )
: Geometry(vec2(0.0f, 1.0f), vec2(0.0f, 1.0f)),
  rmajor_( rmajor ),
  rminor_( rminor ),
  phimin_( phimin ),
  phimax_( phimax ),
  thetamax_( thetamax )
{   
}

Torus::Torus( const Torus& torus, const math::vec2& u_range, const math::vec2& v_range )
: Geometry(u_range, v_range),
  rmajor_( torus.rmajor_ ),
  rminor_( torus.rminor_ ),
  phimin_( torus.phimin_ ),
  phimax_( torus.phimax_ ),
  thetamax_( torus.thetamax_ )
{   
}

bool Torus::boundable() const
{
    return true;
}

void Torus::bound( const math::mat4x4& transform, vec3* minimum, vec3* maximum ) const
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

bool Torus::splittable() const
{
    return true;
}

void Torus::split( std::list<std::shared_ptr<Geometry>>* primitives ) const
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

    shared_ptr<Geometry> torus;
    torus.reset( new Torus(*this, vec2(u0, u1), vec2(v0, v1)) );
    primitives->push_back( torus );
    torus.reset( new Torus(*this, vec2(u0, u1), vec2(v1, v2)) );
    primitives->push_back( torus );
    torus.reset( new Torus(*this, vec2(u1, u2), vec2(v0, v1)) );
    primitives->push_back( torus );
    torus.reset( new Torus(*this, vec2(u1, u2), vec2(v1, v2)) );
    primitives->push_back( torus );
}

bool Torus::diceable() const
{
    return true;
}

void Torus::dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const
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
            positions[vertex] = vec3( transform * vec4(position(u, v), 1.0f) );
            s[vertex] = u;
            t[vertex] = v;
            u = min( u + du, u_range.y );
            ++vertex;
        }        
        v = min( v + dv, v_range.y );
    }    
}

math::vec3 Torus::position( float u, float v ) const
{
    float theta = u * thetamax_;
    float phi = phimin_ + (phimax_ - phimin_) * v;
    float r = rminor_ * cosf( phi );
    return vec3( (rmajor_ + r) * cosf(theta), (rmajor_ + r) * sinf(theta), rminor_ * sinf(phi) );
}
