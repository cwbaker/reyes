#pragma once

#include "Geometry.hpp"
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <list>
#include <memory>

namespace reyes
{

class Grid;

class CubicPatch : public Geometry
{
    const math::vec3* p_;
    const math::vec4* u_basis_;
    const math::vec4* v_basis_;
    
public:
    CubicPatch( const math::vec3* positions, const math::vec4* u_basis, const math::vec4* v_basis );
    CubicPatch( const CubicPatch& patch, const math::vec2& u_range, const math::vec2& v_range );
    
    bool boundable() const override;
    void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum, Grid* grid ) const override;
    bool splittable() const override;
    void split( std::list<std::shared_ptr<Geometry>>* primitives ) const override;
    bool diceable() const override;
    void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const override;        

private:
    math::vec3 position( float u, float v ) const;
    math::vec3 normal( float u, float v ) const;    
};

}
