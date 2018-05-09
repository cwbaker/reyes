#ifndef REYES_OPTIONS_HPP_INCLUDED
#define REYES_OPTIONS_HPP_INCLUDED

#include <math/vec4.hpp>
#include <math/mat4x4.hpp>
#include <string>

namespace reyes
{

/**
// Options that apply to at least one frame of a render.
*/
class Options
{
public:
    typedef float (*FilterFunction)( float x, float y, float width, float height );
    
private:
    int horizontal_resolution_; ///< The width of the rendered image (in pixels).
    int vertical_resolution_; ///< The height of the rendered image (in pixels).
    float pixel_aspect_ratio_; ///< The aspect ratio of a pixel in the rendered image.
    math::vec4 crop_window_; ///< The crop window outside of which pixels are culled (ignored).
    float frame_aspect_ratio_; ///< The aspect ratio of the image.
    math::vec4 screen_window_; ///< The camera space top, left, bottom, and right edges of the screen window on the near plane.
    math::mat4x4 view_transform_; ///< The view transform.
    float near_clip_distance_; ///< The distance from the camera to the near plane.
    float far_clip_distance_; ///< The distance from the camera to the far plane.
    float horizontal_sampling_rate_; ///< The number of samples across each pixel.
    float vertical_sampling_rate_; ///< The number of samples down each pixel.
    float gain_; ///< The gain value to use when exposing the final image.
    float gamma_; ///< The gamma value to use when exposing the final image.
    float one_; ///< The one value to use when exposing the final image.
    float dither_; ///< The dither value to use when exposing the final image.
    int minimum_; ///< The minimum color value clamped to when exposing the final image.
    int maximum_; ///< The maximum color value clamped to when exposing the final image.
    FilterFunction filter_function_; ///< The filter function to use.
    float filter_width_; ///< The width of the filter (in pixels).
    float filter_height_; ///< The height of the filter (in pixels).

public:
    Options();

    int horizontal_resolution() const;
    int vertical_resolution() const;
    float pixel_aspect_ratio() const;
    const math::vec4& crop_window() const;
    float frame_aspect_ratio() const;
    const math::vec4& screen_window() const;
    const math::mat4x4& view_transform() const;
    float near_clip_distance() const;
    float far_clip_distance() const;
    float horizontal_sampling_rate() const;
    float vertical_sampling_rate() const;
    float gain() const;
    float gamma() const;
    float one() const;
    float dither() const;
    int minimum() const;
    int maximum() const;
    FilterFunction filter_function() const;
    float filter_width() const;
    float filter_height() const;

    void set_resolution( int horizontal_resolution, int vertical_resolution, float pixel_aspect_ratio );
    void set_crop_window( const math::vec4& crop_window );
    void set_frame_aspect_ratio( float frame_aspect_ratio );
    void set_screen_window( const math::vec4& screen_window );
    void set_view_transform( const math::mat4x4& view_transform );
    void set_near_clip_distance( float near_clip_distance );
    void set_far_clip_distance( float far_clip_distance );
    void set_f_stop( float f_stop );
    void set_horizontal_sampling_rate( float horizontal_sampling_rate );
    void set_vertical_sampling_rate( float vertical_sampling_rate );
    void set_gain( float gain );
    void set_gamma( float gamma );
    void set_one( float one );
    void set_dither( float dither );
    void set_minimum( int minimum );
    void set_maximum( int maximum );
    void set_filter( FilterFunction function, float width, float height );

    static float box_filter( float x, float y, float width, float height );
    static float triangle_filter( float x, float y, float width, float height );
    static float catmull_rom_filter( float x, float y, float width, float height );
    static float gaussian_filter( float x, float y, float width, float height );
    static float sinc_filter( float x, float y, float width, float height );
};

}

#endif
