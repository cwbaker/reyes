#ifndef REYES_MATHEMATICAL_FUNCTIONS_HPP_INCLUDED
#define REYES_MATHEMATICAL_FUNCTIONS_HPP_INCLUDED

namespace reyes
{

class Grid;
class Renderer;

void radians( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void degrees( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void sin( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void asin( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void cos( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void acos( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void tan( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void atan( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void atan2( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void pow( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void exp( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void sqrt( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void inversesqrt( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void log( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void logb( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void mod( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void abs( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void sign( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void mix_float( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void mix_vec3( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void floor( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void ceil( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void round( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void step( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void smoothstep( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void du_float( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void du_vec3( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void dv_float( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void dv_vec3( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void deriv_float( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void deriv_vec3( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void uniform_float_random( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void uniform_vec3_random( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void float_random( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void vec3_random( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );

}

#endif
