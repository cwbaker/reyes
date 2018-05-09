#ifndef SWEET_RENDER_COLOR_FUNCTIONS_HPP_INCLUDED
#define SWEET_RENDER_COLOR_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include <memory>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void comp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> c, std::shared_ptr<Value> index );
void setcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> c, std::shared_ptr<Value> index, std::shared_ptr<Value> value );
void ctransform( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> color );

}

}

#endif
