#ifndef REYES_PARABOLOID_HPP_INCLUDED
#define REYES_PARABOLOID_HPP_INCLUDED

#include "Geometry.hpp"
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <list>

namespace reyes
{

class Grid;

class Paraboloid : public Geometry
{
    float rmax_;
    float zmin_;
    float zmax_;
    float thetamax_;
    
public:
    Paraboloid( float rmax, float zmin, float zmax, float thetamax );
    Paraboloid( const Paraboloid& paraboloid, const math::vec2& u_range, const math::vec2& v_range );

    bool boundable() const;
    void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum ) const;
    bool splittable() const;
    void split( std::list<std::shared_ptr<Geometry>>* primitives ) const;
    bool diceable() const;
    void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const;

private:
    math::vec3 position( float u, float v ) const;
};

}

#endif
