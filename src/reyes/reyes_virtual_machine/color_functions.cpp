//
// color_functions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "color_functions.hpp"
#include "ctransform.hpp"
#include "Dispatch.hpp"
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/ErrorCode.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/assert.hpp>
#include <math/scalar.ipp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>

using std::string;
using namespace math;

namespace reyes
{

void comp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    float* result = reinterpret_cast<float*>( arguments[0] );
    const vec3* color = reinterpret_cast<const vec3*>( arguments[1] );
    const int index = int(*reinterpret_cast<const float*>( arguments[2] ));
    const int size = grid.size();
    switch ( index )
    {
        case 0:
            for ( int i = 0; i < size; ++i )
            {
                result[i] = color[i].x;
            }
            break;
                    
        case 1:
            for ( int i = 0; i < size; ++i )
            {
                result[i] = color[i].y;
            }
            break;
                    
        case 2:
        default:
            for ( int i = 0; i < size; ++i )
            {
                result[i] = color[i].z;
            }
            break;                    
    }
}

void setcomp( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* color = reinterpret_cast<vec3*>( arguments[1] );
    int index = int( *reinterpret_cast<float*>(arguments[2]) );
    const float* value = reinterpret_cast<float*>( arguments[3] );
    const int size = grid.size();
    switch ( index )
    {
        case 0:
            for ( int i = 0; i < size; ++i )
            {
                color[i].x = value[i];
            }
            break;

        case 1:
            for ( int i = 0; i < size; ++i )
            {
                color[i].y = value[i];
            }
            break;

        case 2:
        default:
            for ( int i = 0; i < size; ++i )
            {
                color[i].z = value[i];
            }
            break;
    }
}

void ctransform_function( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* result = reinterpret_cast<vec3*>( arguments[0] );
    string fromspace = *reinterpret_cast<string*>( arguments[1] );
    const vec3* color = reinterpret_cast<const vec3*>( arguments[2] );
    // The dispatch and fromspace parameters need to be calculated correctly.
    assert( false );    
    ctransform( 
        // color->size() == 1 ? DISPATCH_U3 : DISPATCH_V3,
        DISPATCH_V3,
        result,
        fromspace.c_str(),
        color,
        grid.size()
    );
}

}
