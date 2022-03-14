#ifndef REYES_CODEGENERATOR_HPP_INCLUDED
#define REYES_CODEGENERATOR_HPP_INCLUDED

#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include "Address.hpp"
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <vector>
#include <string>
#include <memory>

namespace reyes
{

class Symbol;
class Value;
class SyntaxNode;
class SymbolTable;
class Encoder;
class ErrorPolicy;

/**
// Generate code from a syntax tree after parsing and semantic analysis.
*/
class CodeGenerator
{
    struct Jump
    {
        int address_; ///< The address immediately following the jump instruction and argument for this jump.
        int distance_address_; ///< The address of the distance argument for this jump.
        Jump( int address, int distance_address );
    };
    
    struct Loop
    {
        int begin_; ///< The address to jump to the beginning of the loop.
        int continue_; ///< The address to jump to the continue point of the loop.
        std::vector<Jump> jumps_to_begin_; ///< The jumps to the beginning of this loop.
        std::vector<Jump> jumps_to_continue_; ///< The jumps to continue this loop.
        std::vector<Jump> jumps_to_end_; ///< The jumps out of this loop.
        Loop( int begin );
    };

    ErrorPolicy* error_policy_; ///< ErrorPolicy to report errors detected during code generation to.
    int maximum_vertices_; ///< The maximum number of values in a varying variable.
    int initialize_address_; ///< The index in the code at which initialize code begins (always 0).
    int shade_address_; ///< The index in the code at which the shade code begins.
    int parameters_; ///< The number of parameters to the shader.
    int variables_; ///< The number of variables in the shader.
    int grid_memory_size_; ///< The amount of memory used by grid variables.
    int temporary_memory_size_; ///< The amount of memory used by temporary variables.
    int errors_; ///< The number of errors detected during code generation.
    std::vector<std::shared_ptr<Symbol>> symbols_; ///< The symbols that are used in the shader.
    std::vector<std::shared_ptr<Value>> values_; ///< The values of any constants used in the shader (including default parameter values).
    std::vector<Loop> loops_; ///< The Loops used to patch jumps to the beginning or the end of an enclosing loop.
    Encoder* encoder_; ///< Write byte code instructions and arguments.
    std::vector<unsigned char> constant_data_; ///< Constants.
    std::vector<int> temporary_addresses_; ///< The stack of addresses that are being used to store temporaries that are still in use.
    int offset_; ///< The offset of the next available temporary memory.

public:
    CodeGenerator( ErrorPolicy* error_policy = nullptr );
    ~CodeGenerator();
    void generate( SyntaxNode* node, const char* name );

    std::shared_ptr<Symbol> find_symbol( const std::string& identifier ) const;
    std::shared_ptr<Symbol> get_symbol( int index ) const;

    int initialize_address() const;
    int shade_address() const;
    int parameters() const;
    int variables() const;
    int maximum_vertices() const;
    int constant_memory_size() const;
    int grid_memory_size() const;
    int temporary_memory_size() const;
    std::vector<std::shared_ptr<Symbol>>& symbols();
    const std::vector<std::shared_ptr<Symbol>>& symbols() const;
    std::vector<std::shared_ptr<Value>>& values();
    const std::vector<std::shared_ptr<Value>>& values() const;
    std::vector<unsigned char>& constant_data();
    const std::vector<unsigned char>& constant_data() const;
    const std::vector<unsigned char>& code() const;
    
private:
    void error( bool condition, int line, const char* format, ... );
    void generate_code_in_case_of_errors();
    void generate_constants( SyntaxNode* node );
    void generate_symbols( const SyntaxNode* node );
    void evaluate_constant_expression( SyntaxNode* node );
    int arithmetic_instruction_from_type( int instruction, ValueType type ) const;
    int promote_instruction_from_type( int instruction, ValueType type ) const;
    Address generate_binary_expression( const SyntaxNode* node );
    Address generate_assign( int instruction, const SyntaxNode& node );
    Address generate_convert( Address address, const SyntaxNode& node );
    Address generate_promote( Address address, const SyntaxNode& node );
    
    void generate_code_for_list( const SyntaxNode& node );
    void generate_code_for_variable( const SyntaxNode& node );
    void generate_code_for_parameters( const SyntaxNode& node );

    void generate_statement( const SyntaxNode& node );
    void generate_if_statement( const SyntaxNode& node );
    void generate_if_else_statement( const SyntaxNode& node );
    void generate_while_statement( const SyntaxNode& node );
    void generate_for_statement( const SyntaxNode& node );
    void generate_ambient_statement( const SyntaxNode& node );
    void generate_solar_statement( const SyntaxNode& node );
    void generate_illuminate_statement( const SyntaxNode& node );
    void generate_illuminance_statement( const SyntaxNode& node );
    void generate_break_statement( const SyntaxNode& node );
    void generate_continue_statement( const SyntaxNode& node );
    void generate_return_statement( const SyntaxNode& node );

    Address generate_expression( const SyntaxNode& node );
    Address generate_call_expression( const SyntaxNode& node );
    Address generate_divide_expression( const SyntaxNode& node );
    Address generate_negate_expression( const SyntaxNode& node );
    Address generate_ternary_expression( const SyntaxNode& node );
    Address generate_typecast_expression( const SyntaxNode& node );
    Address generate_vec3_typecast_expression( const SyntaxNode& node );
    Address generate_mat4x4_typecast_expression( const SyntaxNode& node );
    Address generate_texture_typecast_expression( const SyntaxNode& node );
    Address generate_environment_typecast_expression( const SyntaxNode& node );
    Address generate_shadow_expression( const SyntaxNode& node );
    Address generate_float_texture_expression( const SyntaxNode& node );
    Address generate_vec3_texture_expression( const SyntaxNode& node );
    Address generate_float_environment_expression( const SyntaxNode& node );
    Address generate_vec3_environment_expression( const SyntaxNode& node );
    Address generate_constant_expression( const SyntaxNode& node );
    Address generate_identifier_expression( const SyntaxNode& node );
    
    void instruction( int instruction );
    void instruction( int instruction, ValueType type, ValueStorage storage );
    void instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1 );
    void instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1, ValueType type2, ValueStorage storage2 );
    void argument( int argument );
    void argument( Address address );
    void patch_argument( int address, int distance );
    int argument_for_patching();
    int address();
    
    void push_loop();
    void pop_loop();
    void mark_loop_continue();
    void jump_to_begin( int jump_instruction, int level );
    void jump_to_continue( int jump_instruction, int level );
    void jump_to_end( int jump_instruction, int level );
    
    Address write_constant( const void* value, int size );
    Address write_constant_float( float value );
    Address write_constant_vec3( const math::vec3& value );
    Address write_constant_mat4x4( const math::mat4x4& value );
    Address write_constant_string( const std::string& value );

    Address allocate_address( ValueType type, ValueStorage storage );
    void push_address();
    void pop_address();
    int size_by_type_and_storage( ValueType type, ValueStorage storage ) const;
};

}

#endif
