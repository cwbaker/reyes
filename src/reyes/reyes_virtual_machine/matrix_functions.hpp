#pragma once

namespace reyes
{

class Grid;
class Renderer;

void comp_matrix( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void setcomp_matrix( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void determinant( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );
void translate_matrix( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );
void rotate_matrix( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );
void scale_matrix( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );

}
