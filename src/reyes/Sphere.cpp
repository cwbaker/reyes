//
// Sphere.cpp
// Copyright (c) 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Sphere.hpp"
#include "Grid.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"
#include <algorithm>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using std::min;
using std::max;
using std::list;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

Sphere::Sphere( float radius )
: Geometry( vec2(0.0f, 1.0f), vec2(0.0f, 1.0f) )
, radius_( radius )
, zmin_( -FLT_MAX )
, zmax_( FLT_MAX )
, thetamax_( 2.0f * float(M_PI) )
{   
}

Sphere::Sphere( float radius, float zmin, float zmax, float thetamax )
: Geometry( vec2(0.0f, 1.0f), vec2(0.0f, 1.0f) )
, radius_( radius )
, zmin_( zmin )
, zmax_( zmax )
, thetamax_( thetamax )
{   
}

Sphere::Sphere( const Sphere& sphere, const math::vec2& u_range, const math::vec2& v_range )
: Geometry( u_range, v_range )
, radius_( sphere.radius_ )
, zmin_( sphere.zmin_ )
, zmax_( sphere.zmax_ )
, thetamax_( sphere.thetamax_ )
{   
}

bool Sphere::boundable() const
{
    return true;
}

void Sphere::bound( const math::mat4x4& transform, vec3* minimum, vec3* maximum, Grid* grid ) const
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

bool Sphere::splittable() const
{
    return true;
}

void Sphere::split( std::list<std::shared_ptr<Geometry>>* primitives ) const
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

    shared_ptr<Geometry> sphere;
    sphere.reset( new Sphere(*this, vec2(u0, u1), vec2(v0, v1)) );
    primitives->push_back( sphere );
    sphere.reset( new Sphere(*this, vec2(u0, u1), vec2(v1, v2)) );
    primitives->push_back( sphere );
    sphere.reset( new Sphere(*this, vec2(u1, u2), vec2(v0, v1)) );
    primitives->push_back( sphere );
    sphere.reset( new Sphere(*this, vec2(u1, u2), vec2(v1, v2)) );
    primitives->push_back( sphere );
}

bool Sphere::diceable() const
{
    return true;
}

void Sphere::dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const
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
            u = min( u + du, u_range.y );
            ++vertex;
        }        
        v = min( v + dv, v_range.y );
    }    
}

math::vec3 Sphere::position( float u, float v ) const
{
    return radius_ * normal( u, v );
}

math::vec3 Sphere::normal( float u, float v ) const
{
    const float PI = float(M_PI);    
    float phimin = zmin_ > -radius_ ? asinf( zmin_ / radius_ ) : -PI / 2.0f;
    float phimax = zmax_ < radius_ ? asinf( zmax_ / radius_ ) : PI / 2.0f;
    float phi = phimin + v * (phimax - phimin);
    float theta = u * thetamax_;
    return vec3( cosf(theta) * cosf(phi), sinf(theta) * cosf(phi), sinf(phi) );
}
