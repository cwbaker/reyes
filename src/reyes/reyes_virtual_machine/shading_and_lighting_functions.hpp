#pragma once

namespace reyes
{

class Grid;
class Renderer;

void ambient( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void diffuse( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void specular( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void specularbrdf( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void phong( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void trace( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );

}
