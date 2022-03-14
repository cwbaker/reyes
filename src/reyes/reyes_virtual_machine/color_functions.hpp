#ifndef REYES_COLOR_FUNCTIONS_HPP_INCLUDED
#define REYES_COLOR_FUNCTIONS_HPP_INCLUDED

namespace reyes
{

class Grid;
class Renderer;

void comp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void setcomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void ctransform_function( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );

}

#endif
