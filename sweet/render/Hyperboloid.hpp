//
// Hyperboloid.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_HYPERBOLOID_HPP_INCLUDED
#define SWEET_RENDER_HYPERBOLOID_HPP_INCLUDED

#include "declspec.hpp"
#include "Geometry.hpp"
#include <sweet/math/vec2.hpp>
#include <sweet/math/vec3.hpp>
#include <sweet/math/mat4x4.hpp>
#include <list>

namespace sweet
{

namespace render
{

class Grid;

class SWEET_RENDER_DECLSPEC Hyperboloid : public Geometry
{
    math::vec3 point1_;
    math::vec3 point2_;
    float thetamax_;
    
public:
    Hyperboloid( const math::vec3& point1, const math::vec3& point2, float thetamax );
    Hyperboloid( const Hyperboloid& hyperboloid, const math::vec2& u_range, const math::vec2& v_range );

    bool boundable() const;
    void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum ) const;
    bool splittable() const;
    void split( std::list<ptr<Geometry>>* primitives ) const;
    bool diceable() const;
    void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const;

private:
    math::vec3 position( float u, float v ) const;
};

}

}

#endif
