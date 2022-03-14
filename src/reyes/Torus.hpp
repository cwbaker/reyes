#ifndef REYES_TORUS_HPP_INCLUDED
#define REYES_TORUS_HPP_INCLUDED

#include "Geometry.hpp"
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <list>

namespace reyes
{

class Grid;

class Torus : public Geometry
{
    float rmajor_;
    float rminor_;
    float phimin_;
    float phimax_;
    float thetamax_;
    
public:
    Torus( float rmajor, float rminor, float phimin, float phimax, float thetamax );
    Torus( const Torus& torus, const math::vec2& u_range, const math::vec2& v_range );

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

#endif
