//
// Grid.cpp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include <sweet/math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>
#include <vector>

using std::map;
using std::string;
using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

Grid::Grid()
: width_( 1 ),
  height_( 1 ),
  du_( 0.0f ),
  dv_( 0.0f ),
  values_(),
  values_by_identifier_(),
  lights_(),
  transform_( math::identity() ),
  shader_( NULL )
{
}

Grid::Grid( Shader* shader )
: width_( 1 ),
  height_( 1 ),
  du_( 0.0f ),
  dv_( 0.0f ),
  values_(),
  values_by_identifier_(),
  lights_(),
  transform_( math::identity() ),
  shader_( shader )
{
    SWEET_ASSERT( shader_ );
}

Grid::Grid( const Grid& grid )
: width_( grid.width_ ),
  height_( grid.height_ ),
  du_( grid.du_ ),
  dv_( grid.dv_ ),
  values_(),
  values_by_identifier_(),
  lights_(),
  transform_( grid.transform_ ),
  shader_( grid.shader_ )
{
    values_.reserve( grid.values_by_identifier_.size() );
    const map<string, ptr<Value> >& values_by_identifier = grid.values_by_identifier_;
    for ( map<string, ptr<Value> >::const_iterator i = values_by_identifier.begin(); i != values_by_identifier.end(); ++i )
    {
        copy_value( i->first, i->second );
    }
}

Grid::~Grid()
{
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

Shader* Grid::shader() const
{
    return shader_;
}

void Grid::clear()
{
    width_ = 1;
    height_ = 1;
    du_ = 0.0f;
    dv_ = 0.0f;
    lights_.clear();
    values_by_identifier_.clear();
    values_.clear();
}

void Grid::resize( int width, int height )
{
    SWEET_ASSERT( width > 0 );
    SWEET_ASSERT( height > 0 );
    
    width_ = width;
    height_ = height;
}

void Grid::generate_normals( bool left_handed, bool force )
{
    if ( force || !find_value("N") )
    {
        Value& normals = value( "N", TYPE_NORMAL );    
        const vec3* positions = value("P").vec3_values();
        SWEET_ASSERT( positions );
        
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
        
        normals.clear();
        normals.reserve( generated_normals.size() );
        vec3* values = normals.vec3_values();
        int j = 0;
        for ( vector<vec4>::const_iterator normal = generated_normals.begin(); normal != generated_normals.end(); ++normal )
        {
            values[j] = vec3(*normal) / normal->w;
            ++j;
        }  
    }
}

Value& Grid::value( const std::string& identifier, ValueType type )
{
    ptr<Value> value = find_value( identifier );
    if ( !value )
    {
        value = add_value( identifier, type );
    }
    return *value;
}

const Value& Grid::value( const std::string& identifier ) const
{
    ptr<Value> value = find_value( identifier );
    SWEET_ASSERT( value );
    return *value;
}

Value& Grid::operator[]( const std::string& identifier )
{
    return value( identifier, TYPE_NULL );
}

const Value& Grid::operator[]( const std::string& identifier ) const
{
    return value( identifier );
}

void Grid::copy_value( const std::string& identifier, ptr<Value> value )
{
    SWEET_ASSERT( !identifier.empty() );
    SWEET_ASSERT( !find_value(identifier) );
    SWEET_ASSERT( value );
    SWEET_ASSERT( value->size() <= width_ * height_ );

    ptr<Value> copied_value( new Value(*value) );
    values_.push_back( copied_value );
    values_by_identifier_.insert( make_pair(identifier, copied_value) );
}

void Grid::insert_value( const std::string& identifier, ptr<Value> value )
{
    SWEET_ASSERT( !identifier.empty() );
    SWEET_ASSERT( !find_value(identifier) );
    SWEET_ASSERT( value );
    SWEET_ASSERT( value->size() <= width_ * height_ );
    
    values_by_identifier_.insert( make_pair(identifier, value) );
}

ptr<Value> Grid::add_value( const std::string& identifier, ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( !identifier.empty() );
    SWEET_ASSERT( !find_value(identifier) );
    
    ptr<Value> value( new Value(type, storage, size()) );
    values_.push_back( value );
    values_by_identifier_.insert( make_pair(identifier, value) );
    return value;
}

ptr<Value> Grid::find_value( const std::string& identifier ) const
{
    map<string, ptr<Value> >::const_iterator i = values_by_identifier_.find( identifier );
    return i != values_by_identifier_.end() ? i->second : ptr<Value>();
}

const std::vector<ptr<Value> >& Grid::values() const
{
    return values_;
}

const std::map<std::string, ptr<Value> >& Grid::values_by_identifier() const
{
    return values_by_identifier_;
}

void Grid::reserve_lights( unsigned int lights )
{
    lights_.reserve( lights );
}

void Grid::add_light( ptr<Light> light )
{
    lights_.push_back( light );
}

const Light* Grid::get_light( int index ) const
{
    SWEET_ASSERT( index >= 0 && index < int(lights_.size()) );
    return lights_[index].get();
}

const std::vector<ptr<Light> >& Grid::lights() const
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
