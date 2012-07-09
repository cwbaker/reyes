//
// persist.hpp
// Copyright (c) 2010 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_MATH_PERSIST_HPP_INCLUDED
#define SWEET_MATH_PERSIST_HPP_INCLUDED

#include "quat.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace sweet
{

namespace math
{

template <class Archive> void quat::persist( Archive& archive )
{
    archive.flag( sweet::persist::PERSIST_ANONYMOUS );
    archive.value( "x", x );
    archive.value( "y", y );
    archive.value( "z", z );
    archive.value( "w", w );
}

template <class Archive> void vec2::persist( Archive& archive )
{
    archive.flag( sweet::persist::PERSIST_ANONYMOUS );
    archive.value( "x", x );
    archive.value( "y", y );
}

template <class Archive> void vec3::persist( Archive& archive )
{
    archive.flag( sweet::persist::PERSIST_ANONYMOUS );
    archive.value( "x", x );
    archive.value( "y", y );
    archive.value( "z", z );
}

template <class Archive> void vec4::persist( Archive& archive )
{
    archive.flag( sweet::persist::PERSIST_ANONYMOUS );
    archive.value( "x", x );
    archive.value( "y", y );
    archive.value( "z", z );
    archive.value( "w", w );
}

}

}

#endif