#ifndef REYES_VIRTUALMACHINE_HPP_INCLUDED
#define REYES_VIRTUALMACHINE_HPP_INCLUDED

#include <math/vec4.hpp>
#include <math/vec3.hpp>
#include <vector>
#include <map>
#include <memory>

namespace reyes
{

class Grid;
class Value;
class Shader;
class Renderer;

/**
// A virtual machine that interprets the code generated for shaders to execute
// those shaders.
*/
class VirtualMachine
{
    struct ConditionMask
    {
        std::vector<unsigned char> mask_; ///< The mask that specifies whether or not an element is to be processed.
        int processed_; ///< The number of elements that are to be processed by this mask.

        ConditionMask();
        void generate( std::shared_ptr<Value> value );
        void generate( const ConditionMask& condition_mask, std::shared_ptr<Value> value );
        void invert();
        bool empty() const;
    };

    const Renderer* renderer_; ///< The Renderer that this virtual machine is part of.
    Grid* grid_; ///< The grid of micropolygon vertices that is currently being shaded (null if no shader is being executed).
    Shader* shader_; ///< The shader that is currently being executed (null if no shader is being executed).
    std::vector<std::shared_ptr<Value> > values_; ///< The values allocated for use as temporary registers by this virtual machine.
    std::vector<std::shared_ptr<Value> > registers_; ///< The values loaded into registers by this virtual machine (some from grid, some temporary).
    int register_index_; ///< The index of the next available register.
    int light_index_; ///< The index of the current light (or INT_MAX if there is no current light).
    const unsigned char* code_begin_; ///< The address of the beginning of loaded code.
    const unsigned char* code_end_; ///< The address one past the end of loaded code.
    std::vector<ConditionMask> masks_; ///< The stack of condition masks that specify which elements to use during assignment.
    const unsigned char* code_; ///< The currently executed instruction.
    
public:
    VirtualMachine();
    VirtualMachine( const Renderer& renderer );
    void initialize( Grid& parameters, Shader& shader );
    void shade( Grid& globals, Grid& parameters, Shader& shader );
    
private:
    void construct( int start, int finish );
    void initialize_registers( Grid& grid );
    void execute();
    void jump_illuminance( int distance );
    void jump( int distance );
    int instruction();
    int argument();
    int word();
    int quad();
    
    void execute_halt();
    void execute_reset();
    void execute_clear_mask();
    void execute_generate_mask();
    void execute_invert_mask();
    void execute_jump_empty();
    void execute_jump_not_empty();
    void execute_jump_illuminance();
    void execute_jump();
    void execute_transform();
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
    void execute_promote_integer();
    void execute_promote_float();
    void execute_promote_vec3();
    void execute_float_to_color();
    void execute_float_to_point();
    void execute_float_to_vector();
    void execute_float_to_normal();
    void execute_float_to_matrix();
    void execute_assign();
    void execute_assign_mat4x4();
    void execute_assign_string();
    void execute_add_assign();
    void execute_multiply_assign();
    void execute_float_texture();
    void execute_vec3_texture();
    void execute_float_environment();
    void execute_vec3_environment();
    void execute_shadow();
    void execute_call_0();
    void execute_call_1();
    void execute_call_2();
    void execute_call_3();
    void execute_call_4();
    void execute_call_5();
    void execute_ambient();
    void execute_solar();
    void execute_solar_axis_angle();
    void execute_illuminate();
    void execute_illuminate_axis_angle();
    void execute_illuminance_axis_angle();

    void float_texture( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> s, std::shared_ptr<Value> t ) const;
    void vec3_texture( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> s, std::shared_ptr<Value> t ) const;
    void float_environment( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> direction ) const;
    void vec3_environment( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> direction ) const;
    void shadow( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> position, std::shared_ptr<Value> bias ) const;
    
    void push_mask( std::shared_ptr<Value> value );
    void pop_mask();
    void invert_mask();
    bool mask_empty() const;
    const unsigned char* get_mask() const;
        
    void reset_register( int index );
    int allocate_register();
};

}

#endif
