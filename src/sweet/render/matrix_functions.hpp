//
// matrix_functions.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_MATRIX_FUNCTIONS_HPP_INCLUDED
#define SWEET_RENDER_MATRIX_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void comp_matrix( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> matrix, ptr<Value> row, ptr<Value> column );
void setcomp_matrix( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> matrix, ptr<Value> row, ptr<Value> column, ptr<Value> value );
void determinant( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> matrix );
void translate_matrix( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> matrix, ptr<Value> t );
void rotate_matrix( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> matrix, ptr<Value> angle, ptr<Value> axis );
void scale_matrix( const Renderer& render, const Grid& grid, ptr<Value> result, ptr<Value> matrix, ptr<Value> s );

}

}

#endif
