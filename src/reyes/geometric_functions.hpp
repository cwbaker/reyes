#ifndef REYES_GEOMETRIC_FUNCTIONS_HPP_INCLUDED
#define REYES_GEOMETRIC_FUNCTIONS_HPP_INCLUDED

#include <memory>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void xcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void ycomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void zcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );

void setxcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> p, std::shared_ptr<Value> x );
void setycomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> p, std::shared_ptr<Value> x );
void setzcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> p, std::shared_ptr<Value> x );

void length( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void normalize( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> x );
void distance( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p0, std::shared_ptr<Value> p1 );
void ptlined( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p0, std::shared_ptr<Value> p1 );
void rotate( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> q, std::shared_ptr<Value> angle, std::shared_ptr<Value> p0, std::shared_ptr<Value> p1 );
void area( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void faceforward_vv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n, std::shared_ptr<Value> i );
void faceforward_vvv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n, std::shared_ptr<Value> i, std::shared_ptr<Value> nref );
void reflect( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> i, std::shared_ptr<Value> n );
void refract( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> i, std::shared_ptr<Value> n, std::shared_ptr<Value> eta_value );
void fresnel( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> incident, std::shared_ptr<Value> normal, std::shared_ptr<Value> eta_value, std::shared_ptr<Value> Kr, std::shared_ptr<Value> Kt );
void transform_sv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p );
void transform_ssv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p );
void transform_mv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> m, std::shared_ptr<Value> p );
void transform_smv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> m, std::shared_ptr<Value> p );
void vtransform_sv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p );
void vtransform_ssv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p );
void vtransform_mv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> m, std::shared_ptr<Value> p );
void vtransform_smv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> m, std::shared_ptr<Value> p );
void ntransform_sv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p );
void ntransform_ssv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> tospace, std::shared_ptr<Value> p );
void ntransform_mv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> m, std::shared_ptr<Value> p );
void ntransform_smv( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> m, std::shared_ptr<Value> p );
void depth( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );
void calculatenormal( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> p );

}

}

#endif
