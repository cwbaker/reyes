//
// Grid.hpp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_GRID_HPP_INCLUDED
#define SWEET_RENDER_GRID_HPP_INCLUDED

#include "declspec.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <sweet/math/mat4x4.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace sweet
{

namespace math
{

struct vec2;
struct vec3;
struct vec4;

}

namespace render
{

class Value;
class Light;
class Shader;

/**
// A grid of uniform and/or varying values that represents parameters
// to shaders, vertices in a diced grid of micropolygons, and lighting values
// returned by light shaders run against a diced grid of micropolygons.
*/
class SWEET_RENDER_DECLSPEC Grid
{
public:
    int width_; ///< The number of vertices across the u direction of this grid.
    int height_; ///< The number of vertices down the v direction of this grid.
    float du_; ///< Size of increments in u for this grid.
    float dv_; ///< Size of increments in v for this grid.
    std::vector<std::shared_ptr<Value> > values_; // The values stored in this grid.
    std::map<std::string, std::shared_ptr<Value> > values_by_identifier_; ///< The values stored in this grid by their identifier.
    std::vector<std::shared_ptr<Light> > lights_; ///< The lighting values for this grid.
    math::mat4x4 transform_; ///< The object to camera space transform at the time this Grid was bound to a Shader.
    Shader* shader_; ///< The light shader that this Grid stores parameters for or null if this Grid doesn't store parameters.
    
    public:
        Grid();
        Grid( Shader* shader );
        Grid( const Grid& grid );
        ~Grid();
        
        int width() const;
        int height() const;
        int size() const;
        Shader* shader() const;

        void clear();
        void resize( int width, int height );
        void generate_normals( bool left_handed, bool force = false );

        Value& value( const std::string& identifier, ValueType type );
        const Value& value( const std::string& identifier ) const;
        Value& operator[]( const std::string& identifier );
        const Value& operator[]( const std::string& identifier ) const;
        void copy_value( const std::string& identifier, std::shared_ptr<Value> value );
        void insert_value( const std::string& identifier, std::shared_ptr<Value> value );
        std::shared_ptr<Value> add_value( const std::string& identifier, ValueType type, ValueStorage storage = STORAGE_VARYING );
        std::shared_ptr<Value> find_value( const std::string& identifier ) const;
        const std::vector<std::shared_ptr<Value> >& values() const;
        const std::map<std::string, std::shared_ptr<Value> >& values_by_identifier() const;
        
        void reserve_lights( unsigned int lights );
        void add_light( std::shared_ptr<Light> light );
        const Light* get_light( int index ) const;
        const std::vector<std::shared_ptr<Light> >& lights() const;
        
        void set_transform( const math::mat4x4& transform );
        const math::mat4x4& get_transform() const;
};

}

}

#endif