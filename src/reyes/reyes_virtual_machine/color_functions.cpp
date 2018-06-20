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

using std::shared_ptr;
using namespace math;

namespace reyes
{

void comp( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> color, std::shared_ptr<Value> index_value )
{
    REYES_ASSERT( result );
    REYES_ASSERT( color );
    REYES_ASSERT( index_value );
    REYES_ASSERT( index_value->storage() == STORAGE_UNIFORM );
    
    result->reset( TYPE_FLOAT, color->storage(), color->size() );
    
    const int size = color->size();
    float* values = result->float_values();
    const vec3* colors = color->vec3_values();
    int index = int(index_value->float_value());
    switch ( index )
    {
        case 0:
            for ( int i = 0; i < size; ++i )
            {
                values[i] = colors[i].x;
            }
            break;
                    
        case 1:
            for ( int i = 0; i < size; ++i )
            {
                values[i] = colors[i].y;
            }
            break;
                    
        case 2:
        default:
            for ( int i = 0; i < size; ++i )
            {
                values[i] = colors[i].z;
            }
            break;                    
    }
}

void setcomp( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> /*result*/, std::shared_ptr<Value> color, std::shared_ptr<Value> index_value, std::shared_ptr<Value> value )
{
    REYES_ASSERT( color );
    REYES_ASSERT( index_value );
    REYES_ASSERT( index_value->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( color->size() == value->size() );

    const int size = color->size();
    vec3* colors = color->vec3_values();
    const float* values = value->float_values();
    int index = int(index_value->float_value());

    switch ( index )
    {
        case 0:
            for ( int i = 0; i < size; ++i )
            {
                colors[i].x = values[i];
            }
            break;

        case 1:
            for ( int i = 0; i < size; ++i )
            {
                colors[i].y = values[i];
            }
            break;

        case 2:
        default:
            for ( int i = 0; i < size; ++i )
            {
                colors[i].z = values[i];
            }
            break;
    }
}

void ctransform_function( const Renderer& /*renderer*/, const Grid& /*grid*/, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> color )
{
    REYES_ASSERT( result );
    REYES_ASSERT( fromspace );
    REYES_ASSERT( fromspace->type() == TYPE_STRING );
    REYES_ASSERT( color );    
    result->reset( TYPE_COLOR, color->storage(), color->size() );
    ctransform( 
        color->size() == 1 ? DISPATCH_U3 : DISPATCH_V3,
        result->vec3_values(),
        fromspace->string_value().c_str(),
        color->vec3_values(),
        color->size()
    );
}

}
