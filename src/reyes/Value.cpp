//
// Value.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Value.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Grid.hpp"
#include "Light.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/vec4.ipp>
#include <math/mat3x3.ipp>
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include "assert.hpp"
#include <math.h>
#include <memory.h>

using std::min;
using std::max;
using std::vector;
using namespace math;
using namespace reyes;

// @todo
//  Make sure that MAXIMUM_VERTICES_PER_GRID in Value.cpp is unified with the
//  same variable in Renderer.
static const int MAXIMUM_VERTICES_PER_GRID = 64 * 64;

Value::Value()
: type_( TYPE_NULL ),
  storage_( STORAGE_NULL ),
  string_value_(),
  values_( NULL ),
  size_( 0 ),
  capacity_( 0 )
{
    allocate();
}

Value::Value( const Value& value )
: type_( value.type_ ),
  storage_( value.storage_ ),
  string_value_( value.string_value_ ),
  values_( NULL ),
  size_( 0 ),
  capacity_( 0 )
{
    allocate();
    if ( value.capacity_ > 0 )
    {
        reserve( value.capacity_ );
        memcpy( values_, value.values_, capacity_ * element_size() );
        size_ = capacity_;
    }
}

Value& Value::operator=( const Value& value )
{
    if ( this != &value )
    {
        type_ = value.type_;
        storage_ = value.storage_;
        string_value_ = value.string_value_;
        size_ = value.size_;
        capacity_ = value.capacity_;
        memcpy( values_, value.values_, size_ * element_size() );
    }
    return *this;
}

Value::Value( ValueType type, ValueStorage storage )
: type_( type ),
  storage_( storage ),
  string_value_(),
  values_( NULL ),
  size_( 0 ),
  capacity_( 0 )
{
    allocate();
}

Value::Value( ValueType type, ValueStorage storage, unsigned int capacity )
: type_( type ),
  storage_( storage ),
  string_value_(),
  values_( NULL ),
  size_( 0 ),
  capacity_( 0 )
{
    allocate();
    reserve( capacity );
    size_ = capacity;
}

Value::~Value()
{
    if ( values_ )
    {
        free( values_ );
        values_ = NULL;
    }
}

Value& Value::operator=( float value )
{
    switch ( type_ )
    {
        case TYPE_FLOAT:
        {
            float* values = float_values();
            values[0] = value;
            storage_ = STORAGE_UNIFORM;
            size_ = 1;
            break;
        }
        
        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
        {
            vec3* values = vec3_values();
            values[0] = vec3( value, value, value );
            storage_ = STORAGE_UNIFORM;
            size_ = 1;
            break;
        }
        
        default:
            REYES_ASSERT( false );
            break;
    }
    
    return *this;
}

Value& Value::operator=( const math::vec3& value )
{
    vec3* values = vec3_values();
    values[0] = value;        
    storage_ = STORAGE_UNIFORM;
    size_ = 1;
    return *this;
}

Value& Value::operator=( const math::mat4x4& value )
{
    mat4x4* values = mat4x4_values();
    values[0] = value;    
    storage_ = STORAGE_UNIFORM;
    size_ = 1;
    return *this;
}

Value& Value::operator=( const char* value )
{
    REYES_ASSERT( value );
    reset( TYPE_STRING, STORAGE_UNIFORM, 1 );
    string_value_ = value;
    return *this;
}

ValueType Value::type() const
{
    return type_;
}

ValueStorage Value::storage() const
{
    return storage_;
}

void* Value::values() const
{
    return values_;
}

unsigned int Value::element_size() const
{
    unsigned int size = 0;
    switch ( type_ )
    {
        case TYPE_INTEGER:  
            size = sizeof(int);
            break;
    
        case TYPE_FLOAT:
            size = sizeof(float);
            break;
            
        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
            size = sizeof(math::vec3);
            break;
            
        case TYPE_MATRIX:
            size = sizeof(math::mat4x4);
            break;
            
        case TYPE_STRING:
            size = 1;
            break;
            
        default:
            REYES_ASSERT( false );
            break;
    }
    return size;
}

void Value::set_size( unsigned int size )
{
    REYES_ASSERT( storage_ == STORAGE_VARYING );
    size_ = size;
}

unsigned int Value::size() const
{
    return size_;
}

void Value::zero()
{
    REYES_ASSERT( values_ );
    REYES_ASSERT( capacity_ > 0 );
    memset( values_, 0, capacity_ * element_size() );
    size_ = capacity_;
}

void Value::clear()
{
    size_ = 0;
    capacity_ = 0;
}

void Value::reserve( unsigned int capacity )
{
    capacity_ = capacity;
    size_ = capacity;
}

void Value::reset( ValueType type, ValueStorage storage, unsigned int capacity )
{
    type_ = type;
    storage_ = storage;    
    capacity_ = capacity;
    size_ = capacity;
}

void Value::convert( ValueType type )
{
    REYES_ASSERT( storage_ == STORAGE_CONSTANT || storage_ == STORAGE_UNIFORM );
    
    if ( type == TYPE_COLOR || type == TYPE_POINT || type == TYPE_VECTOR || type == TYPE_NORMAL )
    {
        REYES_ASSERT( type == TYPE_FLOAT || type == TYPE_COLOR || type == TYPE_POINT || type == TYPE_VECTOR || type == TYPE_NORMAL );
        if ( type_ == TYPE_FLOAT )
        {
            float value = *float_values();
            reset( type, storage_, 1 );
            vec3* values = vec3_values();
            values[0] = vec3( value, value, value );
        }
    }
}

bool Value::empty() const
{
    return size_ == 0;
}

void Value::set_string( const std::string& value )
{
    reset( TYPE_STRING, STORAGE_UNIFORM, 1 );
    string_value_ = value;
}

const std::string& Value::string_value() const
{
    REYES_ASSERT( type_ == TYPE_STRING );
    return string_value_;
}

const math::mat4x4& Value::mat4x4_value() const
{
    REYES_ASSERT( storage_ == STORAGE_UNIFORM );
    return mat4x4_values()[0];
}

int* Value::int_values() const
{
    REYES_ASSERT( type_ == TYPE_INTEGER );
    return reinterpret_cast<int*>( values_ );
}

float* Value::float_values() const
{
    REYES_ASSERT( type_ == TYPE_FLOAT );
    return reinterpret_cast<float*>( values_ );
}

math::vec3* Value::vec3_values() const
{
    REYES_ASSERT( type_ == TYPE_COLOR || type_ == TYPE_POINT || type_ == TYPE_VECTOR || type_ == TYPE_NORMAL );
    return reinterpret_cast<math::vec3*>( values_ );
}

math::mat4x4* Value::mat4x4_values() const
{
    REYES_ASSERT( type_ == TYPE_MATRIX );
    return reinterpret_cast<math::mat4x4*>( values_ );
}

float Value::float_value() const
{
    REYES_ASSERT( type_ == TYPE_FLOAT );
    REYES_ASSERT( storage_ == STORAGE_UNIFORM );
    REYES_ASSERT( size_ == 1 );
    return *(const float*) values_;
}

math::vec3 Value::vec3_value() const
{
    REYES_ASSERT( type_ == TYPE_COLOR || type_ == TYPE_POINT || type_ == TYPE_VECTOR || type_ == TYPE_NORMAL );
    REYES_ASSERT( storage_ == STORAGE_UNIFORM );
    REYES_ASSERT( size_ == 1 );
    return *(const vec3*) values_;
}

void Value::float_to_vec3( ValueType type, std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_FLOAT );
    REYES_ASSERT( type == TYPE_COLOR || type == TYPE_POINT || type == TYPE_VECTOR || type == TYPE_NORMAL );
    
    reset( type, value->storage(), value->size() );
    size_ = value->size();
    
    const int size = size_;
    vec3* values = vec3_values();
    const float* other_values = value->float_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = vec3( other_values[i], other_values[i], other_values[i] );
    }
}

void Value::float_to_mat4x4( std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_FLOAT );

    reset( TYPE_MATRIX, value->storage(), value->size() );
    size_ = value->size();
    
    const int size = size_;
    mat4x4* values = mat4x4_values();
    const float* other_values = value->float_values();
    for ( int i = 0; i < size; ++i )
    {
        float x = other_values[i];
        values[i] = mat4x4( 
            x, 0.0f, 0.0f, 0.0f,
            0.0f, x, 0.0f, 0.0f,
            0.0f, 0.0f, x, 0.0f,
            0.0f, 0.0f, 0.0f, x
        );
    }
}

/**
// Calculate the light direction from the light at \e light_position (the
// explicit position expression of an illuminate statement) to the surface 
// at \e position (the implicit "Ps" value passed to a light shader).
//
// This is used to calculate "L" in a light shader's illuminate statement.
*/
void Value::light_to_surface_vector( std::shared_ptr<Value> position, const math::vec3& light_position )
{
    REYES_ASSERT( position );
    REYES_ASSERT( position->storage() == STORAGE_VARYING );
    
    reset( TYPE_VECTOR, position->storage(), position->size() );
    size_ = position->size();
    
    const int size = size_;
    vec3* values = vec3_values();
    vec3* positions = position->vec3_values();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = positions[i] - light_position;
    }
}

/**
// Calculate the light direction from the surface (the implicit "P" value in
// a surface shader) \e position to \e light.
//
// This is used to calculate "L" in a surface shader's illuminance statement 
// from the surface position and the currently active light.
*/
void Value::surface_to_light_vector( std::shared_ptr<Value> position, const Light* light )
{
    REYES_ASSERT( position );
    REYES_ASSERT( position->storage() == STORAGE_VARYING );
    REYES_ASSERT( light );
    
    reset( TYPE_VECTOR, position->storage(), position->size() );
    size_ = position->size();
    
    const int size = size_;
    vec3* values = vec3_values();
    const vec3* positions = position->vec3_values();
    const vec3& light_position = light->position();
    for ( int i = 0; i < size; ++i )
    {
        values[i] = light_position - positions[i];
    }
}

/**
// Calculate a mask based on the axis and angle passed to an illuminance 
// statement and the light from \e light.
*/
void Value::illuminance_axis_angle( std::shared_ptr<Value> position, std::shared_ptr<Value> axis, std::shared_ptr<Value> angle, const Light* light )
{
    REYES_ASSERT( position );
    REYES_ASSERT( position->storage() == STORAGE_VARYING );
    REYES_ASSERT( axis );
    REYES_ASSERT( axis->storage() == STORAGE_VARYING );
    REYES_ASSERT( position->size() == axis->size() );
    REYES_ASSERT( angle );
    REYES_ASSERT( light );
    
    reset( TYPE_INTEGER, position->storage(), position->size() );
    size_ = position->size();

    const int size = size_;
    const float angle_cosine = cosf( angle->float_value() );
    const vec3* axis_values = axis->vec3_values();
    const vec3* positions = position->vec3_values();
    int* values = int_values();

    switch ( light->type() )
    {
        case LIGHT_SOLAR_AXIS:
        case LIGHT_SOLAR_AXIS_ANGLE:
        {
            const vec3 light_direction = math::normalize( -light->position() );
            for ( int i = 0; i < size; ++i )
            {
                values[i] = dot( axis_values[i], light_direction ) >= angle_cosine;
            }
            break;
        }
            
        case LIGHT_ILLUMINATE:
        case LIGHT_ILLUMINATE_AXIS_ANGLE:
        {
            const vec3 light_position = light->position();
            for ( int i = 0; i < size; ++i )
            {
                values[i] = dot( axis_values[i], math::normalize(light_position - positions[i]) ) >= angle_cosine;
            }
            break;
        }
    }        
}

void Value::transform( const math::mat4x4& m, std::shared_ptr<Value> p )
{
    REYES_ASSERT( p );
    REYES_ASSERT( p->type() == TYPE_POINT || p->type() == TYPE_VECTOR || p->type() == TYPE_NORMAL );

    reset( p->type(), p->storage(), p->size() );

    const vec3* p_values = p->vec3_values();
    vec3* values = vec3_values();
    for ( unsigned int i = 0; i < p->size(); ++i )
    {
        values[i] = vec3( m * vec4(p_values[i], 1.0f) );
    }
}

void Value::vtransform( const math::mat4x4& m, std::shared_ptr<Value> v )
{
    REYES_ASSERT( v );
    REYES_ASSERT( v->type() == TYPE_POINT || v->type() == TYPE_VECTOR || v->type() == TYPE_NORMAL );

    reset( v->type(), v->storage(), v->size() );

    const vec3* v_values = v->vec3_values();
    vec3* values = vec3_values();
    for ( unsigned int i = 0; i < v->size(); ++i )
    {
        values[i] = vec3( m * vec4(v_values[i], 0.0f) );
    }
}

void Value::ntransform( const math::mat4x4& mm, std::shared_ptr<Value> n )
{
    REYES_ASSERT( n );
    REYES_ASSERT( n->type() == TYPE_POINT || n->type() == TYPE_VECTOR || n->type() == TYPE_NORMAL );

    reset( n->type(), n->storage(), n->size() );

    mat3x3 m( transpose(inverse(mat3x3(mm))) );
    const vec3* n_values = n->vec3_values();
    vec3* values = vec3_values();
    for ( unsigned int i = 0; i < n->size(); ++i )
    {
        values[i] = m * n_values[i];
    }
}

void Value::transform_matrix( const math::mat4x4& m, std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_MATRIX );
    
    const int size = value->size();
    reset( TYPE_MATRIX, value->storage(), value->size() );
    
    mat4x4* values = mat4x4_values();
    const mat4x4* other_values = value->mat4x4_values();
    for ( unsigned int i = 0; i < size; ++i )
    {
        values[i] = m * other_values[i];
    }
}

void Value::assign_vec3( std::shared_ptr<Value> value, const unsigned char* mask )
{
    REYES_ASSERT( value );
    
    reset( value->type(), value->storage(), value->size() );
               
    vec3* values = vec3_values();
    const vec3* other_values = value->vec3_values();
    const int size = value->size();

    if ( !mask )
    {
        for ( int i = 0; i < size; ++i )
        {
            values[i] = other_values[i];
        }
    }
    else
    {
        for ( int i = 0; i < size; ++i )
        {
            if ( mask[i] )
            {
                values[i] = other_values[i];
            }
        }
    }
}

void Value::assign_mat4x4( std::shared_ptr<Value> value, const unsigned char* mask )
{
    REYES_ASSERT( value );
    
    reset( value->type(), value->storage(), value->size() );
               
    mat4x4* values = mat4x4_values();
    const mat4x4* other_values = value->mat4x4_values();
    const int size = value->size();

    if ( !mask )
    {
        for ( int i = 0; i < size; ++i )
        {
            values[i] = other_values[i];
        }
    }
    else
    {
        for ( int i = 0; i < size; ++i )
        {
            if ( mask[i] )
            {
                values[i] = other_values[i];
            }
        }
    }
}

void Value::assign_string( std::shared_ptr<Value> value, const unsigned char* mask )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->storage() != STORAGE_VARYING );
    REYES_ASSERT( value->size() == 1 );

    reset( value->type(), value->storage(), value->size() );               
    string_value_ = value->string_value_;
}

void Value::inside_cone( std::shared_ptr<Value> direction, const math::vec3& axis, float angle )
{
    REYES_ASSERT( direction );
    REYES_ASSERT( direction->type() == TYPE_VECTOR );
    REYES_ASSERT( direction->storage() == STORAGE_VARYING );
    
    unsigned int size = direction->size();
    reset( TYPE_INTEGER, STORAGE_VARYING, size );
    size_ = size;
    
    int* inside = int_values();
    const vec3* direction_values = direction->vec3_values();
    const float cosine = cosf( angle );
    for ( unsigned int i = 0; i < size; ++i )
    {
        inside[i] = int(dot( math::normalize(direction_values[i]), axis ) >= cosine);
    }
}

void Value::dot_vec3( std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs )
{
    REYES_ASSERT( lhs );
    REYES_ASSERT( rhs );
    REYES_ASSERT( lhs->size() == rhs->size() );

    unsigned int size = lhs->size();
    reset( TYPE_FLOAT, max(lhs->storage(), rhs->storage()), size );

    const vec3* lhs_values = lhs->vec3_values();
    const vec3* rhs_values = rhs->vec3_values();    
    float* values = float_values();
    for ( unsigned int i = 0; i < size; ++i )
    {
        values[i] = math::dot( lhs_values[i], rhs_values[i] );
    }
}

void Value::allocate()
{
    REYES_ASSERT( !values_ );
    values_ = malloc( sizeof(vec3) * MAXIMUM_VERTICES_PER_GRID );
}
