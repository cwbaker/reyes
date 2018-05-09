//
// CodeGenerator.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_CODEGENERATOR_HPP_INCLUDED
#define SWEET_RENDER_CODEGENERATOR_HPP_INCLUDED

#include "declspec.hpp"
#include "SyntaxNodeType.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <vector>
#include <string>
#include <memory>

namespace sweet
{

namespace render
{

class Symbol;
class Value;
class SyntaxNode;
class SymbolTable;
class ErrorPolicy;

/**
// Generate code from a syntax tree after parsing and semantic analysis.
*/
class SWEET_RENDER_DECLSPEC CodeGenerator
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

    const SymbolTable& symbol_table_; ///< The SymbolTable to lookup symbols in.
    ErrorPolicy* error_policy_; ///< ErrorPolicy to report errors detected during code generation to.
    int initialize_address_; ///< The index in the code at which initialize code begins (always 0).
    int shade_address_; ///< The index in the code at which the shade code begins.
    int permanent_registers_; ///< The number of registers used to store constant and uniform values in the shader.
    int parameters_; ///< The number of parameters to the shader.
    int variables_; ///< The number of variables in the shader.
    int constants_; ///< The number of constants in the shader.
    int errors_; ///< The number of errors detected during code generation.
    std::vector<std::shared_ptr<Symbol> > symbols_; ///< The symbols that are used in the shader.
    std::vector<std::shared_ptr<Value> > values_; ///< The values of any constants used in the shader (including default parameter values).
    std::vector<short> code_; ///< The byte code generated for the shader.
    std::vector<int> temporary_registers_; ///< The stack of register indices that are being used to store values that are still being used.
    std::vector<Loop> loops_; ///< The Loops used to patch jumps to the beginning or the end of an enclosing loop.
    int index_; ///< The index of the next available register.  
    int registers_; ///< The number of registers that are used by the most recently generated code (variables and temporaries).

public:
    CodeGenerator( const SymbolTable& symbol_table, ErrorPolicy* error_policy = NULL );
    void generate( SyntaxNode* node, const char* name );

    std::shared_ptr<Symbol> find_symbol( const std::string& identifier ) const;
    std::shared_ptr<Symbol> get_symbol( int index ) const;

    int initialize_address() const;
    int shade_address() const;
    int permanent_registers() const;
    int parameters() const;
    int variables() const;
    int constants() const;
    std::vector<std::shared_ptr<Symbol> >& symbols();
    const std::vector<std::shared_ptr<Symbol> >& symbols() const;
    std::vector<std::shared_ptr<Value> >& values();
    const std::vector<std::shared_ptr<Value> >& values() const;
    std::vector<short>& code();
    const std::vector<short>& code() const;
    int registers() const;
    
private:
    void error( bool condition, int line, const char* format, ... );
    void generate_code_in_case_of_errors();
    void generate_symbols( const SyntaxNode& node );
    void generate_symbols_for_parameters( SyntaxNode* node );
    std::shared_ptr<Value> generate_constants( SyntaxNode* node );
    void generate_indexes_for_symbols();
    void evaluate_expression( std::shared_ptr<Value> value, const SyntaxNode* node ) const;
    int assign_instruction_from_type( int instruction, ValueType type ) const;
    int arithmetic_instruction_from_type( int instruction, ValueType type ) const;
    int promote_instruction_from_type( int instruction, ValueType type ) const;
    int generate_binary_expression( const SyntaxNode* node );
    int generate_code_for_assign_expression( int instruction, const SyntaxNode& node );
    int generate_type_conversion( int register_index, const SyntaxNode* node );
    int generate_storage_promotion( int register_index, const SyntaxNode* node );
    
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

    int generate_expression( const SyntaxNode& node );
    int generate_call_expression( const SyntaxNode& node );
    int generate_divide_expression( const SyntaxNode& node );
    int generate_negate_expression( const SyntaxNode& node );
    int generate_ternary_expression( const SyntaxNode& node );
    int generate_typecast_expression( const SyntaxNode& node );
    int generate_vec3_typecast_expression( const SyntaxNode& node );
    int generate_mat4x4_typecast_expression( const SyntaxNode& node );
    int generate_texture_typecast_expression( const SyntaxNode& node );
    int generate_environment_typecast_expression( const SyntaxNode& node );
    int generate_shadow_expression( const SyntaxNode& node );
    int generate_float_texture_expression( const SyntaxNode& node );
    int generate_vec3_texture_expression( const SyntaxNode& node );
    int generate_float_environment_expression( const SyntaxNode& node );
    int generate_vec3_environment_expression( const SyntaxNode& node );
    int generate_constant_expression( const SyntaxNode& node );
    int generate_identifier_expression( const SyntaxNode& node );
    
    void instruction( int instruction );
    void argument( int argument );
    void patch_argument( int address, int distance );
    int argument_for_patching();
    int address();
    
    void push_loop();
    void pop_loop();
    void mark_loop_continue();
    void jump_to_begin( int jump_instruction, int level );
    void jump_to_continue( int jump_instruction, int level );
    void jump_to_end( int jump_instruction, int level );
    
    int allocate_register();
    void push_register();
    void pop_register();
};

}

}

#endif