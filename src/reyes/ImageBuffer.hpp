#pragma once

#include <math/vec4.hpp>

namespace reyes
{

class ErrorPolicy;

/**
// An buffer of pixel or texel elements in which the number and type of 
// elements can vary (used for framebuffers and textures).
*/
class ImageBuffer
{
    int width_; ///< The width of the buffer in pixels/texels.
    int height_; ///< The height the buffer in pixels/texels.
    int elements_; ///< The number of elements in each pixel/texel.
    int format_; ///< The format of each element (either FORMAT_U8 or FORMAT_F32).
    int pixel_size_; ///< The size of each pixel/texel (format size * elements).
    void* data_; ///< The buffer that pixels/texels are stored in.

    public:
        ImageBuffer();
        ImageBuffer( int width, int height, int elements, int format );
        ~ImageBuffer();
        
        int width() const;
        int height() const;
        int elements() const;
        int format() const;
        int pixel_size() const;
        
        unsigned char* u8_data() const;
        unsigned char* u8_data( int x, int y ) const;
        unsigned char* u8_data( float s, float t ) const;
        
        float* f32_data() const;
        float* f32_data( int x, int y ) const;
        float* f32_data( float s, float t ) const;

        void set_pixel( int x, int y, const math::vec4& pixel );

        void swap( ImageBuffer& image_buffer );
        void reset( int width = 0, int height = 0, int elements = 4, int format = 0, const void* data = 0 );
        void expose( float gain, float gamma );
        void quantize( const ImageBuffer& image_buffer, float one, int minimum, int maximum, float dither );

        void load( const char* filename, ErrorPolicy* error_policy = nullptr );
        void save( const char* filename, ErrorPolicy* error_policy = nullptr ) const;
        
        void load_png( const char* filename, ErrorPolicy* error_policy = nullptr );
        void save_png( const char* filename, ErrorPolicy* error_policy = nullptr ) const;
        
        void load_jpeg( const char* filename, ErrorPolicy* error_policy = nullptr );
};

}
