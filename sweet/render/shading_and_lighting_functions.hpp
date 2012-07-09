//
// shading_and_lighting_functions.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_SHADING_AND_LIGHTING_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void ambient( const Renderer& renderer, const Grid& grid, ptr<Value> result );
void diffuse( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> n );
void specular( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> n, ptr<Value> v, ptr<Value> roughness );
void specularbrdf( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> l, ptr<Value> n, ptr<Value> v, ptr<Value> roughness );
void phong( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> normal, ptr<Value> view, ptr<Value> size_value );
void trace( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> point, ptr<Value> reflection );

}

}

#endif