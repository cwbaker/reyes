//
// mathematical_functions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Dispatch.hpp"
#include <reyes/Grid.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/assert.hpp>
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace math;

namespace reyes
{

void radians( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* radians = reinterpret_cast<float*>( arguments[0] );
    const float* degrees = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        radians[i] = degrees[i] * float(M_PI) / 180.0f;
    }
}

void degrees( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* degrees = reinterpret_cast<float*>( arguments[0] );
    const float* radians = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        degrees[i] = radians[i] * 180.0f / float(M_PI);
    }
}

void sin( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* sine = reinterpret_cast<float*>( arguments[0] );
    const float* angle = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        sine[i] = sinf( angle[i] );
    }
}

void asin( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* angle = reinterpret_cast<float*>( arguments[0] );
    const float* sine = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        angle[i] = asinf( sine[i] );
    }
}

void cos( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* cosine = reinterpret_cast<float*>( arguments[0] );
    const float* angle = reinterpret_cast<float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        cosine[i] = cosf( angle[i] );
    }
}

void acos( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* angle = reinterpret_cast<float*>( arguments[0] );
    const float* cosine = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        angle[i] = acosf( cosine[i] );
    }
}

void tan( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* tangent = reinterpret_cast<float*>( arguments[0] );
    const float* angle = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        tangent[i] = tanf( angle[i] );
    }
}

void atan( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* angle = reinterpret_cast<float*>( arguments[0] );
    const float* tangent = reinterpret_cast<const float*>( arguments[1] );    
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        angle[i] = atanf( tangent[i] );
    }
}

void atan2( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* angle = reinterpret_cast<float*>( arguments[0] );
    const float* y = reinterpret_cast<const float*>( arguments[1] );
    const float* x = reinterpret_cast<const float*>( arguments[2] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        angle[i] = atan2f( y[i], x[i] );
    }
}

void pow( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const float* y = reinterpret_cast<const float*>( arguments[2] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = powf( x[i], y[i] );
    }
}

void exp( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = expf( x[i] );
    }
}

void sqrt( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = sqrtf( x[i] );
    }
}

void inversesqrt( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = 1.0f / sqrtf( x[i] );
    }
}

void log( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = logf( x[i] );
    }
}

void logb( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const float* base = reinterpret_cast<const float*>( arguments[2] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = logf( x[i] ) / logf( base[0] );
    }
}

void mod( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const float* y = reinterpret_cast<const float*>( arguments[2] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = fmodf( x[i], y[i] );
    }
}

void abs( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = fabsf( x[i] );
    }
}

void sign( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = math::sign( x[i] );
    }
}

void mix_float( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const float* y = reinterpret_cast<const float*>( arguments[2] );
    const float* alpha = reinterpret_cast<const float*>( arguments[3] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = lerp( x[i], y[i], alpha[i] );
    }
}

void mix_vec3( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* x = reinterpret_cast<const vec3*>( arguments[1] );
    const vec3* y = reinterpret_cast<const vec3*>( arguments[2] );
    const float* alpha = reinterpret_cast<const float*>( arguments[3] );
    const int size = dispatch == DISPATCH_V3 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = lerp( x[i], y[i], alpha[i] );
    }
}

void floor( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = floorf( x[i] );
    }
}

void ceil( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = ceilf( x[i] );
    }
}

void round( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* x = reinterpret_cast<const float*>( arguments[1] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = floorf( x[i] + 0.5f );
    }
}

void step( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float minimum = *reinterpret_cast<const float*>( arguments[1] );
    const float* x = reinterpret_cast<const float*>( arguments[2] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        result[i] = x[i] < minimum ? 0.0f : 1.0f;
    }
}

void smoothstep( const Renderer& /*renderer*/, const Grid& grid, int dispatch, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float minimum = *reinterpret_cast<const float*>( arguments[1] );
    const float maximum = *reinterpret_cast<const float*>( arguments[2] );
    const float* x = reinterpret_cast<const float*>( arguments[3] );
    const int size = dispatch == DISPATCH_V1 ? grid.size() : 1;
    for ( int i = 0; i < size; ++i )
    {
        float t = clamp( (x[i] - minimum) / (maximum - minimum), 0.0f, 1.0f );
        result[i] = t * t * (3.0f - 2.0f * t);
    }
}

void du_float( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* v = reinterpret_cast<const float*>( arguments[1] );

    int i = 0;
    const int width = grid.width();
    const int height = grid.height();
    const float du = grid.du();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        result[i0] = (v[i1] - v[i0]) / du;

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            result[i0] = (v[i2] - v[i0]) / (2.0f * du);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        result[i0] = (v[i1] - v[i0]) / du;

        i += width;
    }
}

void du_vec3( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* v = reinterpret_cast<const vec3*>( arguments[1] );
    
    int i = 0;
    int width = grid.width();
    int height = grid.height();
    float du = grid.du();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        result[i0] = (v[i1] - v[i0]) / du;

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            result[i0] = (v[i2] - v[i0]) / (2.0f * du);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        result[i0] = (v[i1] - v[i0]) / du;

        i += width;
    }
}

void dv_float( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* v = reinterpret_cast<const float*>( arguments[1] );
    
    int i = 0;
    int width = grid.width();
    int height = grid.height();
    float dv = grid.dv();
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        result[i0] = (v[i1] - v[i0]) / dv;
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            result[i0] = (v[i2] - v[i0]) / (2.0f * dv);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        result[i0] = (v[i1] - v[i0]) / dv;
    }                            
}

void dv_vec3( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* v = reinterpret_cast<const vec3*>( arguments[1] );
    
    int i = 0;
    int width = grid.width();
    int height = grid.height();
    float dv = grid.dv();
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        result[i0] = (v[i1] - v[i0]) / dv;
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            result[i0] = (v[i2] - v[i0]) / (2.0f * dv);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        result[i0] = (v[i1] - v[i0]) / dv;
    }                            
}

void deriv_float( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* yy = reinterpret_cast<const float*>( arguments[1] );
    const float* xx = reinterpret_cast<const float*>( arguments[2] );

    int i = 0;
    int width = grid.width();
    int height = grid.height();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        result[i0] = (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            result[i0] = (yy[i2] - yy[i0]) / (xx[i2] - xx[i0]);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        result[i1] = (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);

        i += width;
    }

    i = 0;
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;        
        result[i0] += (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            result[i0] += (yy[i2] - yy[i0]) / (xx[i2] - xx[i0]);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        result[i1] += (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);
    }                            
}

void deriv_vec3( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const float* yy = reinterpret_cast<const float*>( arguments[1] );
    const float* xx = reinterpret_cast<const float*>( arguments[2] );

    int i = 0;
    int width = grid.width();
    int height = grid.height();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        result[i0] = (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            result[i0] = (yy[i2] - yy[i0]) / (xx[i2] - xx[i0]);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        result[i1] = (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);

        i += width;
    }

    i = 0;
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;        
        result[i0] += (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            result[i0] += (yy[i2] - yy[i0]) / (xx[i2] - xx[i0]);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        result[i1] += (yy[i1] - yy[i0]) / (xx[i1] - xx[i0]);
    }                            
}

void uniform_float_random( const Renderer& /*renderer*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );    
    result[0] = (float(rand()) / (RAND_MAX / 2) - 1.0f);
}

void uniform_vec3_random( const Renderer& /*renderer*/, const Grid& /*grid*/, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    result[0] = vec3(
        (float(rand()) / (RAND_MAX / 2) - 1.0f),
        (float(rand()) / (RAND_MAX / 2) - 1.0f),
        (float(rand()) / (RAND_MAX / 2) - 1.0f)
    );
}

void float_random( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );    
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = float(rand()) / RAND_MAX;
    }    
}

void vec3_random( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );    
    const int size = grid.size();
    for ( int i = 0; i < size; ++i )
    {
        result[i] = vec3(
            float(rand() / RAND_MAX),
            float(rand() / RAND_MAX),
            float(rand() / RAND_MAX)
        );
    }    
}

}
