//
// SampleBuffer.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "SampleBuffer.hpp"
#include "ImageBuffer.hpp"
#include "DisplayMode.hpp"
#include "ImageBufferFormat.hpp"
#include "ErrorCode.hpp"
#include "ErrorPolicy.hpp"
#include <math/vec2.ipp>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include "assert.hpp"
#include <algorithm>

using std::max;
using namespace math;
using namespace reyes;

SampleBuffer::SampleBuffer( int horizontal_resolution, int vertical_resolution, int horizontal_sampling_rate, int vertical_sampling_rate, float filter_width, float filter_height )
: horizontal_resolution_( horizontal_resolution ),
  vertical_resolution_( vertical_resolution ),
  horizontal_sampling_rate_( horizontal_sampling_rate ),
  vertical_sampling_rate_( vertical_sampling_rate ),
  filter_width_( filter_width ),
  filter_height_( filter_height ),
  width_( (horizontal_resolution + int(ceilf(filter_width - 0.5f))) * horizontal_sampling_rate ),
  height_( (vertical_resolution + int(ceilf(filter_height - 0.5f))) * vertical_sampling_rate ),
  colors_( NULL ),
  depths_( NULL ),
  positions_( NULL )
{
    REYES_ASSERT( width_ > 0 );
    REYES_ASSERT( height_ > 0 );

    colors_ = new ImageBuffer( width_, height_, 4, FORMAT_F32 );
    depths_ = new ImageBuffer( width_, height_, 1, FORMAT_F32 );
    positions_ = new ImageBuffer( width_, height_, 4, FORMAT_F32 );
    
    float* depths = depths_->f32_data();
    for ( int i = 0; i < width_ * height_; ++i )
    {
        depths[i] = FLT_MAX;
    }
    
    float* positions = positions_->f32_data();
    for ( int y = 0; y < height_; ++y )
    {
        for ( int x = 0; x < width_; ++x )
        {
            positions[(y * width_ + x) * 4 + 0] = float(x);
            positions[(y * width_ + x) * 4 + 1] = float(y);
            positions[(y * width_ + x) * 4 + 2] = 0.0f;
            positions[(y * width_ + x) * 4 + 3] = 0.0f;
        }
    }
}

SampleBuffer::~SampleBuffer()
{
    delete positions_;
    positions_ = NULL;

    delete depths_;
    depths_ = NULL;
    
    delete colors_;
    colors_ = NULL;    
}

int SampleBuffer::width() const
{
    return width_;
}

int SampleBuffer::height() const
{
    return height_;
}

float* SampleBuffer::color( int x, int y ) const
{
    REYES_ASSERT( x >= 0 && x < width_ );
    REYES_ASSERT( y >= 0 && y < height_ );
    REYES_ASSERT( colors_ );
    return colors_->f32_data( x, y );
}

float* SampleBuffer::depth( int x, int y ) const
{
    REYES_ASSERT( x >= 0 && x < width_ );
    REYES_ASSERT( y >= 0 && y < height_ );
    REYES_ASSERT( depths_ );
    return depths_->f32_data( x, y );
}

float* SampleBuffer::position( int x, int y ) const
{
    REYES_ASSERT( x >= 0 && x < width_ );
    REYES_ASSERT( y >= 0 && y < height_ );
    REYES_ASSERT( positions_ );
    return positions_->f32_data( x, y );
}

void SampleBuffer::save( int mode, const char* filename ) const
{
    ImageBuffer image_buffer;
    pack( mode, &image_buffer );
    image_buffer.save( filename );
}

void SampleBuffer::save_png( int mode, const char* filename, ErrorPolicy* error_policy ) const
{
    bool rgb = (mode & DISPLAY_MODE_RGB) != 0;
    bool alpha = (mode & DISPLAY_MODE_A) != 0;
    bool depth = (mode & DISPLAY_MODE_Z) != 0;
    
    if ( !rgb )
    {
        if ( error_policy )
        {
            error_policy->error( RENDER_ERROR_INVALID_DISPLAY_MODE, "Unable to write to a PNG image without 'rgb'" );
        }
        return;
    }
    else if ( alpha && depth )
    {
        if ( error_policy )
        {
            error_policy->error( RENDER_ERROR_INVALID_DISPLAY_MODE, "Unable to write both 'a' and 'z' into a single PNG image" );
        }
    }
    
    ImageBuffer image_buffer;
    pack( mode, &image_buffer );
    ImageBuffer quantized_image_buffer;
    quantized_image_buffer.quantize( image_buffer, 255, 0, 255, 0.0f );
    quantized_image_buffer.save_png( filename );
}

void SampleBuffer::filter( float (*filter_function)(float, float, float, float), ImageBuffer* image_buffer ) const
{
    REYES_ASSERT( filter_function );
    REYES_ASSERT( image_buffer );

    float horizontal_sampling_rate = float(horizontal_sampling_rate_);
    float vertical_sampling_rate = float(vertical_sampling_rate_);
    int half_filter_width = int(ceilf(filter_width_ / 2.0f - 0.5f));
    int half_filter_height = int(ceilf(filter_height_ / 2.0f - 0.5f));

    image_buffer->reset( horizontal_resolution_, vertical_resolution_, 4, FORMAT_F32 );
    for ( int y = 0; y < vertical_resolution_; ++y )
    {
        for ( int x = 0; x < horizontal_resolution_; ++x )
        {
            float px = float(x + half_filter_width) * horizontal_sampling_rate + horizontal_sampling_rate / 2.0f - 0.5f;
            float py = float(y + half_filter_height) * vertical_sampling_rate + vertical_sampling_rate / 2.0f - 0.5f;
            
            int x0 = x * horizontal_sampling_rate_;
            int x1 = x0 + max(1, 2 * half_filter_width) * horizontal_sampling_rate_;
            int y0 = y * vertical_sampling_rate_;
            int y1 = y0 + max(1, 2 * half_filter_height) * vertical_sampling_rate_;
            
            float area = 0.0f;
            vec4 pixel = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
            for ( int yy = y0; yy < y1; ++yy )
            {
                for ( int xx = x0; xx < x1; ++xx )
                {
                    const float* color = SampleBuffer::color( xx, yy );
                    float weight = (*filter_function)( float(xx) - px, float(yy) - py, filter_width_, filter_height_ );
                    area += weight;
                    pixel += weight * vec4( color[0], color[1], color[2], color[3] );
                }
            }
            pixel = 1.0f / area * pixel;
            pixel.w = 1.0f;
            image_buffer->set_pixel( x, y, pixel );
        }
    }
}

void SampleBuffer::pack( int mode, ImageBuffer* image_buffer ) const
{
    REYES_ASSERT( image_buffer );

    bool rgb = (mode & DISPLAY_MODE_RGB) != 0;
    bool alpha = (mode & DISPLAY_MODE_A) != 0;
    bool depth = (mode & DISPLAY_MODE_Z) != 0;
    int elements = 3 * rgb + alpha + depth;
    const float* colors = colors_->f32_data();
    const float* depths = depths_->f32_data();
   
    image_buffer->reset( width_, height_, elements, FORMAT_F32 );    
    float* data = image_buffer->f32_data();
    for ( int i = 0; i < width_ * height_; ++i )
    {
        if ( rgb )
        {
            data[0] = colors[0];
            data[1] = colors[1];
            data[2] = colors[2];
            data += 3;
        }
        
        if ( alpha )
        {
            data[0] = colors[3];
            data += 1;
        }
        
        if ( depth )
        {
            data[0] = depths[0];
            data += 1;
        }
        
        colors += 4;
        depths += 1;
    }
}
