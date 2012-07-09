//
// mathematical_functions.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_MATHEMATICAL_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void radians( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> degrees );
void degrees( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> radians );

void sin( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> a );
void asin( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> a );
void cos( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> a );
void acos( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> a );
void tan( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> a );
void atan( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> yoverx );
void atan2( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> y, ptr<Value> x );

void pow( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x, ptr<Value> y );
void exp( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void sqrt( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void inversesqrt( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void log( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void logb( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x, ptr<Value> base );

void mod( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> a, ptr<Value> b );
void abs( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void sign( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );

void mix_float( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x, ptr<Value> y, ptr<Value> alpha );
void mix_vec3( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x, ptr<Value> y, ptr<Value> alpha );

void floor( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void ceil( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void round( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );

void step( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> min, ptr<Value> value );
void smoothstep( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> min, ptr<Value> max, ptr<Value> value );

void du_float( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void du_vec3( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void dv_float( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void dv_vec3( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void deriv_float( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x, ptr<Value> y );
void deriv_vec3( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> y, ptr<Value> x );

void uniform_float_random( const Renderer& renderer, const Grid& grid, ptr<Value> result );
void uniform_vec3_random( const Renderer& renderer, const Grid& grid, ptr<Value> result );
void float_random( const Renderer& renderer, const Grid& grid, ptr<Value> result );
void vec3_random( const Renderer& renderer, const Grid& grid, ptr<Value> result );

}

}

#endif