//
// Geometry.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_GEOMETRY_HPP_INCLUDED
#define SWEET_RENDER_GEOMETRY_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/math/vec2.hpp>
#include <sweet/math/vec3.hpp>
#include <sweet/math/mat4x4.hpp>
#include <sweet/pointer/ptr.hpp>
#include <list>

namespace sweet
{

namespace render
{

class Grid;

/**
// The base class for geometry types supported by the renderer.
*/
class SWEET_RENDER_DECLSPEC Geometry
{
    math::vec2 u_range_; ///< The range in u covered by this piece of geometry.
    math::vec2 v_range_; ///< The range in v covered by this piece of geometry.
    
public:
    Geometry( const math::vec2& u_range, const math::vec2& v_range );
    virtual ~Geometry();

    const math::vec2& u_range() const;
    const math::vec2& v_range() const;
    
    virtual bool boundable() const;
    virtual void bound( const math::mat4x4& transform, math::vec3* minimum, math::vec3* maximum ) const;
    virtual bool splittable() const;
    virtual void split( std::list<ptr<Geometry> >* primitives ) const;
    virtual bool diceable() const;
    virtual void dice( const math::mat4x4& transform, int width, int height, Grid* grid ) const;
};

}

}

#endif
