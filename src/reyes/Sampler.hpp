#ifndef REYES_SAMPLER_HPP_INCLUDED
#define REYES_SAMPLER_HPP_INCLUDED

#include <math/vec3.hpp>
#include <math/mat4x4.hpp>

namespace reyes
{

class Grid;
class SampleBuffer;

/**
// Sample the micropolygons in a diced grid down into a sample buffer.
*/
class Sampler
{
    struct Sample
    {
        float u_; ///< The u parameter of this sample for color interpolation.
        float v_; ///< The v parameter of this sample for color interpolation.
        int index_; ///< Index of the micropolygon that this sample is for.
        short x_; ///< The x coordinate of the sample in the sample buffer that this sample applies to.
        short y_; ///< The y coordinate of the sample in the sample buffer that this sample applies to.
    };

    const float width_;
    const float height_;
    const int maximum_vertices_;
    int x0_;
    int x1_;
    int y0_;
    int y1_;
    math::vec3* raster_positions_;
    math::vec3* origins_and_edges_;
    int* indices_;
    int* bounds_;
    int polygons_;
    Sample* samples_;
    
public:
    Sampler( float width, float height, int maximum_vertices, const math::vec4& crop_window );
    ~Sampler();    
    void sample( const math::mat4x4& screen_transform, const Grid& grid, bool matte, bool two_sided, bool left_handed, SampleBuffer* sample_buffer );
    
private:
    void calculate_raster_positions( const math::mat4x4& screen_transform, const math::vec3* positions, int vertices );
    void calculate_indices_origins_and_edges( const Grid& grid, bool two_sided, bool left_handed );
    void calculate_indices_origins_and_edges_two_sided( const Grid& grid );
    void calculate_indices_origins_and_edges_left_handed( const Grid& grid );
    void calculate_indices_origins_and_edges_right_handed( const Grid& grid );
    void calculate_bounds( int width, int height, int polygons );
    void calculate_samples( const math::vec3* colors, const math::vec3* opacities, bool matte, int polygons, SampleBuffer* sample_buffer );
    void calculate_colors_in_sample_buffer( const math::vec3* colors, const math::vec3* opacities, bool matte, int samples, SampleBuffer* sample_buffer );

    float min( float a, float b, float c ) const;
    float max( float a, float b, float c ) const;
};

}

#endif
