#pragma once

#include "SetValueHelper.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <math/mat4x4.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace math
{

struct vec2;
struct vec3;
struct vec4;
struct mat4x4;

}

namespace reyes
{

class Value;
class Symbol;
class Light;
class Shader;

/**
// A grid of uniform and/or varying values that represents parameters
// to shaders, vertices in a diced grid of micropolygons, and lighting values
// returned by light shaders run against a diced grid of micropolygons.
*/
class Grid
{
    int width_; ///< The number of vertices across the u direction of this grid.
    int height_; ///< The number of vertices down the v direction of this grid.
    float du_; ///< Size of increments in u for this grid.
    float dv_; ///< Size of increments in v for this grid.
    std::vector<std::shared_ptr<Symbol>> symbols_; // The symbols stored in this grid.
    int memory_size_; ///< The number of bytes of allocated for persistent variables, parameters, and globals.
    unsigned char* memory_; ///< The base address of memory for persistent variables, parameters, and globals.
    mutable std::vector<std::string> strings_; ///< The strings in this Grid.
    std::vector<std::shared_ptr<Light>> lights_; ///< The lighting values for this grid.
    math::mat4x4 transform_; ///< The object to camera space transform at the time this Grid was bound to a Shader.
    Shader* shader_; ///< The light shader that this Grid stores parameters for or null if this Grid doesn't store parameters.
    bool normals_generated_; // True if normals have been generated from positions.

public:
    Grid();
    Grid( Shader* shader );
    Grid( const Grid& grid );
    Grid& operator=( const Grid& grid );
    ~Grid();
    
    int width() const;
    int height() const;
    int maximum_vertices() const;
    int size() const;
    float du() const;
    float dv() const;
    Shader* shader() const;
    const Symbol* find_symbol( const char* identifier ) const;
    unsigned char* memory() const;
    float* float_value( int address ) const;
    float* float_value( const Symbol* symbol ) const;
    float* float_value( const char* identifier ) const;
    int* int_value( int address ) const;
    int* int_value( const char* identifier ) const;
    int* int_value( const Symbol* symbol ) const;
    math::vec3* vec3_value( int address ) const;
    math::vec3* vec3_value( const char* identifier ) const;
    math::vec3* vec3_value( const Symbol* symbol ) const;
    math::mat4x4* mat4x4_value( int address ) const;
    math::mat4x4* mat4x4_value( const char* identifier ) const;
    math::mat4x4* mat4x4_value( const Symbol* symbol ) const;
    char* string_value( int index ) const;
    char* string_value( const char* identifier ) const;
    char* string_value( const Symbol* symbol ) const;

    void clear();
    void resize( int width, int height );
    void set_normals_generated( bool normals_generated );
    void generate_normals( bool left_handed, bool force = false );
    void set_string( int index, const std::string& value );
    SetValueHelper operator[]( const std::string& identifier );
    void set_shader( Shader* shader );
    void set_symbols( const std::vector<std::shared_ptr<Symbol>>& symbols );
    void zero();

    void clear_lights();
    void reserve_lights( unsigned int lights );
    void add_light( std::shared_ptr<Light> light );
    const Light* get_light( int index ) const;
    const std::vector<std::shared_ptr<Light> >& lights() const;
    
    void set_transform( const math::mat4x4& transform );
    const math::mat4x4& get_transform() const;

    void set_du( float du );
    void set_dv( float dv );

private:
    void reserve();
    void* lookup( int address ) const;
    float* lookup_float( int address ) const;
    int* lookup_int( int address ) const;
    math::vec3* lookup_vec3( int address ) const;
    math::vec4* lookup_vec4( int address ) const;
    math::mat4x4* lookup_mat4x4( int address ) const;
};

}
