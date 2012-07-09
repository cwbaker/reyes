//
// color_functions.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_COLOR_FUNCTIONS_HPP_INCLUDED
#define SWEET_RENDER_COLOR_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void comp( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> c, ptr<Value> index );
void setcomp( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> c, ptr<Value> index, ptr<Value> value );
void ctransform( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> fromspace, ptr<Value> color );

}

}

#endif
