//
// geometric_functions.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_GEOMETRIC_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void xcomp( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void ycomp( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void zcomp( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );

void setxcomp( const Renderer& renderer, const Grid& grid, ptr<Value> p, ptr<Value> x );
void setycomp( const Renderer& renderer, const Grid& grid, ptr<Value> p, ptr<Value> x );
void setzcomp( const Renderer& renderer, const Grid& grid, ptr<Value> p, ptr<Value> x );

void length( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void normalize( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> x );
void distance( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p0, ptr<Value> p1 );
void ptlined( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p0, ptr<Value> p1 );
void rotate( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> q, ptr<Value> angle, ptr<Value> p0, ptr<Value> p1 );
void area( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void faceforward_vv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> n, ptr<Value> i );
void faceforward_vvv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> n, ptr<Value> i, ptr<Value> nref );
void reflect( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> i, ptr<Value> n );
void refract( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> i, ptr<Value> n, ptr<Value> eta_value );
void fresnel( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> incident, ptr<Value> normal, ptr<Value> eta_value, ptr<Value> Kr, ptr<Value> Kt );
void transform_sv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> tospace, ptr<Value> p );
void transform_ssv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> tospace, ptr<Value> p );
void transform_mv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> m, ptr<Value> p );
void transform_smv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> m, ptr<Value> p );
void vtransform_sv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> tospace, ptr<Value> p );
void vtransform_ssv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> tospace, ptr<Value> p );
void vtransform_mv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> m, ptr<Value> p );
void vtransform_smv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> m, ptr<Value> p );
void ntransform_sv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> tospace, ptr<Value> p );
void ntransform_ssv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> tospace, ptr<Value> p );
void ntransform_mv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> m, ptr<Value> p );
void ntransform_smv( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> m, ptr<Value> p );
void depth( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );
void calculatenormal( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> p );

}

}

#endif