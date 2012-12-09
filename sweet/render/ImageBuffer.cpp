// 
// ImageBuffer.cpp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "ImageBuffer.hpp"
#include "DisplayMode.hpp"
#include "ImageBufferFormat.hpp"
#include "Error.hpp"
#include "ErrorPolicy.hpp"
#include <sweet/math/scalar.ipp>
#include <sweet/assert/assert.hpp>
#include <sweet/png/png.h>
#include <sweet/jpeg/jpeglib.h>
#include <algorithm>
#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

ImageBuffer::ImageBuffer()
: width_( 0 ),
  height_( 0 ),
  elements_( 0 ),
  format_( FORMAT_U8 ),
  data_( NULL )
{
}

ImageBuffer::ImageBuffer( int width, int height, int elements, int format )
: width_( 0 ),
  height_( 0 ),
  elements_( 0 ),
  format_( 0 ),
  data_( NULL )
{
    reset( width, height, elements, format );
}

ImageBuffer::~ImageBuffer()
{
    if ( data_ )
    {
        free( data_ );
        data_ = NULL;
    }
}

int ImageBuffer::width() const
{
    return width_;
}

int ImageBuffer::height() const
{
    return height_;
}

int ImageBuffer::elements() const
{
    return elements_;
}

int ImageBuffer::format() const
{
    return format_;
}

int ImageBuffer::pixel_size() const
{
    static const int SIZE_BY_FORMAT[FORMAT_COUNT] =
    {
        sizeof(unsigned char), // FORMAT_U8
        sizeof(float) // FORMAT_F32
    };
    return SIZE_BY_FORMAT[format_] * elements_;
}

unsigned char* ImageBuffer::u8_data() const
{
    SWEET_ASSERT( format_ == FORMAT_U8 );
    return reinterpret_cast<unsigned char*>( data_ );
}

unsigned char* ImageBuffer::u8_data( int x, int y ) const
{
    SWEET_ASSERT( format_ == FORMAT_U8 );
    unsigned char* data = reinterpret_cast<unsigned char*>( data_ );
    return &data[(y * width_ + x) * elements_];
}

unsigned char* ImageBuffer::u8_data( float s, float t ) const
{
    SWEET_ASSERT( s >= 0.0f && s <= 1.0f );
    SWEET_ASSERT( t >= 0.0f && t <= 1.0f );
    return u8_data( int(s * (width_ - 1)), int(t * (height_ - 1)) );
}

float* ImageBuffer::f32_data() const
{
    SWEET_ASSERT( format_ == FORMAT_F32 );
    return reinterpret_cast<float*>( data_ );
}

float* ImageBuffer::f32_data( int x, int y ) const
{
    SWEET_ASSERT( format_ == FORMAT_F32 );
    float* data = reinterpret_cast<float*>( data_ );
    return &data[(y * width_ + x) * elements_];
}

float* ImageBuffer::f32_data( float s, float t ) const
{
    SWEET_ASSERT( s >= 0.0f && s <= 1.0f );
    SWEET_ASSERT( t >= 0.0f && t <= 1.0f );
    return f32_data( int(s * (width_ - 1)), int(t * (height_ - 1)) );
}

void ImageBuffer::set_pixel( int x, int y, const math::vec4& pixel )
{
    SWEET_ASSERT( x >= 0 && x < width_ );
    SWEET_ASSERT( y >= 0 && y < height_ );
    SWEET_ASSERT( format_ == FORMAT_F32 );
    SWEET_ASSERT( elements_ == 4 );
        
    float* data = f32_data( x, y );
    *(data + 0) = pixel.x;
    *(data + 1) = pixel.y;
    *(data + 2) = pixel.z;
    *(data + 3) = pixel.w;
}

void ImageBuffer::swap( ImageBuffer& image_buffer )
{
    std::swap( width_, image_buffer.width_ );
    std::swap( height_, image_buffer.height_ );
    std::swap( elements_, image_buffer.elements_ );
    std::swap( format_, image_buffer.format_ );
    std::swap( pixel_size_, image_buffer.pixel_size_ );
    std::swap( data_, image_buffer.data_ );
}

void ImageBuffer::reset( int width, int height, int elements, int format, const void* data )
{
    SWEET_ASSERT( width >= 0 );
    SWEET_ASSERT( height >= 0 );
    SWEET_ASSERT( elements > 0 );
    SWEET_ASSERT( format >= FORMAT_U8 && format < FORMAT_COUNT );

    if ( width_ != width || height_ != height || elements_ != elements || format_ != format )
    {
        if ( data_ )
        {
            free( data_ );
            data_ = NULL;
        }
                
        width_ = width;
        height_ = height;
        elements_ = elements;
        format_ = format;
        pixel_size_ = pixel_size();

        if ( width_ > 0 && height_ > 0 )
        {
            data_ = reinterpret_cast<unsigned char*>( malloc(width_ * height_ * pixel_size_) );
        }
    }
            
    if ( data )
    {
        memcpy( data_, data, width_ * height_ * pixel_size_ );
    }
    else
    {
        memset( data_, 0, width_ * height_ * pixel_size_ );
    }
}

void ImageBuffer::expose( float gain, float gamma )
{
    float* pixels = f32_data();
    int size = width_ * height_ * elements_;
    for ( int i = 0; i < size; ++i )
    {
        pixels[i] = powf( pixels[i] * gain, gamma );
    }
}

void ImageBuffer::quantize( const ImageBuffer& image_buffer, float one, int minimum, int maximum, float dither )
{
    SWEET_ASSERT( image_buffer.format_ == FORMAT_F32 );

    minimum = clamp( minimum, 0, 255 );
    maximum = clamp( maximum, 0, 255 );

    reset( image_buffer.width_, image_buffer.height_, image_buffer.elements_, FORMAT_U8 );
    unsigned char* quantized_pixels = u8_data();
    const float* pixels = image_buffer.f32_data();
    int size = width_ * height_ * elements_;
    for ( int i = 0; i < size; ++i )
    {
        quantized_pixels[i] = clamp( int(round(one * pixels[i] + dither * (float(rand()) / (RAND_MAX / 2) - 1.0f))), minimum, maximum );
    }    
}

void ImageBuffer::load( const char* filename, ErrorPolicy* error_policy )
{
    SWEET_ASSERT( filename );

    struct LoadNativeGuard
    {
        FILE* file;
        
        LoadNativeGuard()
        : file( NULL )
        {
        }
        
        ~LoadNativeGuard()
        {
            if ( file )
            {
                fclose( file );
                file = NULL;
            }
        }        
    };

    LoadNativeGuard guard;
    guard.file = fopen( filename, "rb" );
    if ( guard.file )
    {
        int width = 0;
        int height = 0;
        int elements = 0;
        int format = 0;

        fread( &width, sizeof(format), 1, guard.file );
        fread( &height, sizeof(height), 1, guard.file );
        fread( &elements, sizeof(elements), 1, guard.file );
        fread( &format, sizeof(format), 1, guard.file );    
        reset( width, height, elements, format );
        fread( data_, pixel_size_, width_ * height_, guard.file );
    }
    else
    {
        if ( error_policy )
        {
            error_policy->error( RENDER_ERROR_OPENING_FILE_FAILED, "Opening '%s' to read a native image failed", filename );
        }    
    }
}

void ImageBuffer::save( const char* filename, ErrorPolicy* error_policy ) const
{
    SWEET_ASSERT( filename );
    
    struct SaveNativeGuard
    {
        FILE* file;
        
        SaveNativeGuard()
        : file( NULL )
        {
        }
        
        ~SaveNativeGuard()
        {
            if ( file )            
            {
                fclose( file );
                file = NULL;
            }
        }
    };
    
    SaveNativeGuard guard;
    guard.file = fopen( filename, "wb" );
    if ( guard.file )
    {
        fwrite( &width_, sizeof(width_), 1, guard.file );
        fwrite( &height_, sizeof(width_), 1, guard.file );
        fwrite( &elements_, sizeof(elements_), 1, guard.file );
        fwrite( &format_, sizeof(format_), 1, guard.file );
        fwrite( data_, sizeof(unsigned char) * pixel_size_, width_ * height_, guard.file );
    }
    else
    {
        if ( error_policy )
        {
            error_policy->error( RENDER_ERROR_OPENING_FILE_FAILED, "Opening '%s' to write native image failed", filename );
        }
    }    
}

void ImageBuffer::load_png( const char* filename, ErrorPolicy* error_policy )
{
    SWEET_ASSERT( filename );

    struct LoadPngGuard
    {
        FILE* file;
        png_structp png_read;
        png_infop png_info;
        
        LoadPngGuard()
        : file( NULL ),
          png_read( NULL ),
          png_info( NULL )
        {
        }
        
        ~LoadPngGuard()
        {
            png_destroy_read_struct( &png_read, &png_info, NULL );
            png_info = NULL;
            png_read = NULL;

            if ( file )
            {
                fclose( file );
                file = NULL;
            }        
        }

        static void png_read_status_callback( png_structp png, png_uint_32 row, int pass )
        {
        }
    };

    LoadPngGuard guard;
    
    guard.file = fopen( filename, "rb" );
    if ( guard.file )
    {
        const unsigned int SIGNATURE_LENGTH = 8;
        png_byte signature [SIGNATURE_LENGTH];
        fread( signature, sizeof(unsigned char), sizeof(signature), guard.file );
        if ( png_sig_cmp(signature, 0, sizeof(signature)) != 0 )
        {
            SWEET_ERROR( ReadingFileFailedError("Signature of '%s' doesn't match PNG", filename) );
        }
        
        guard.png_read = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
        if ( !guard.png_read )
        {
            SWEET_ERROR( OutOfMemoryError("Allocating memory to read a PNG from '%s' failed", filename) );
        }
        
        guard.png_info = png_create_info_struct( guard.png_read );
        if ( !guard.png_info )
        {
            SWEET_ERROR( OutOfMemoryError("Allocating memory to read PNG info from '%s' failed", filename) );
        }
        
        png_init_io( guard.png_read, guard.file );
        png_set_sig_bytes( guard.png_read, sizeof(signature) );
        png_set_keep_unknown_chunks( guard.png_read, PNG_HANDLE_CHUNK_NEVER, NULL, 0 );
        png_read_info( guard.png_read, guard.png_info );
        
        int interlace_type = png_get_interlace_type( guard.png_read, guard.png_info );
        if ( interlace_type != 0 )
        {
            SWEET_ERROR( ReadingFileFailedError("Reading an interlaced PNG from '%s' is not supported", filename) );
        }

        int width = png_get_image_width( guard.png_read, guard.png_info );
        int height = png_get_image_height( guard.png_read, guard.png_info );
        int bit_depth = png_get_bit_depth( guard.png_read, guard.png_info );
        int color_type = png_get_color_type( guard.png_read, guard.png_info );

        if ( bit_depth < 8 )
        {
            png_set_packing( guard.png_read );
        }    
        
        if ( bit_depth == 16 )
        {
            png_set_strip_16( guard.png_read );
        }

        if ( color_type == PNG_COLOR_TYPE_PALETTE )
        {
            png_set_palette_to_rgb( guard.png_read );
        }
        
        if ( color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
        {
            png_set_expand_gray_1_2_4_to_8( guard.png_read );
        }
        
        if ( png_get_valid(guard.png_read, guard.png_info, PNG_INFO_tRNS) )
        {
            png_set_tRNS_to_alpha( guard.png_read );
        }
        
        if ( color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY )
        {
            png_set_add_alpha( guard.png_read, 0xff, PNG_FILLER_AFTER );
        }
        
        if ( color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
        {
            png_set_gray_to_rgb( guard.png_read );
        }
               
        png_read_update_info( guard.png_read, guard.png_info );
        reset( width, height, 4, FORMAT_U8 );
        unsigned char* data = u8_data();
        for ( int y = 0; y < height; ++y )
        {
            png_read_row( guard.png_read, &data[y * width_ * pixel_size_], NULL );
        }
        png_read_end( guard.png_read, NULL );    
    }
    else
    {
        if ( error_policy )
        {
            error_policy->error( RENDER_ERROR_OPENING_FILE_FAILED, "Opening '%s' to read a PNG failed", filename );
        }
    }    
}

void ImageBuffer::save_png( const char* filename, ErrorPolicy* error_policy ) const
{
    SWEET_ASSERT( filename );

    struct SavePngGuard
    {
        FILE* file;
        png_structp png_write;
        png_infop png_info;
        
        SavePngGuard()
        : file( NULL ),
          png_write( NULL ),
          png_info( NULL )
        {
        }
        
        ~SavePngGuard()
        {
            png_destroy_write_struct( &png_write, &png_info );
            png_info = NULL;
            png_write = NULL;

            if ( file )
            {
                fclose( file );
                file = NULL;
            }        
        }

        static void png_write_status_callback( png_structp png, png_uint_32 row, int pass )
        {
        }

    };

    SavePngGuard guard;

    guard.file = fopen( filename, "wb" );
    if ( guard.file )
    {
        guard.png_write = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
        if ( !guard.png_write )
        {
            SWEET_ERROR( OutOfMemoryError("Allocating memory to write a PNG to '%s' failed", filename) );
        }

        guard.png_info = png_create_info_struct( guard.png_write );
        if ( !guard.png_info )
        {
            SWEET_ERROR( OutOfMemoryError("Allocating memory to write a PNG to '%s' failed", filename) );
        }
        
        png_init_io( guard.png_write, guard.file );
        png_set_write_status_fn( guard.png_write, &SavePngGuard::png_write_status_callback );
        int color_type = elements_ == 4 ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB;
        png_set_IHDR( guard.png_write, guard.png_info, width_, height_, 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
        png_write_info( guard.png_write, guard.png_info );
        unsigned char* data = u8_data();
        for ( int y = 0; y < height_; ++y )
        {
            png_write_row( guard.png_write, &data[y * width_ * elements_] );
        }
    }
    else
    {
        if ( error_policy )
        {
            error_policy->error( RENDER_ERROR_OPENING_FILE_FAILED, "Opening '%s' to write a PNG failed", filename );
        }
    }
}

void ImageBuffer::load_jpeg( const char* filename, ErrorPolicy* error_policy )
{
    SWEET_ASSERT( filename );
    
    struct JpegDecoder
    {
        struct jpeg_error_mgr error_mgr_;
        struct jpeg_decompress_struct* decompress_;
        ErrorPolicy* error_policy_;
        FILE* file_;
        
        JpegDecoder( struct jpeg_decompress_struct* decompress, const char* filename, ErrorPolicy* error_policy )
        : error_mgr_(),
          decompress_( decompress ),
          error_policy_( error_policy ),
          file_( NULL )
        {
            SWEET_ASSERT( decompress );
            SWEET_ASSERT( filename );
            
            file_ = fopen( filename, "rb" );
            if ( file_ )
            {
                decompress_->err = jpeg_std_error( &error_mgr_ );
                error_mgr_.error_exit = &JpegDecoder::jpeg_error_exit;
            }
            else
            {
                decompress_ = NULL;
                
                if ( error_policy_ )
                {
                    error_policy->error( RENDER_ERROR_OPENING_FILE_FAILED, "Opening '%s' to read a JPEG failed", filename );
                }
            }
        }
        
        ~JpegDecoder()
        {           
            if ( file_ )
            {
                fclose( file_ );
                file_ = NULL;
            }

            if ( decompress_ )
            {
                jpeg_destroy_decompress( decompress_ );
                decompress_ = NULL;
            }
        }

        static void jpeg_error_exit( struct jpeg_common_struct* jpeg_common )
        {
            SWEET_ERROR( ReadingFileFailedError("Reading a JPEG failed") );
        }
    };

    struct jpeg_decompress_struct decompress;
    JpegDecoder decoder( &decompress, filename, error_policy );
    if ( decoder.file_ )
    {
        jpeg_create_decompress( &decompress );
        jpeg_stdio_src( &decompress, decoder.file_ );
        jpeg_read_header( &decompress, TRUE );
        jpeg_start_decompress( &decompress );

        SWEET_ASSERT( decompress.output_components == 3 );
        reset( decompress.output_width, decompress.output_height, sizeof(unsigned char) * 3 );

        for ( int y = 0; y < height_; ++y )
        {
            unsigned char* row = u8_data( 0, y );
            jpeg_read_scanlines( &decompress, &row, 1 );
        }
        jpeg_finish_decompress( &decompress );
    }
}
