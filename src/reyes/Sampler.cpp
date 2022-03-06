//
// Sampler.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Sampler.hpp"
#include "SampleBuffer.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include "assert.hpp"
#include <vector>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace math;
using namespace reyes;

static const int MAXIMUM_SAMPLES = 4096;

Sampler::Sampler( float width, float height, const math::vec4& crop_window )
: width_( width )
, height_( height )
, maximum_vertices_( 0 )
, x0_( floorf(crop_window.x * width) )
, x1_( floorf(crop_window.y * width) + 1 )
, y0_( floorf(crop_window.z * height) )
, y1_( floorf(crop_window.w * height) + 1)
, origins_and_edges_( nullptr )
, indices_( nullptr )
, polygons_( 0 )
, samples_( nullptr )
, raster_positions_( nullptr )
{
    const unsigned int MAXIMUM_TRIANGLES = 2 * 63 * 63;
    origins_and_edges_ = reinterpret_cast<vec3*>( malloc(3 * sizeof(vec3) * MAXIMUM_TRIANGLES) );
    indices_ = reinterpret_cast<int*>( malloc(3 * sizeof(int) * MAXIMUM_TRIANGLES) );
    bounds_ = reinterpret_cast<int*>( malloc(4 * sizeof(int) * MAXIMUM_TRIANGLES) );
    samples_ = reinterpret_cast<Sample*>( malloc(sizeof(Sample) * MAXIMUM_SAMPLES) );
}

Sampler::~Sampler()
{
    reset();
    free( samples_ );
    free( bounds_ );
    free( indices_ );
    free( origins_and_edges_ );
}

void Sampler::sample( const math::mat4x4& screen_transform, const Grid& grid, bool matte, bool two_sided, bool left_handed, SampleBuffer* sample_buffer )
{
    REYES_ASSERT( sample_buffer );

    polygons_ = 0;

    const vec3* colors = !matte ? grid.vec3_value( "Ci" ) : nullptr;
    const vec3* opacities = !matte ? grid.vec3_value( "Oi" ) : nullptr;
    const vec3* positions = grid.vec3_value( "P" );
    const int vertices = grid.size();
    
    reserve( grid.maximum_vertices() );
    calculate_raster_positions( screen_transform, positions, vertices );
    calculate_indices_origins_and_edges( grid, two_sided, left_handed );
    calculate_bounds( sample_buffer->width(), sample_buffer->height(), polygons_ );
    calculate_samples( colors, opacities, matte, polygons_, sample_buffer );
}

void Sampler::reset()
{
    if ( raster_positions_ )
    {
        free( raster_positions_ );
        raster_positions_ = nullptr;
    }    
}

void Sampler::reserve( int maximum_vertices )
{
    REYES_ASSERT( grid.maximum_vertices() > 0 );
    if ( maximum_vertices > maximum_vertices_ )
    {
        reset();
        raster_positions_ = reinterpret_cast<vec3*>( malloc(sizeof(vec3) * maximum_vertices) );
        maximum_vertices_ = maximum_vertices;
    }
}

void Sampler::calculate_raster_positions( const math::mat4x4& screen_transform, const vec3* positions, int vertices )
{
    REYES_ASSERT( positions );
    REYES_ASSERT( vertices >= 0 );
    REYES_ASSERT( raster_positions_ );
    
    for ( int i = 0; i < vertices; ++i )
    {
        vec4 raster_position = renderman_project( screen_transform, width_, height_, positions[i] );
        raster_positions_[i] = vec3( raster_position.x, raster_position.y, raster_position.w );
    }
}

void Sampler::calculate_indices_origins_and_edges( const Grid& grid, bool two_sided, bool left_handed )
{
    if ( two_sided )
    {
        calculate_indices_origins_and_edges_two_sided( grid );
    }
    else if ( left_handed )
    {
        calculate_indices_origins_and_edges_left_handed( grid );
    }
    else
    {
        calculate_indices_origins_and_edges_right_handed( grid );
    }
}

void Sampler::calculate_indices_origins_and_edges_two_sided( const Grid& grid )
{
    const int width = grid.width();
    const int height = grid.height();
    int i = 0;
    int index = 0;    

    for ( int y = 0; y < height - 1; ++y )
    {
        for ( int x = 0; x < width - 1; ++x )
        {
            int i0 = i + x;
            int i1 = i + width + x;
            int i2 = i + width + x + 1;
            int i3 = i + x + 1;

            const vec3& p0 = raster_positions_[i0];
            const vec3& p1 = raster_positions_[i1];
            const vec3& p2 = raster_positions_[i2];
            const vec3& p3 = raster_positions_[i3];

            vec3 u = p1 - p0;
            vec3 v = p3 - p0;

            origins_and_edges_[index * 3 + 0] = p0;
            origins_and_edges_[index * 3 + 1] = u;
            origins_and_edges_[index * 3 + 2] = v;

            indices_[index * 3 + 0] = i0;
            indices_[index * 3 + 1] = i1;
            indices_[index * 3 + 2] = i3;
            ++index;

            u = p3 - p2;
            v = p1 - p2;
            
            origins_and_edges_[index * 3 + 0] = p2;
            origins_and_edges_[index * 3 + 1] = u;
            origins_and_edges_[index * 3 + 2] = v;

            indices_[index * 3 + 0] = i2;
            indices_[index * 3 + 1] = i3;
            indices_[index * 3 + 2] = i1;
            ++index;
        }
        i += width;
    }

    polygons_ = index;
}

void Sampler::calculate_indices_origins_and_edges_left_handed( const Grid& grid )
{
    const int width = grid.width();
    const int height = grid.height();
    int i = 0;
    int index = 0;    

    for ( int y = 0; y < height - 1; ++y )
    {
        for ( int x = 0; x < width - 1; ++x )
        {
            int i0 = i + x;
            int i1 = i + width + x;
            int i2 = i + width + x + 1;
            int i3 = i + x + 1;

            const vec3& p0 = raster_positions_[i0];
            const vec3& p1 = raster_positions_[i1];
            const vec3& p2 = raster_positions_[i2];
            const vec3& p3 = raster_positions_[i3];
                                    
            vec3 u = p1 - p0;
            vec3 v = p3 - p0;
            vec3 n = cross( u, v );
            
            if ( n.z < 0.0f )
            {
                origins_and_edges_[index * 3 + 0] = p0;
                origins_and_edges_[index * 3 + 1] = u;
                origins_and_edges_[index * 3 + 2] = v;

                indices_[index * 3 + 0] = i0;
                indices_[index * 3 + 1] = i1;
                indices_[index * 3 + 2] = i3;
                ++index;
            }

            u = p3 - p2;
            v = p1 - p2;
            n = cross( u, v );
            
            if ( n.z < 0.0f )
            {
                origins_and_edges_[index * 3 + 0] = p2;
                origins_and_edges_[index * 3 + 1] = u;
                origins_and_edges_[index * 3 + 2] = v;

                indices_[index * 3 + 0] = i2;
                indices_[index * 3 + 1] = i3;
                indices_[index * 3 + 2] = i1;
                ++index;
            }
        }
        i += width;
    }

    polygons_ = index;
}

void Sampler::calculate_indices_origins_and_edges_right_handed( const Grid& grid )
{
    const int width = grid.width();
    const int height = grid.height();
    int i = 0;
    int index = 0;    

    for ( int y = 0; y < height - 1; ++y )
    {
        for ( int x = 0; x < width - 1; ++x )
        {
            int i0 = i + x;
            int i1 = i + width + x;
            int i2 = i + width + x + 1;
            int i3 = i + x + 1;

            const vec3& p0 = raster_positions_[i0];
            const vec3& p1 = raster_positions_[i1];
            const vec3& p2 = raster_positions_[i2];
            const vec3& p3 = raster_positions_[i3];
            
            vec3 u = p1 - p0;
            vec3 v = p3 - p0;
            vec3 n = cross( u, v );
            
            if ( n.z > 0.0f )
            {
                origins_and_edges_[index * 3 + 0] = p0;
                origins_and_edges_[index * 3 + 1] = u;
                origins_and_edges_[index * 3 + 2] = v;

                indices_[index * 3 + 0] = i0;
                indices_[index * 3 + 1] = i1;
                indices_[index * 3 + 2] = i3;
                ++index;
            }
            
            u = p3 - p2;
            v = p1 - p2;
            n = cross( u, v );
            
            if ( n.z > 0.0f )
            {
                origins_and_edges_[index * 3 + 0] = p2;
                origins_and_edges_[index * 3 + 1] = u;
                origins_and_edges_[index * 3 + 2] = v;

                indices_[index * 3 + 0] = i2;
                indices_[index * 3 + 1] = i3;
                indices_[index * 3 + 2] = i1;
                ++index;
            }
        }
        i += width;
    }

    polygons_ = index;
}

void Sampler::calculate_bounds( int width, int height, int polygons )
{
    REYES_ASSERT( width >= 0 );
    REYES_ASSERT( height >= 0 );
    REYES_ASSERT( polygons >= 0 );
    
    for ( int i = 0; i < polygons; ++i )
    {
        int i0 = indices_[i * 3 + 0];
        int i1 = indices_[i * 3 + 1];
        int i2 = indices_[i * 3 + 2];

        const vec3& p0 = raster_positions_[i0];
        const vec3& p1 = raster_positions_[i1];
        const vec3& p2 = raster_positions_[i2];

        int sx0 = int(floorf( min(p0.x, p1.x, p2.x) ));
        int sx1 = int(ceilf( max(p0.x, p1.x, p2.x) )) + 1;
        int sy0 = int(floorf( min(p0.y, p1.y, p2.y) ));
        int sy1 = int(ceilf( max(p0.y, p1.y, p2.y) )) + 1;

        bounds_[i * 4 + 0] = std::max( x0_, sx0 );
        bounds_[i * 4 + 1] = std::min( sx1, x1_ );
        bounds_[i * 4 + 2] = std::max( y0_, sy0 );
        bounds_[i * 4 + 3] = std::min( sy1, y1_ );
    }
}

void Sampler::calculate_samples( const math::vec3* colors, const math::vec3* opacities, bool matte, int polygons, SampleBuffer* sample_buffer )
{
    REYES_ASSERT( colors );
    REYES_ASSERT( opacities );
    REYES_ASSERT( sample_buffer );
    REYES_ASSERT( polygons >= 0 );

    Sample* sample = samples_;

    for ( int i = 0; i < polygons; ++i )
    {
        int sx0 = bounds_[i * 4 + 0];
        int sx1 = bounds_[i * 4 + 1];
        int sy0 = bounds_[i * 4 + 2];
        int sy1 = bounds_[i * 4 + 3];

        int samples = sample - samples_;
        if ( samples > 0 && samples + (sy1 - sy0) * (sx1 - sx0) > MAXIMUM_SAMPLES )
        {
            calculate_colors_in_sample_buffer( colors, opacities, matte, samples, sample_buffer );
            sample = samples_;
        }
    
        const vec3& o = origins_and_edges_[i * 3 + 0];
        const vec3& u = origins_and_edges_[i * 3 + 1];
        const vec3& v = origins_and_edges_[i * 3 + 2];
        const float one_over_determinant = 1.0f / (u.x * v.y - v.x * u.y);
        REYES_ASSERT( one_over_determinant != 0.0f );

        for ( int y = sy0; y < sy1; ++y )
        {
            for ( int x = sx0; x < sx1; ++x )
            {
                const vec3& s = *reinterpret_cast<const vec3*>( sample_buffer->position(x, y) );
                vec3 p = s - o;
                float uu = one_over_determinant * (v.y * p.x - v.x * p.y);
                float vv = one_over_determinant * (u.x * p.y - u.y * p.x);

                const float EPSILON = -0.01f;
                if ( uu >= EPSILON & vv >= EPSILON & uu + vv < 1.0f )
                {
                    float* depth = sample_buffer->depth( x, y );
                    float z = o.z + u.z * uu + v.z * vv;
                    if ( z < *depth )
                    {
                        *depth = z;
                        sample->u_ = uu;
                        sample->v_ = vv;
                        sample->index_ = i;
                        sample->x_ = x;
                        sample->y_ = y;
                        ++sample;
                    }
                }
            }
        }
    }    

    int samples = sample - samples_;
    if ( samples > 0 )
    {
        calculate_colors_in_sample_buffer( colors, opacities, matte, samples, sample_buffer );
    }
}

void Sampler::calculate_colors_in_sample_buffer( const math::vec3* colors, const math::vec3* opacities, bool matte, int samples, SampleBuffer* sample_buffer )
{
    REYES_ASSERT( colors );
    REYES_ASSERT( opacities );
    REYES_ASSERT( samples >= 0 );
    REYES_ASSERT( sample_buffer );
    REYES_ASSERT( samples_ );

    if ( matte )
    {
        const vec4 matte_color( 0.0f, 0.0f, 0.0f, 0.0f );
        for ( int i = 0; i < samples; ++i )
        {
            const Sample* sample = &samples_[i];
            vec4* color_address = reinterpret_cast<vec4*>(sample_buffer->color( sample->x_, sample->y_ ));
            *color_address = matte_color;
        }
    }
    else
    {
        for ( int i = 0; i < samples; ++i )
        {
            const Sample* sample = &samples_[i];
            
            float uu = clamp( sample->u_, 0.0f, 1.0f );
            float vv = clamp( sample->v_, 0.0f, 1.0f );
            
            int index = sample->index_;
            int i0 = indices_[index * 3 + 0];
            int i1 = indices_[index * 3 + 1];
            int i2 = indices_[index * 3 + 2];
            
            const vec3& c0 = colors[i0];
            const vec3& c1 = colors[i1];
            const vec3& c2 = colors[i2];
            
            const vec3& o0 = opacities[i0];
            const vec3& o1 = opacities[i1];
            const vec3& o2 = opacities[i2];
            
            vec4* color = reinterpret_cast<vec4*>(sample_buffer->color( sample->x_, sample->y_ ));
            *color = vec4(
                lerp(lerp(c0, c1, uu), lerp(c0, c2, vv), 0.5f),
                lerp(lerp(o0, o1, uu), lerp(o0, o2, vv), 0.5f).x
            );
        }
    }
}

float Sampler::min( float a, float b, float c ) const
{
    return std::min( std::min(a, b), c );
}

float Sampler::max( float a, float b, float c ) const
{
    return std::max( std::max(a, b), c );
}
