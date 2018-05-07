#ifndef SWEET_RENDER_LINEARPATCH_HPP_INCLUDED
#define SWEET_RENDER_LINEARPATCH_HPP_INCLUDED

#include "declspec.hpp"
#include "Geometry.hpp"
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <list>
#include <memory>

namespace sweet
{

namespace render
{

class Grid;

class SWEET_RENDER_DECLSPEC LinearPatch : public Geometry
{
    math::vec3 positions_ [4];
    math::vec3 normals_ [4];
    math::vec2 texture_coordinates_ [4];
    
public:
    LinearPatch( const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates );
    LinearPatch( const LinearPatch& patch, const math::vec2& u_range, const math::vec2& v_range );        

    bool boundable() const;
    void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum ) const;
    bool splittable() const;
    void split( std::list<std::shared_ptr<Geometry>>* primitives ) const;
    bool diceable() const;
    void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const;        

private:    
    math::vec3 bilerp( const math::vec3* x, float u, float v ) const;
    math::vec2 bilerp( const math::vec2* x, float u, float v ) const;
};

}

}

#endif
