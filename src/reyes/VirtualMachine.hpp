#pragma once

#include "Address.hpp"
#include <reyes/reyes_virtual_machine/ConditionMask.hpp>
#include <math/vec4.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <vector>
#include <map>
#include <memory>

namespace reyes
{

class Grid;
class Shader;
class SymbolTable;
class Renderer;

/**
// A virtual machine that interprets the code generated for shaders to execute
// those shaders.
*/
class VirtualMachine
{
    const Renderer* renderer_; ///< The Renderer that this virtual machine is part of.
    SymbolTable* symbol_table_; ///< Symbol table used for resolving called functions.
    Grid* grid_; ///< The grid of micropolygon vertices that is currently being shaded (null if no shader is being executed).
    Shader* shader_; ///< The shader that is currently being executed (null if no shader is being executed).
    int light_index_; ///< The index of the current light (or INT_MAX if there is no current light).
    int length_; ///< The number of values in a varying variable.
    int constant_memory_size_; ///< The number of bytes of constant memory.
    const unsigned char* constant_memory_; // Constant memory.
    int grid_memory_size_; // Number of bytes of grid memory.
    unsigned char* grid_memory_; // Grid memory.
    int temporary_memory_size_; // Number of bytes of temporary memory.
    unsigned char* temporary_memory_; // Temporary memory.
    const unsigned char* code_begin_; ///< The address of the beginning of loaded code.
    const unsigned char* code_end_; ///< The address one past the end of loaded code.
    const unsigned char* code_; ///< The currently executed instruction.
    std::vector<ConditionMask> masks_; ///< The stack of condition masks that specify which elements to use during assignment.
    
public:
    VirtualMachine();
    VirtualMachine( const Renderer& renderer );
    ~VirtualMachine();
    void initialize( Grid& grid, Shader& shader );
    void shade( Grid& grid, Shader& shader );
    
private:
    void construct( int start, int finish );
    void execute();
    void jump_illuminance( int distance );
    void jump( int distance );
    int instruction();
    int dispatch();
    int argument();
    int byte();
    int word();
    int quad();
    
    void execute_halt();
    void execute_clear_mask();
    void execute_generate_mask();
    void execute_invert_mask();
    void execute_jump_empty();
    void execute_jump_not_empty();
    void execute_jump_illuminance();
    void execute_jump();
    void execute_transform_point();
    void execute_transform_vector();
    void execute_transform_normal();
    void execute_transform_color();
    void execute_transform_matrix();
    void execute_dot();
    void execute_multiply();
    void execute_divide();
    void execute_add();
    void execute_subtract();
    void execute_greater();
    void execute_greater_equal();
    void execute_less();
    void execute_less_equal();
    void execute_and();
    void execute_or();
    void execute_equal();
    void execute_not_equal();
    void execute_negate();
    void execute_convert();
    void execute_promote();
    void execute_assign();
    void execute_assign_string();
    void execute_add_assign();
    void execute_subtract_assign();
    void execute_multiply_assign();
    void execute_divide_assign();
    void execute_float_texture();
    void execute_vec3_texture();
    void execute_float_environment();
    void execute_vec3_environment();
    void execute_shadow();
    void execute_call();
    void execute_ambient();
    void execute_solar();
    void execute_solar_axis_angle();
    void execute_illuminate();
    void execute_illuminate_axis_angle();
    void execute_illuminance_axis_angle();

    void float_texture( const Renderer& renderer, float* result, const char* texturename, const float* s, const float* t, int length ) const;
    void vec3_texture( const Renderer& renderer, math::vec3* result, const char* texturename, const float* s, const float* t, int length ) const;
    void float_environment( const Renderer& renderer, float* result, const char* texturename, const math::vec3* direction, int length ) const;
    void vec3_environment( const Renderer& renderer, math::vec3* result, const char* texturename, const math::vec3* direction, int length ) const;
    void shadow( const Renderer& renderer, float* result, const char* texturename, const math::vec3* position, const float* bias, int length ) const;
    
    void push_mask( const float* values, int length );
    void pop_mask();
    void invert_mask();
    bool mask_empty() const;
    const unsigned char* mask( unsigned int dispatch ) const;

    void* lookup( Address address );
    const char* lookup_string( Address address );    
    float* lookup_float( Address address );
    int* lookup_int( Address address );
    math::vec3* lookup_vec3( Address address );
    math::vec4* lookup_vec4( Address address );
    math::mat4x4* lookup_mat4x4( Address address );

    VirtualMachine( VirtualMachine&& ) = delete;
    VirtualMachine( const VirtualMachine& ) = delete;
    VirtualMachine& operator=( VirtualMachine&& ) = delete;
    VirtualMachine& operator=( const VirtualMachine& ) = delete;
};

}
