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
    
    void light_to_surface_vector( std::shared_ptr<Value> position, const math::vec3& light_position );
    void surface_to_light_vector( std::shared_ptr<Value> position, const Light* light );
    void illuminance_axis_angle( std::shared_ptr<Value> position, std::shared_ptr<Value> axis, std::shared_ptr<Value> angle, const Light* light );
    void transform_matrix( const math::mat4x4& m, std::shared_ptr<Value> value );
    void assign_string( std::shared_ptr<Value> value, const unsigned char* mask );
    
private:
    void allocate();
};

}

#endif
