#ifndef REYES_MATHEMATICAL_FUNCTIONS_HPP_INCLUDED
#define REYES_MATHEMATICAL_FUNCTIONS_HPP_INCLUDED

#include <memory>

namespace sweet
{

namespace reyes
{

class Grid;
class Value;
class Renderer;

void radians( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> degrees );
void degrees( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> radians );

void sin( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> a );
void asin( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> a );
void cos( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> a );
void acos( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> a );
void tan( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> a );
void atan( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> yoverx );
void atan2( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> y, std::shared_ptr<Value> x );

void pow( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y );
void exp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void sqrt( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void inversesqrt( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void log( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void logb( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> base );

void mod( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> a, std::shared_ptr<Value> b );
void abs( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void sign( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );

void mix_float( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y, std::shared_ptr<Value> alpha );
void mix_vec3( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y, std::shared_ptr<Value> alpha );

void floor( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void ceil( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void round( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );

void step( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> min, std::shared_ptr<Value> value );
void smoothstep( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> min, std::shared_ptr<Value> max, std::shared_ptr<Value> value );

void du_float( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void du_vec3( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void dv_float( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void dv_vec3( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void deriv_float( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x, std::shared_ptr<Value> y );
void deriv_vec3( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> y, std::shared_ptr<Value> x );

void uniform_float_random( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result );
void uniform_vec3_random( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result );
void float_random( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result );
void vec3_random( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result );

}

}

#endif