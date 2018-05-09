#ifndef REYES_SAMPLEBUFFER_HPP_INCLUDED
#define REYES_SAMPLEBUFFER_HPP_INCLUDED

#include "declspec.hpp"
#include <math/vec4.hpp>
#include <math/mat4x4.hpp>

namespace sweet
{

namespace render
{

class ErrorPolicy;
class ImageBuffer;

/**
// A buffer of samples.
*/
class REYES_DECLSPEC SampleBuffer
{
    int horizontal_resolution_; ///< The number of pixels across.
    int vertical_resolution_; ///< The number of pixels down.
    int horizontal_sampling_rate_; ///< The number of samples across a pixel.
    int vertical_sampling_rate_; ///< The number of samples down a pixel.
    float filter_width_; ///< The number of pixels to filter in x.
    float filter_height_; ///< The number of pixels to filter in y.
    int width_; ///< The number of horiztonal samples (horizontal resolution * horizontal samples per pixel + floor((filter_width + 1) / 2)).
    int height_; ///< The number of vertical samples (vertical resolution * vertical samples per pixel + floor((filter_height + 1) / 2)).
    ImageBuffer* colors_; ///< The color of the nearest element.
    ImageBuffer* depths_; ///< The distance of the nearest element from the near plane.
    ImageBuffer* positions_; ///< The sample position on the near plane in sample space.
    
    public:
        SampleBuffer( int horizontal_resolution, int vertical_resolution, int horizontal_sampling_rate, int vertical_sampling_rate, float filter_width, float filter_height );
        ~SampleBuffer();
        
        int width() const;
        int height() const;        
        float* color( int x, int y ) const;
        float* depth( int x, int y ) const;
        float* position( int x, int y ) const;
        
        void save( int mode, const char* filename ) const;
        void save_png( int mode, const char* filename, ErrorPolicy* error_policy ) const;
        void filter( float (*filter_function)(float, float, float, float), ImageBuffer* image_buffer ) const;
        void pack( int mode, ImageBuffer* image_buffer ) const;        
};

}

}

#endif