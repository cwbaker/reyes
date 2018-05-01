
#include "stdafx.hpp"
#include "color_functions.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include "Renderer.hpp"
#include "Error.hpp"
#include "ErrorPolicy.hpp"
#include <sweet/math/scalar.ipp>
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/assert/assert.hpp>

using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;

namespace sweet
{

namespace render
{

void comp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> color, std::shared_ptr<Value> index_value )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( color );
    SWEET_ASSERT( index_value );
    SWEET_ASSERT( index_value->storage() == STORAGE_UNIFORM );
    
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

void setcomp( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> color, std::shared_ptr<Value> index_value, std::shared_ptr<Value> value )
{
    SWEET_ASSERT( color );
    SWEET_ASSERT( index_value );
    SWEET_ASSERT( index_value->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( color->size() == value->size() );

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

void ctransform( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> fromspace, std::shared_ptr<Value> color )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( fromspace );
    SWEET_ASSERT( fromspace->type() == TYPE_STRING );
    SWEET_ASSERT( color );
    
    result->reset( TYPE_COLOR, color->storage(), color->size() );
    
    const int size = color->size();
    const vec3* other_colors = color->vec3_values();
    vec3* colors = result->vec3_values();
    
    if ( fromspace->string_value() == "hsv" )
    {
        for ( int i = 0; i < size; ++i )
        {
            colors[i] = rgb_from_hsv( other_colors[i] );
        }
    }
    else if ( fromspace->string_value() == "hsl" )
    {
        for ( int i = 0; i < size; ++i )
        {
            colors[i] = rgb_from_hsl( other_colors[i] );
        }
    }
    else
    {
        renderer.error_policy().error( RENDER_ERROR_UNKNOWN_COLOR_SPACE, "Unknown color space '%s'", fromspace->string_value().c_str() );
    }
}

}

}
