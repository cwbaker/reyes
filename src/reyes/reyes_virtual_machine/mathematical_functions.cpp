//
// mathematical_functions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/assert.hpp>
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace math;

namespace reyes
{

void radians( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> degrees )
{
    REYES_ASSERT( result );
    REYES_ASSERT( degrees );
    
    result->reset( degrees->type(), degrees->storage(), degrees->size() );

    const int size = degrees->size();
    float* values = result->float_values();
    const float* degree_values = degrees->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = degree_values[i] * M_PI / 180.0f;
    }
}

void degrees( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> radians )
{
    REYES_ASSERT( result );
    REYES_ASSERT( radians );
    
    result->reset( radians->type(), radians->storage(), radians->size() );

    const int size = radians->size();
    float* values = result->float_values();
    const float* radian_values = radians->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = radian_values[i] * 180.0f / M_PI;
    }
}

void sin( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> a )
{
    REYES_ASSERT( result );
    REYES_ASSERT( a );
    
    result->reset( a->type(), a->storage(), a->size() );

    const int size = a->size();
    float* values = result->float_values();
    const float* a_values = a->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = sinf( a_values[i] );
    }
}

void asin( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> a )
{
    REYES_ASSERT( result );
    REYES_ASSERT( a );
    
    result->reset( a->type(), a->storage(), a->size() );

    const int size = a->size();
    float* values = result->float_values();
    const float* a_values = a->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = asinf( a_values[i] );
    }
}

void cos( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> a )
{
    REYES_ASSERT( result );
    REYES_ASSERT( a );
    
    result->reset( a->type(), a->storage(), a->size() );

    const int size = a->size();
    float* values = result->float_values();
    const float* a_values = a->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = cosf( a_values[i] );
    }
}

void acos( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> a )
{
    REYES_ASSERT( result );
    REYES_ASSERT( a );
    
    result->reset( a->type(), a->storage(), a->size() );

    const int size = a->size();
    float* values = result->float_values();
    const float* a_values = a->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = acosf( a_values[i] );
    }
}

void tan( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> a )
{
    REYES_ASSERT( result );
    REYES_ASSERT( a );
    
    result->reset( a->type(), a->storage(), a->size() );

    const int size = a->size();
    float* values = result->float_values();
    const float* a_values = a->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = tanf( a_values[i] );
    }
}

void atan( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> yoverx )
{
    REYES_ASSERT( result );
    REYES_ASSERT( yoverx );
    
    result->reset( yoverx->type(), yoverx->storage(), yoverx->size() );

    const int size = yoverx->size();
    float* values = result->float_values();
    const float* yoverx_values = yoverx->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = atanf( yoverx_values[i] );
    }
}

void atan2( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> y, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( y );
    REYES_ASSERT( x );
    REYES_ASSERT( x->size() == y->size() );
    
    result->reset( y->type(), y->storage(), y->size() );

    const int size = y->size();
    float* values = result->float_values();
    const float* y_values = y->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = atanf( y_values[i] / x_values[i] );
    }
}

void pow( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    REYES_ASSERT( y );
    REYES_ASSERT( x->size() == y->size() );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    const float* y_values = y->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = powf( x_values[i], y_values[i] );
    }
}

void exp( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = expf( x_values[i] );
    }
}

void sqrt( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = sqrtf( x_values[i] );
    }
}

void inversesqrt( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = 1.0f / sqrtf( x_values[i] );
    }
}

void log( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = logf( x_values[i] );
    }
}

void logb( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> base )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    REYES_ASSERT( base );
    REYES_ASSERT( x->size() == base->size() );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    const float* base_values = base->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = logf( x_values[i] ) / logf( base_values[i] );
    }
}

void mod( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> a, std::shared_ptr<Value> b )
{
    REYES_ASSERT( result );
    REYES_ASSERT( a );
    REYES_ASSERT( b );
    REYES_ASSERT( a->size() == b->size() );
    
    result->reset( a->type(), a->storage(), a->size() );

    const int size = a->size();
    float* values = result->float_values();
    const float* a_values = a->float_values();
    const float* b_values = b->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = fmodf( a_values[i], b_values[i] );
    }
}

void abs( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = fabsf( x_values[i] );
    }
}

void sign( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = math::sign( x_values[i] );
    }
}

void mix_float( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y, std::shared_ptr<Value> alpha )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    REYES_ASSERT( x );
    REYES_ASSERT( x->size() == y->size() == alpha->size() );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    const float* y_values = y->float_values();
    const float* alpha_values = alpha->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = lerp( x_values[i], y_values[i], alpha_values[i] );
    }
}

void mix_vec3( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y, std::shared_ptr<Value> alpha )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    REYES_ASSERT( y );
    REYES_ASSERT( alpha );
    REYES_ASSERT( x->size() == y->size() );
    REYES_ASSERT( x->size() == alpha->size() );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    vec3* values = result->vec3_values();
    const vec3* x_values = x->vec3_values();
    const vec3* y_values = y->vec3_values();
    const float* alpha_values = alpha->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = lerp( x_values[i], y_values[i], alpha_values[i] );
    }
}

void floor( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = floorf( x_values[i] );
    }
}

void ceil( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = ceilf( x_values[i] );
    }
}

void round( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( x );
    
    result->reset( x->type(), x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = floorf( x_values[i] + 0.5f );
    }
}

void step( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> min, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( min );
    REYES_ASSERT( x );
    
    result->reset( TYPE_FLOAT, x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float min_value = min->float_value();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = x_values[i] < min_value ? 0.0f : 1.0f;
    }
}

void smoothstep( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> min, std::shared_ptr<Value> max, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( min );
    REYES_ASSERT( max );
    REYES_ASSERT( x );
    
    result->reset( TYPE_FLOAT, x->storage(), x->size() );

    const int size = x->size();
    float* values = result->float_values();
    const float min_value = min->float_value();
    const float max_value = max->float_value();
    const float* x_values = x->float_values();
    for ( int i = 0; i < size; ++i )
    {
        float t = clamp( (x_values[i] - min_value) / (max_value - min_value), 0.0f, 1.0f );
        values[i] = t * t * (3.0f - 2.0f * t);
    }
}

void du_float( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    REYES_ASSERT( p );
    REYES_ASSERT( p->type() >= TYPE_COLOR && p->type() <= TYPE_NORMAL );
    
    result->reset( p->type(), p->storage(), p->size() );
    
    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    float du = grid.du_;
    const float* other_values = p->float_values();    
    float* values = result->float_values();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        values[i0] = (other_values[i1] - other_values[i0]) / du;

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            values[i0] = (other_values[i2] - other_values[i0]) / (2.0f * du);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        values[i0] = (other_values[i1] - other_values[i0]) / du;

        i += width;
    }
}

void du_vec3( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    REYES_ASSERT( p );
    REYES_ASSERT( p->type() >= TYPE_COLOR && p->type() <= TYPE_NORMAL );
    
    result->reset( p->type(), p->storage(), p->size() );
    
    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    float du = grid.du_;
    const vec3* other_values = p->vec3_values();    
    vec3* values = result->vec3_values();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        values[i0] = (other_values[i1] - other_values[i0]) / du;

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            values[i0] = (other_values[i2] - other_values[i0]) / (2.0f * du);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        values[i0] = (other_values[i1] - other_values[i0]) / du;

        i += width;
    }
}

void dv_float( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    REYES_ASSERT( p );
    REYES_ASSERT( p->type() >= TYPE_COLOR && p->type() <= TYPE_NORMAL );
    
    result->reset( p->type(), p->storage(), p->size() );
    
    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    float dv = grid.dv_;
    const float* other_values = p->float_values();    
    float* values = result->float_values();
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        values[i0] = (other_values[i1] - other_values[i0]) / dv;
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            values[i0] = (other_values[i2] - other_values[i0]) / (2.0f * dv);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        values[i0] = (other_values[i1] - other_values[i0]) / dv;
    }                            
}

void dv_vec3( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p )
{
    REYES_ASSERT( p );
    REYES_ASSERT( p->type() >= TYPE_COLOR && p->type() <= TYPE_NORMAL );
    
    result->reset( p->type(), p->storage(), p->size() );
    
    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    float dv = grid.dv_;
    const vec3* other_values = p->vec3_values();    
    vec3* values = result->vec3_values();
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        values[i0] = (other_values[i1] - other_values[i0]) / dv;
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            values[i0] = (other_values[i2] - other_values[i0]) / (2.0f * dv);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        values[i0] = (other_values[i1] - other_values[i0]) / dv;
    }                            
}

void deriv_float( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> y, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( y );
    REYES_ASSERT( x );
    REYES_ASSERT( x->size() == y->size() );
    
    result->reset( TYPE_FLOAT, y->storage(), y->size() );

    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    const float* y_values = y->float_values();    
    const float* x_values = x->float_values();    
    float* values = result->float_values();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        values[i0] = (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            values[i0] = (y_values[i2] - y_values[i0]) / (x_values[i2] - x_values[i0]);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        values[i1] = (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);

        i += width;
    }

    i = 0;
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;        
        values[i0] += (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            values[i0] += (y_values[i2] - y_values[i0]) / (x_values[i2] - x_values[i0]);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        values[i1] += (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);
    }                            
}

void deriv_vec3( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> y, std::shared_ptr<Value> x )
{
    REYES_ASSERT( result );
    REYES_ASSERT( y );
    REYES_ASSERT( x );
    REYES_ASSERT( x->size() == y->size() );
    
    result->reset( y->type(), y->storage(), y->size() );

    int i = 0;
    int width = grid.width_;
    int height = grid.height_;
    const vec3* y_values = y->vec3_values();    
    const float* x_values = x->float_values();    
    vec3* values = result->vec3_values();
    
    for ( int y = 0; y < height; ++y )
    {
        int i0 = i;
        int i1 = i + 1;
        values[i0] = (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);

        for ( int x = 1; x < width - 1; ++x )
        {
            int i0 = i + x - 1;
            int i2 = i + x + 1;
            values[i0] = (y_values[i2] - y_values[i0]) / (x_values[i2] - x_values[i0]);
        }

        i0 = i + width - 2;
        i1 = i + width - 1;
        values[i1] = (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);

        i += width;
    }

    i = 0;
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;        
        values[i0] += (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);
    }

    for ( int y = 1; y < height - 1; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            int i0 = i + x;
            int i2 = i + width * 2 + x;
            values[i0] += (y_values[i2] - y_values[i0]) / (x_values[i2] - x_values[i0]);
        }                               
        i += width;
    }
    
    for ( int x = 0; x < width; ++x )
    {
        int i0 = i + x;
        int i1 = i + width + x;
        values[i1] += (y_values[i1] - y_values[i0]) / (x_values[i1] - x_values[i0]);
    }                            
}

void uniform_float_random( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result )
{
    REYES_ASSERT( result );
    result->reset( TYPE_FLOAT, STORAGE_UNIFORM, 1 );
    result->float_values()[0] = (float(rand()) / (RAND_MAX / 2) - 1.0f);
}

void uniform_vec3_random( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result )
{
    REYES_ASSERT( result );
    result->reset( TYPE_POINT, STORAGE_UNIFORM, 1 );
    result->vec3_values()[0] = vec3(
        (float(rand()) / (RAND_MAX / 2) - 1.0f),
        (float(rand()) / (RAND_MAX / 2) - 1.0f),
        (float(rand()) / (RAND_MAX / 2) - 1.0f)
    );
}

void float_random( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result )
{
    REYES_ASSERT( result );
    
    int size = grid.size();
    result->reset( TYPE_FLOAT, STORAGE_VARYING, size );
    float* values = result->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = float(rand()) / RAND_MAX;
    }    
}

void vec3_random( const Renderer& /*renderer*/, const Grid& grid, std::shared_ptr<Value> result )
{
    REYES_ASSERT( result );

    int size = grid.size();
    result->reset( TYPE_POINT, STORAGE_VARYING, size );
    vec3* values = result->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = vec3(
            float(rand() / RAND_MAX),
            float(rand() / RAND_MAX),
            float(rand() / RAND_MAX)
        );
    }    
}

}
