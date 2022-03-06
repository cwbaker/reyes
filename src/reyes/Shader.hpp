#ifndef REYES_SHADER_HPP_INCLUDED
#define REYES_SHADER_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace reyes
{

class SyntaxNode;
class Symbol;
class Value;
class Grid;
class SymbolTable;
class Renderer;
class ErrorPolicy;

/**
// A displacement, surface, or light shader.
*/
class Shader
{
    std::vector<std::shared_ptr<Symbol>> symbols_; ///< The symbols that are used in the shader.
    std::vector<std::shared_ptr<Value>> values_; ///< The values of any constants used in the shader (including default parameter values).
    std::vector<unsigned char> constants_; ///< Shader constant data.
    std::vector<unsigned char> code_; ///< The byte code generated for the shader.
    int initialize_address_; ///< The index of the start of the initialize code fragment.
    int shade_address_; ///< The index of the start of the shade code fragment.
    int parameters_; ///< The number of parameters to the shader.
    int variables_; ///< The number of variables in the shader.
    int maximum_vertices_; ///< The maximum number of values in a varying variable.
    int constant_memory_size_;
    int grid_memory_size_;
    int temporary_memory_size_;

public:
    Shader();
    Shader( const char* filename, ErrorPolicy& error_policy );
    Shader( const char* filename, SymbolTable& symbol_table, ErrorPolicy& error_policy );
    Shader( const char* start, const char* finish, ErrorPolicy& error_policy );
    Shader( const char* start, const char* finish, SymbolTable& symbol_table, ErrorPolicy& error_policy );

    const Symbol* symbol( int index ) const;
    const std::vector<std::shared_ptr<Symbol>>& symbols() const;
    const std::vector<std::shared_ptr<Value>>& values() const;
    const unsigned char* constant( int offset ) const;
    const std::vector<unsigned char>& code() const;
    int initialize_address() const;
    int shade_address() const;
    int end_address() const;
    int parameters() const;
    int variables() const;
    int maximum_vertices() const;
    int constant_memory_size() const;
    int grid_memory_size() const;
    int temporary_memory_size() const;
    std::shared_ptr<Symbol> find_symbol( const std::string& identitifer ) const;

    void load_file( const char* filename, ErrorPolicy& error_policy );
    void load_file( const char* filename, SymbolTable& symbol_table, ErrorPolicy& error_policy );
    void load_memory( const char* start, const char* finish, ErrorPolicy& error_policy );
    void load_memory( const char* start, const char* finish, SymbolTable& symbol_table, ErrorPolicy& error_policy );
};

}

#endif
