//
// Value.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_VALUE_HPP_INCLUDED
#define SWEET_RENDER_VALUE_HPP_INCLUDED

#include "declspec.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <sweet/pointer/ptr.hpp>
#include <sweet/math/vec2.hpp>
#include <sweet/math/vec3.hpp>
#include <sweet/math/vec4.hpp>
#include <sweet/math/mat4x4.hpp>
#include <string>
#include <vector>

namespace sweet
{

namespace render
{

class Grid;
class Light;
class Texture;
class Renderer;

/**
// A value stored by the renderer for a shader parameter or a varying variable
// used in a diced grid of micropolygon vertices.
*/
class SWEET_RENDER_DECLSPEC Value
{
    ValueType type_; ///< The type stored in this value.
    ValueStorage storage_; ///< The storage of this value.
    std::string string_value_; ///< The string value of this value.
    void* values_; ///< A pointer to the buffer of floating point values that this value can use.
    unsigned int size_; ///< The number of values stored in this value.
    unsigned int capacity_; ///< The capacity of this value.

public:
    Value();
    Value( const Value& value );
    Value& operator=( const Value& value );
    Value( ValueType type, ValueStorage storage );
    Value( ValueType type, ValueStorage storage, unsigned int capacity );
    ~Value();
    
    Value& operator=( float value );
    Value& operator=( const math::vec3& value );
    Value& operator=( const math::mat4x4& value );
    Value& operator=( const char* value );
    
    ValueType type() const;
    ValueStorage storage() const;
    void* values() const;
    unsigned int element_size() const;

    void set_size( unsigned int size );
    unsigned int size() const;
    
    void zero();
    void clear();
    void reserve( unsigned int capacity );
    void reset( ValueType type, ValueStorage storage, unsigned int capacity );
    void convert( ValueType type );

    bool empty() const;
    void set_string( const std::string& value );
    const std::string& string_value() const;
    const math::mat4x4& mat4x4_value() const;
    int* int_values() const;
    float* float_values() const;
    math::vec3* vec3_values() const;
    math::mat4x4* mat4x4_values() const;
    float float_value() const;
    math::vec3 vec3_value() const;
    
    void float_to_vec3( ValueType type, ptr<Value> value );
    void float_to_mat4x4( ptr<Value> value );
    void light_to_surface_vector( ptr<Value> position, const math::vec3& light_position );
    void surface_to_light_vector( ptr<Value> position, const Light* light );
    void illuminance_axis_angle( ptr<Value> position, ptr<Value> axis, ptr<Value> angle, const Light* light );
    void logical_and( ptr<Value> lhs, ptr<Value> rhs );
    void logical_or( ptr<Value> lhs, ptr<Value> rhs );
    void transform( const math::mat4x4& m, ptr<Value> p );
    void vtransform( const math::mat4x4& m, ptr<Value> v );
    void ntransform( const math::mat4x4& m, ptr<Value> v );
    void transform_matrix( const math::mat4x4& m, ptr<Value> value );
    
    void promote_integer( int size, ptr<Value> value );
    void promote_float( int size, ptr<Value> value );
    void promote_vec3( int size, ptr<Value> value );

    void assign_integer( ptr<Value> value, const unsigned char* mask );
    void assign_float( ptr<Value> value, const unsigned char* mask );
    void assign_vec3( ptr<Value> value, const unsigned char* mask );
    void assign_mat4x4( ptr<Value> value, const unsigned char* mask );
    void assign_string( ptr<Value> value, const unsigned char* mask );

    void add_assign_float( ptr<Value> value, const unsigned char* mask );
    void add_assign_vec3( ptr<Value> value, const unsigned char* mask );

    void multiply_assign_float( ptr<Value> value, const unsigned char* mask );
    void multiply_assign_vec3( ptr<Value> value, const unsigned char* mask );

    void equal_float( ptr<Value> lhs, ptr<Value> rhs );
    void equal_vec3( ptr<Value> lhs, ptr<Value> rhs );

    void not_equal_float( ptr<Value> lhs, ptr<Value> rhs );
    void not_equal_vec3( ptr<Value> lhs, ptr<Value> rhs );

    void greater_float( ptr<Value> lhs, ptr<Value> rhs );
    void greater_equal_float( ptr<Value> lhs, ptr<Value> rhs );
    void less_float( ptr<Value> lhs, ptr<Value> rhs );
    void less_equal_float( ptr<Value> lhs, ptr<Value> rhs );
    void inside_cone( ptr<Value> direction, const math::vec3& axis, float angle );

    void negate_float( ptr<Value> value );
    void negate_vec3( ptr<Value> value );

    void add_float( ptr<Value> lhs, ptr<Value> rhs );
    void add_vec3( ptr<Value> lhs, ptr<Value> rhs );
    
    void subtract_float( ptr<Value> lhs, ptr<Value> rhs );
    void subtract_vec3( ptr<Value> lhs, ptr<Value> rhs );

    void dot_vec3( ptr<Value> lhs, ptr<Value> rhs );

    void multiply_float( ptr<Value> lhs, ptr<Value> rhs );
    void multiply_vec3( ptr<Value> lhs, ptr<Value> rhs );

    void divide_float( ptr<Value> lhs, ptr<Value> rhs );
    void divide_vec3( ptr<Value> lhs, ptr<Value> rhs );
    
private:
    void allocate();
};

}

}

#endif