#ifndef REYES_VALUE_HPP_INCLUDED
#define REYES_VALUE_HPP_INCLUDED

#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/vec4.hpp>
#include <math/mat4x4.hpp>
#include <string>
#include <vector>
#include <memory>

namespace reyes
{

class Grid;
class Light;
class Texture;
class Renderer;

/**
// A value stored by the renderer for a shader parameter or a varying variable
// used in a diced grid of micropolygon vertices.
*/
class Value
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
    
    void float_to_vec3( ValueType type, std::shared_ptr<Value> value );
    void float_to_mat4x4( std::shared_ptr<Value> value );
    void light_to_surface_vector( std::shared_ptr<Value> position, const math::vec3& light_position );
    void surface_to_light_vector( std::shared_ptr<Value> position, const Light* light );
    void illuminance_axis_angle( std::shared_ptr<Value> position, std::shared_ptr<Value> axis, std::shared_ptr<Value> angle, const Light* light );
    void logical_and( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void logical_or( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void transform( const math::mat4x4& m, std::shared_ptr<Value> p );
    void vtransform( const math::mat4x4& m, std::shared_ptr<Value> v );
    void ntransform( const math::mat4x4& m, std::shared_ptr<Value> v );
    void transform_matrix( const math::mat4x4& m, std::shared_ptr<Value> value );
    
    void promote_integer( int size, std::shared_ptr<Value> value );
    void promote_float( int size, std::shared_ptr<Value> value );
    void promote_vec3( int size, std::shared_ptr<Value> value );

    void assign_integer( std::shared_ptr<Value> value, const unsigned char* mask );
    void assign_float( std::shared_ptr<Value> value, const unsigned char* mask );
    void assign_vec3( std::shared_ptr<Value> value, const unsigned char* mask );
    void assign_mat4x4( std::shared_ptr<Value> value, const unsigned char* mask );
    void assign_string( std::shared_ptr<Value> value, const unsigned char* mask );

    void add_assign_float( std::shared_ptr<Value> value, const unsigned char* mask );
    void add_assign_vec3( std::shared_ptr<Value> value, const unsigned char* mask );

    void multiply_assign_float( std::shared_ptr<Value> value, const unsigned char* mask );
    void multiply_assign_vec3( std::shared_ptr<Value> value, const unsigned char* mask );

    void equal_float( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void equal_vec3( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );

    void not_equal_float( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void not_equal_vec3( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );

    void greater_float( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void greater_equal_float( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void less_float( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void less_equal_float( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    void inside_cone( std::shared_ptr<Value> direction, const math::vec3& axis, float angle );

    void dot_vec3( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs );
    
private:
    void allocate();
};

}

#endif
