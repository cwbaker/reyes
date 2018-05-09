//
// Options.cpp
// Copyright (c) Charles Baker. All rights reserved
//

#include "stdafx.hpp"
#include "Options.hpp"
#include <math/vec3.ipp>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include "assert.hpp"
#include <algorithm>

using std::max;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::reyes;

Options::Options()
: horizontal_resolution_( 640 ),
  vertical_resolution_( 480 ),
  pixel_aspect_ratio_( 1.0f ),
  crop_window_( 0.0f, 1.0f, 0.0f, 1.0f ),
  frame_aspect_ratio_( 4.0f / 3.0f ),
  screen_window_( -4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f ),
  view_transform_(),
  near_clip_distance_( 1.0f ),
  far_clip_distance_( 100.0f ),
  horizontal_sampling_rate_( 2.0f ),
  vertical_sampling_rate_( 2.0f ),
  gain_( 1.0f ),
  gamma_( 1.0f ),
  one_( 255.0f ),
  dither_( 0.5f ),
  minimum_( 0 ),
  maximum_( 255 ),
  filter_function_( &Options::box_filter ),
  filter_width_( 1.0f ),
  filter_height_( 1.0f )
{
#ifdef BUILD_VARIANT_DEBUG
    horizontal_resolution_ = 32;
    vertical_resolution_ = 24;
#endif
}

int Options::horizontal_resolution() const
{
    return horizontal_resolution_;
}

int Options::vertical_resolution() const
{
    return vertical_resolution_;
}

float Options::pixel_aspect_ratio() const
{
    return pixel_aspect_ratio_;
}

const math::vec4& Options::crop_window() const
{
    return crop_window_;
}

float Options::frame_aspect_ratio() const
{
    return frame_aspect_ratio_;
}

const math::vec4& Options::screen_window() const
{
    return screen_window_;
}

const math::mat4x4& Options::view_transform() const
{
    return view_transform_;
}

float Options::near_clip_distance() const
{
    return near_clip_distance_;
}

float Options::far_clip_distance() const
{
    return far_clip_distance_;
}

float Options::horizontal_sampling_rate() const
{
    return horizontal_sampling_rate_;
}

float Options::vertical_sampling_rate() const
{
    return vertical_sampling_rate_;
}

float Options::gain() const
{
    return gain_;
}

float Options::gamma() const
{
    return gamma_;
}

float Options::one() const
{
    return one_;
}

float Options::dither() const
{
    return dither_;
}

int Options::minimum() const
{
    return minimum_;
}

int Options::maximum() const
{
    return maximum_;
}

Options::FilterFunction Options::filter_function() const
{
    return filter_function_;
}

float Options::filter_width() const
{
    return filter_width_;
}

float Options::filter_height() const
{
    return filter_height_;
}

void Options::set_resolution( int horizontal_resolution, int vertical_resolution, float pixel_aspect_ratio )
{
    REYES_ASSERT( horizontal_resolution > 1 );
    REYES_ASSERT( vertical_resolution > 1 );
    REYES_ASSERT( pixel_aspect_ratio > 0.0f );

    horizontal_resolution_ = horizontal_resolution;
    vertical_resolution_ = vertical_resolution;
    pixel_aspect_ratio_ = pixel_aspect_ratio;
    set_frame_aspect_ratio( float(horizontal_resolution) * pixel_aspect_ratio / float(vertical_resolution) );
}

void Options::set_crop_window( const math::vec4& crop_window )
{
    crop_window_ = vec4(
        clamp( crop_window.x, 0.0f, 1.0f ),
        clamp( crop_window.y, 0.0f, 1.0f ),
        clamp( crop_window.z, 0.0f, 1.0f ),
        clamp( crop_window.w, 0.0f, 1.0f )
    );
}

void Options::set_frame_aspect_ratio( float frame_aspect_ratio )
{
    REYES_ASSERT( frame_aspect_ratio > 0.0f );

    frame_aspect_ratio_ = frame_aspect_ratio;
      
    if ( frame_aspect_ratio_ >= 1.0f )
    {
        screen_window_ = vec4( -frame_aspect_ratio_, frame_aspect_ratio_, -1.0f, 1.0f );
    }
    else
    {
        screen_window_ = vec4( -1.0f, 1.0f, -1.0f / frame_aspect_ratio_, 1.0f / frame_aspect_ratio_ );
    }
}

void Options::set_screen_window( const math::vec4& screen_window )
{
    screen_window_ = screen_window;
}

void Options::set_view_transform( const math::mat4x4& view_transform )
{
    view_transform_ = view_transform;
}

void Options::set_near_clip_distance( float near_clip_distance )
{
    near_clip_distance_ = near_clip_distance;
}

void Options::set_far_clip_distance( float far_clip_distance )
{
    far_clip_distance_ = far_clip_distance;
}

void Options::set_horizontal_sampling_rate( float horizontal_sampling_rate )
{
    horizontal_sampling_rate_ = horizontal_sampling_rate;
}

void Options::set_vertical_sampling_rate( float vertical_sampling_rate )
{
    vertical_sampling_rate_ = vertical_sampling_rate;
}

void Options::set_gain( float gain )
{
    gain_ = gain;
}

void Options::set_gamma( float gamma )
{
    gamma_ = gamma;
}

void Options::set_one( float one )
{
    one_ = one;
}

void Options::set_dither( float dither )
{
    dither_ = dither;
}

void Options::set_minimum( int minimum )
{
    minimum_ = minimum;
}

void Options::set_maximum( int maximum )
{
    maximum_ = maximum;
}

void Options::set_filter( FilterFunction function, float width, float height )
{
    REYES_ASSERT( function );
    REYES_ASSERT( width >= 1.0f );
    REYES_ASSERT( height >= 1.0f );
    filter_function_ = function;
    filter_width_ = max( 1.0f, width );
    filter_height_ = max( 1.0f, height );
}

float Options::box_filter( float /*x*/, float /*y*/, float /*width*/, float /*height*/ )
{
    return 1.0f;
}

float Options::triangle_filter( float x, float y, float width, float height )
{
    return ((1.0f - fabsf(x)) / (0.5f * width)) * ((1.0f - fabsf(y)) / (0.5f * height));
}

float Options::catmull_rom_filter( float x, float y, float /*width*/, float /*height*/ )
{
    float r2 = x * x + y * y;
    float r = sqrtf( r2 );
    return r >= 2.0f ? 0.0f : r < 1.0f ? 3.0f * r * r2 - 5.0f * r2 + 2.0f : -r * r2 + 5.0f * r2 - 8.0f * r + 4.0f;        
}

float Options::gaussian_filter( float x, float y, float width, float height )
{
    x *= 2.0f / width;
    y *= 2.0f / height;
    return exp( -2.0f * (x * x * y * y) );
}

float Options::sinc_filter( float x, float y, float /*width*/, float /*height*/ )
{
    float s = x > -0.001f && x < 0.001f ? 1.0f : sinf( x ) / x;
    float t = y > -0.001f && y < 0.001f ? 1.0f : sinf( y ) / y;
    return s * t;
}
