#ifndef REYES_GEOMETRIC_FUNCTIONS_HPP_INCLUDED
#define REYES_GEOMETRIC_FUNCTIONS_HPP_INCLUDED

namespace reyes
{

class Grid;
class Renderer;

void xcomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void ycomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void zcomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void setxcomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void setycomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void setzcomp( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void length( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void normalize( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void distance( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void ptlined( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void rotate( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void area( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void faceforward_vv( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void faceforward_vvv( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void reflect( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );
void refract( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );
void fresnel( const Renderer& render, const Grid& grid, int /*dispatch*/, void** arguments );
void transform_sv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void transform_ssv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void transform_mv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void transform_smv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void vtransform_sv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void vtransform_ssv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void vtransform_mv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void vtransform_smv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void ntransform_sv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void ntransform_ssv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void ntransform_mv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void ntransform_smv( const Renderer& renderer, const Grid& grid, int dispatch, void** arguments );
void depth( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );
void calculatenormal( const Renderer& renderer, const Grid& grid, int /*dispatch*/, void** arguments );

}

#endif
