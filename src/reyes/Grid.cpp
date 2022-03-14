//
// Grid.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Grid.hpp"
#include "Shader.hpp"
#include "Symbol.hpp"
#include "assert.hpp"
#include <math/mat4x4.ipp>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>

using std::max;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using std::back_inserter;
using namespace math;
using namespace reyes;

Grid::Grid()
: width_( 1 )
, height_( 1 )
, du_( 0.0f )
, dv_( 0.0f )
, symbols_()
, memory_size_( 0 )
, memory_( nullptr )
, lights_()
, transform_( math::identity() )
, shader_( nullptr )
, normals_generated_( false )
{
}

Grid::Grid( Shader* shader )
: width_( 1 )
, height_( 1 )
, du_( 0.0f )
, dv_( 0.0f )
, symbols_()
, memory_size_( 0 )
, memory_( nullptr )
, lights_()
, transform_( math::identity() )
, shader_( nullptr )
, normals_generated_( false )
{
    set_shader( shader );
}

Grid::Grid( const Grid& grid )
: width_( grid.width_ )
, height_( grid.height_ )
, du_( grid.du_ )
, dv_( grid.dv_ )
, symbols_( grid.symbols_ )
, memory_size_( 0 )
, memory_( nullptr )
, lights_()
, transform_( grid.transform_ )
, shader_( grid.shader_ )
, normals_generated_( grid.normals_generated_ )
{
    reserve();
    memcpy( memory_, grid.memory_, memory_size_ );
}

Grid& Grid::operator=( const Grid& grid )
{
    if ( &grid != this )
    {
        width_ = grid.width_;
        height_ = grid.height_;
        du_ = grid.du_;
        dv_ = grid.dv_;
        symbols_ = grid.symbols_;
        transform_ = grid.transform_;
        shader_ = grid.shader_;
        normals_generated_ = grid.normals_generated_;
        lights_.clear();        
        reserve();
        memcpy( memory_, grid.memory_, memory_size_ );
    }
    return *this;
}

Grid::~Grid()
{
    if ( memory_ )
    {
        free( memory_ );
        memory_ = nullptr;
    }
}

int Grid::width() const
{
    return width_;
}

int Grid::height() const
{
    return height_;
}

int Grid::size() const
{
    return width_ * height_;
}

int Grid::maximum_vertices() const
{
    REYES_ASSERT( shader_ );
    return shader_ ? shader_->maximum_vertices() : 0;
}

float Grid::du() const
{
    return du_;
}

float Grid::dv() const
{
    return dv_;
}

Shader* Grid::shader() const
{
    return shader_;
}

const Symbol* Grid::find_symbol( const char* identifier ) const
{
    vector<shared_ptr<Symbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    return i != symbols_.end() ? i->get() : nullptr;
}

unsigned char* Grid::memory() const
{
    return memory_;
}

float* Grid::float_value( int address ) const
{
    return lookup_float( address );
}

float* Grid::float_value( const Symbol* symbol ) const
{
    if ( symbol && symbol->type() == TYPE_FLOAT )
    {
        return float_value( symbol->offset() );        
    }
    return nullptr;
}

float* Grid::float_value( const char* identifier ) const
{
    REYES_ASSERT( identifier );
    return float_value( find_symbol(identifier) );
}

int* Grid::int_value( int address ) const
{
    return lookup_int( address );
}

int* Grid::int_value( const Symbol* symbol ) const
{
    if ( symbol && symbol->type() == TYPE_INTEGER )
    {
        return int_value( symbol->offset() );
    }
    return nullptr;
}

int* Grid::int_value( const char* identifier ) const
{
    REYES_ASSERT( identifier );
    return int_value( find_symbol(identifier) );
}

math::vec3* Grid::vec3_value( int address ) const
{
    return lookup_vec3( address );
}

math::vec3* Grid::vec3_value( const Symbol* symbol ) const
{
    if ( symbol )
    {
        ValueType type = symbol->type();
        if ( type == TYPE_COLOR || type == TYPE_POINT || type == TYPE_VECTOR || type == TYPE_NORMAL )
        {
            return vec3_value( symbol->offset() );
        }
    }
    return nullptr;
}

math::vec3* Grid::vec3_value( const char* identifier ) const
{
    return vec3_value( find_symbol(identifier) );
}

math::mat4x4* Grid::mat4x4_value( int address ) const
{
    return lookup_mat4x4( address );
}

math::mat4x4* Grid::mat4x4_value( const Symbol* symbol ) const
{
    if ( symbol && symbol->type() == TYPE_MATRIX )
    {
        return mat4x4_value( symbol->offset() );
    }
    return nullptr;
}

math::mat4x4* Grid::mat4x4_value( const char* identifier ) const
{
    return mat4x4_value( find_symbol(identifier) );
}

char* Grid::string_value( int index ) const
{
    if ( index >= int(strings_.size()) )
    {
        strings_.insert( strings_.end(), index - strings_.size() + 1, string() );
    }
    assert( index >= 0 && index < int(strings_.size()) );
    return strings_[index].data();
}

char* Grid::string_value( const char* identifier ) const
{
    return string_value( find_symbol(identifier) );
}

char* Grid::string_value( const Symbol* symbol ) const
{
    if ( symbol && symbol->type() == TYPE_STRING )
    {
        assert( symbol->segment() == SEGMENT_STRING );
        return string_value( symbol->offset() );
    }
    return nullptr;
}

void Grid::clear()
{
    width_ = 1;
    height_ = 1;
    du_ = 0.0f;
    dv_ = 0.0f;
    symbols_.clear();
    strings_.clear();
    lights_.clear();
}

void Grid::resize( int width, int height )
{
    REYES_ASSERT( width > 0 );
    REYES_ASSERT( height > 0 );
    width_ = width;
    height_ = height;
    normals_generated_ = false;
}

void Grid::set_normals_generated( bool normals_generated )
{
    normals_generated_ = normals_generated;
}

void Grid::generate_normals( bool left_handed, bool force )
{
    if ( force || !normals_generated_ )
    {
        const vec3* positions = vec3_value( "P" );
        REYES_ASSERT( positions );
        
        vector<vec4> generated_normals;
        generated_normals.insert( generated_normals.end(), width_ * height_, vec4(0.0f, 0.0f, 0.0f, 0.0f) );

        int i = 0;
        for ( int y = 0; y < height_ - 1; ++y )
        {
            for ( int x = 0; x < width_ - 1; ++x )
            {
                int i0 = i + x;
                int i1 = i + width_ + x;
                int i2 = i + width_ + x + 1;
                int i3 = i + x + 1;

                vec3 u0 = positions[i3] - positions[i0];
                vec3 u1 = positions[i2] - positions[i1];
                vec3 u = length(u0) > length(u1) ? u0 : u1;
                vec3 v0 = positions[i1] - positions[i0];
                vec3 v1 = positions[i2] - positions[i3];
                vec3 v = length(v0) > length(v1) ? v0 : v1;     
                vec3 normal = normalize( left_handed ? cross(u, v) : cross(v, u) );

                generated_normals[i0] += vec4( normal, 1.0f );
                generated_normals[i1] += vec4( normal, 1.0f );
                generated_normals[i2] += vec4( normal, 1.0f );
                generated_normals[i3] += vec4( normal, 1.0f );
            }
            i += width_;
        }
        
        vec3* normals = vec3_value( "N" );
        REYES_ASSERT( normals );
        for ( size_t j = 0; j < generated_normals.size(); ++j )
        {
            const vec4& generated_normal = generated_normals[j];
            normals[j] = vec3(generated_normal) / generated_normal.w;
        }

        normals_generated_ = true;
    }
}

void Grid::set_string( int index, const std::string& value )
{
    if ( index >= int(strings_.size()) )
    {
        strings_.insert( strings_.end(), index - int(strings_.size()) + 1, string() );
    }
    assert( index >= 0 && index < int(strings_.size()) );
    strings_[index] = value;
}

SetValueHelper Grid::operator[]( const std::string& identifier )
{
    const Symbol* symbol = find_symbol( identifier.c_str() );
    if ( symbol )
    {
        return SetValueHelper( this, symbol->offset() );
    }
    return SetValueHelper();
}

void Grid::set_shader( Shader* shader )
{
    REYES_ASSERT( shader );
    shader_ = shader;
    set_symbols( shader->symbols() );
}

void Grid::set_symbols( const std::vector<std::shared_ptr<Symbol>>& symbols )
{
    auto include_symbol = [](const shared_ptr<Symbol>& symbol)
    {
        return 
            symbol->segment() == SEGMENT_GRID ||
            symbol->segment() == SEGMENT_STRING
        ;
    };
    symbols_.clear();
    copy_if( symbols.begin(), symbols.end(), back_inserter(symbols_), include_symbol );
    if ( !symbols_.empty() )
    {
        reserve();
    }
}

void Grid::zero()
{
    vec3* positions = vec3_value( "P" );
    if ( memory_ && positions )
    {
        memset( positions, 0, sizeof(vec3) * size() );
    }
}

void Grid::clear_lights()
{
    lights_.clear();
}

void Grid::reserve_lights( unsigned int lights )
{
    lights_.reserve( lights );
}

void Grid::add_light( std::shared_ptr<Light> light )
{
    lights_.push_back( light );
}

const Light* Grid::get_light( int index ) const
{
    REYES_ASSERT( index >= 0 && index < int(lights_.size()) );
    return lights_[index].get();
}

const std::vector<std::shared_ptr<Light> >& Grid::lights() const
{
    return lights_;
}

void Grid::set_transform( const math::mat4x4& transform )
{
    transform_ = transform;
}

const math::mat4x4& Grid::get_transform() const
{
    return transform_;
}

void Grid::set_du( float du )
{
    du_ = du;
}

void Grid::set_dv( float dv )
{
    dv_ = dv;
}

void Grid::reserve()
{
    int memory_size = 0;
    for ( const shared_ptr<Symbol>& symbol : symbols_ )
    {
        if ( symbol->segment() == SEGMENT_GRID )
        {
            // Maximum values in a varying variable.
            const int MAXIMUM_LENGTH = 512;
            memory_size = max( memory_size, symbol->offset() + symbol->size_by_type_and_storage(MAXIMUM_LENGTH) );
        }
    }

    if ( memory_size_ < memory_size )
    {
        free( memory_ );
        memory_ = nullptr;
        memory_size_ = 0;
    }

    if ( !memory_ )
    {
        memory_ = reinterpret_cast<unsigned char*>( malloc(memory_size) );
        memory_size_ = memory_size;
    }
}

void* Grid::lookup( int offset ) const
{
    REYES_ASSERT( !symbols_.empty() && symbols_.front() );
    offset -= symbols_.front()->offset();
    REYES_ASSERT( offset >= 0 && offset < memory_size_ );
    return memory_ + offset;
}

float* Grid::lookup_float( int offset ) const
{
    return reinterpret_cast<float*>( lookup(offset) );
}

int* Grid::lookup_int( int offset ) const
{
    return reinterpret_cast<int*>( lookup(offset) );
}

math::vec3* Grid::lookup_vec3( int offset ) const
{
    return reinterpret_cast<vec3*>( lookup(offset) );
}

math::vec4* Grid::lookup_vec4( int offset ) const
{
    return reinterpret_cast<vec4*>( lookup(offset) );
}

math::mat4x4* Grid::lookup_mat4x4( int offset ) const
{
    return reinterpret_cast<mat4x4*>( lookup(offset) );
}
