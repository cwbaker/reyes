//
// SetValueHelper.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "SetValueHelper.hpp"
#include "Grid.hpp"
#include "assert.hpp"
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>

using std::string;
using math::vec3;
using math::mat4x4;
using namespace reyes;

SetValueHelper::SetValueHelper()
: grid_( nullptr )
, offset_( 0 )
{
}

SetValueHelper::SetValueHelper( Grid* grid, int offset )
: grid_( grid )
, offset_( offset )
{
    REYES_ASSERT( grid_ );
    REYES_ASSERT( offset_ >= 0 );
}

SetValueHelper& SetValueHelper::operator=( float value )
{
    if ( grid_ )
    {
        float* other_value = grid_->float_value( offset_ );
        REYES_ASSERT( other_value );
        other_value[0] = value;
    }
    return *this;
}

SetValueHelper& SetValueHelper::operator=( const math::vec3& value )
{
    if ( grid_ )
    {
        vec3* other_value = grid_->vec3_value( offset_ );
        REYES_ASSERT( other_value );
        other_value[0] = value;
    }
    return *this;
}

SetValueHelper& SetValueHelper::operator=( const math::mat4x4& value )
{
    if ( grid_ )
    {
        mat4x4* other_value = grid_->mat4x4_value( offset_ );
        REYES_ASSERT( other_value );
        other_value[0] = value;
    }
    return *this;
}

SetValueHelper& SetValueHelper::operator=( const char* value )
{
    assert( value );
    if ( grid_ )
    {
        grid_->set_string( offset_, string(value) );
    }
    return *this;
}

SetValueHelper& SetValueHelper::operator=( const std::string& value )
{
    if ( grid_ )
    {
        grid_->set_string( offset_, value );
    }
    return *this;
}
