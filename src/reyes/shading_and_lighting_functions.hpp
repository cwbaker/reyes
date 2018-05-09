#ifndef REYES_SHADING_AND_LIGHTING_FUNCTIONS_HPP_INCLUDED
#define REYES_SHADING_AND_LIGHTING_FUNCTIONS_HPP_INCLUDED

#include <memory>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void ambient( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result );
void diffuse( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n );
void specular( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> n, std::shared_ptr<Value> v, std::shared_ptr<Value> roughness );
void specularbrdf( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> l, std::shared_ptr<Value> n, std::shared_ptr<Value> v, std::shared_ptr<Value> roughness );
void phong( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> normal, std::shared_ptr<Value> view, std::shared_ptr<Value> size_value );
void trace( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> point, std::shared_ptr<Value> reflection );

}

}

#endif