#ifndef REYES_GEOMETRY_HPP_INCLUDED
#define REYES_GEOMETRY_HPP_INCLUDED

#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <list>
#include <memory>

namespace reyes
{

class Grid;

/**
// The base class for geometry types supported by the renderer.
*/
class Geometry
{
    math::vec2 u_range_; ///< The range in u covered by this piece of geometry.
    math::vec2 v_range_; ///< The range in v covered by this piece of geometry.
    
public:
    Geometry( const math::vec2& u_range, const math::vec2& v_range );
    virtual ~Geometry();

    const math::vec2& u_range() const;
    const math::vec2& v_range() const;
    
    virtual bool boundable() const;
    virtual void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum, Grid* grid ) const;
    virtual bool splittable() const;
    virtual void split( std::list<std::shared_ptr<Geometry>>* primitives ) const;
    virtual bool diceable() const;
    virtual void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const;
};

}

#endif
