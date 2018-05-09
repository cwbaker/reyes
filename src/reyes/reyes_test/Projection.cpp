
#include <UnitTest++/UnitTest++.h>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace math;

// static void project( const mat4x4& projection, const vec4& point )
// {
//     // vec4 screen = projection * point;
//     // vec4 raster = vec4( screen.x / screen.w, screen.y / screen.w, screen.z / screen.w, screen.w );
// }

SUITE( Projection )
{
    // TEST( perspective_projection_is_correct )
    // {
    //     const float FOVX = float(M_PI) / 2.0f;
    //     const float ASPECT = 4.0f / 3.0f;
    //     const float NEAR = 1.0f;
    //     const float FAR = 100.0f;
    //     mat4x4 projection = renderman_perspective( FOVX, ASPECT, NEAR, FAR );
        
    //     float half_width = NEAR * tanf( FOVX / 2.0F );
    //     float half_height = half_width / ASPECT;

    //     float y = -half_height;        
    //     while ( y <= half_height )
    //     {
    //         float x = -half_width;
    //         while ( x <= half_width )
    //         {
    //             // float z = 4.0f;
    //             // project( projection, vec4(x, y, z, 1.0f) );
    //             const float dx = half_width / 4.0f;
    //             x += dx;
    //         }

    //         const float dy = half_height / 4.0f;
    //         y += dy;
    //     }

    //     vec4 near_middle = projection * vec4( 0.0f, 0.0f, NEAR, 1.0f );
    //     vec4 near_top_left = projection * vec4( -half_width, half_height, NEAR, 1.0f );
    //     vec4 near_bottom_left = projection * vec4( -half_width, -half_height, NEAR, 1.0f );
    //     vec4 near_top_right = projection * vec4( half_width, half_height, NEAR, 1.0f );
    //     vec4 near_bottom_right = projection * vec4( half_width, -half_height, NEAR, 1.0f );

    //     vec4 far_middle = projection * vec4( 0.0f, 0.0f, FAR, 1.0f );
    //     vec4 far_top_left = projection * vec4( -half_width, half_height, FAR, 1.0f );
    //     vec4 far_bottom_left = projection * vec4( -half_width, -half_height, FAR, 1.0f );
    //     vec4 far_top_right = projection * vec4( half_width, half_height, FAR, 1.0f );
    //     vec4 far_bottom_right = projection * vec4( half_width, -half_height, FAR, 1.0f );
    // }

    TEST( orthographic_projection_is_correct )
    {
        const float WIDTH = 2.0f;
        const float HEIGHT = 2.0f;
        const float NEAR = 1.0f;
        const float FAR = 100.0f;
        const float TOLERANCE = 0.01f;
        const float half_width = WIDTH / 2.0f;
        const float half_height = HEIGHT / 2.0f;
        mat4x4 projection = renderman_orthographic( -half_width, half_width, -half_height, half_height, NEAR, FAR );        
        
        vec4 near_middle = projection * vec4( 0.0f, 0.0f, NEAR, 1.0f );
        CHECK_CLOSE( 0.0f, near_middle.x, TOLERANCE );
        CHECK_CLOSE( 0.0f, near_middle.y, TOLERANCE );
        CHECK_CLOSE( 0.0f, near_middle.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_middle.w, TOLERANCE );
        
        vec4 near_top_left = projection * vec4( -half_width, half_height, NEAR, 1.0f );
        CHECK_CLOSE( -1.0f, near_top_left.x, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_top_left.y, TOLERANCE );
        CHECK_CLOSE( 0.0f, near_top_left.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_top_left.w, TOLERANCE );

        vec4 near_bottom_left = projection * vec4( -half_width, -half_height, NEAR, 1.0f );
        CHECK_CLOSE( -1.0f, near_bottom_left.x, TOLERANCE );
        CHECK_CLOSE( -1.0f, near_bottom_left.y, TOLERANCE );
        CHECK_CLOSE( 0.0f, near_bottom_left.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_bottom_left.w, TOLERANCE );

        vec4 near_top_right = projection * vec4( half_width, half_height, NEAR, 1.0f );
        CHECK_CLOSE( 1.0f, near_top_right.x, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_top_right.y, TOLERANCE );
        CHECK_CLOSE( 0.0f, near_top_right.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_top_right.w, TOLERANCE );

        vec4 near_bottom_right = projection * vec4( half_width, -half_height, NEAR, 1.0f );
        CHECK_CLOSE( 1.0f, near_bottom_right.x, TOLERANCE );
        CHECK_CLOSE( -1.0f, near_bottom_right.y, TOLERANCE );
        CHECK_CLOSE( 0.0f, near_bottom_right.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, near_bottom_right.w, TOLERANCE );

        vec4 far_middle = projection * vec4( 0.0f, 0.0f, FAR, 1.0f );
        CHECK_CLOSE( 0.0f, far_middle.x, TOLERANCE );
        CHECK_CLOSE( 0.0f, far_middle.y, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_middle.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_middle.w, TOLERANCE );

        vec4 far_top_left = projection * vec4( -half_width, half_height, FAR, 1.0f );
        CHECK_CLOSE( -1.0f, far_top_left.x, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_top_left.y, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_top_left.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_top_left.w, TOLERANCE );

        vec4 far_bottom_left = projection * vec4( -half_width, -half_height, FAR, 1.0f );
        CHECK_CLOSE( -1.0f, far_bottom_left.x, TOLERANCE );
        CHECK_CLOSE( -1.0f, far_bottom_left.y, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_bottom_left.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_bottom_left.w, TOLERANCE );

        vec4 far_top_right = projection * vec4( half_width, half_height, FAR, 1.0f );
        CHECK_CLOSE( 1.0f, far_top_right.x, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_top_right.y, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_top_right.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_top_right.w, TOLERANCE );

        vec4 far_bottom_right = projection * vec4( half_width, -half_height, FAR, 1.0f );
        CHECK_CLOSE( 1.0f, far_bottom_right.x, TOLERANCE );
        CHECK_CLOSE( -1.0f, far_bottom_right.y, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_bottom_right.z, TOLERANCE );
        CHECK_CLOSE( 1.0f, far_bottom_right.w, TOLERANCE );
    }
}
