#ifndef REYES_DISK_HPP_INCLUDED
#define REYES_DISK_HPP_INCLUDED

#include "Geometry.hpp"
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <list>
#include <memory>

namespace reyes
{

class Grid;

class Disk : public Geometry
{
    float height_;
    float radius_;
    float thetamax_;
    
public:
    Disk( float height, float radius, float thetamax );
    Disk( const Disk& disk, const math::vec2& u_range, const math::vec2& v_range );

    bool boundable() const override;
    void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum, Grid* grid ) const override;
    bool splittable() const override;
    void split( std::list<std::shared_ptr<Geometry>>* primitives ) const override;
    bool diceable() const override;
    void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const override;

private:
    math::vec3 position( float u, float v ) const;
};

}

#endif
