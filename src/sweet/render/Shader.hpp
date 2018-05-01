#ifndef SWEET_RENDER_SHADER_HPP_INCLUDED
#define SWEET_RENDER_SHADER_HPP_INCLUDED

#include "declspec.hpp"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace sweet
{

namespace render
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
class SWEET_RENDER_DECLSPEC Shader
{
    std::vector<std::shared_ptr<Symbol>> symbols_; ///< The symbols that are used in the shader.
    std::vector<std::shared_ptr<Value>> values_; ///< The values of any constants used in the shader (including default parameter values).
    std::vector<short> code_; ///< The byte code generated for the shader.
    int initialize_address_; ///< The index of the start of the initialize code fragment.
    int shade_address_; ///< The index of the start of the shade code fragment.
    int parameters_; ///< The number of parameters to the shader.
    int variables_; ///< The number of variables in the shader.
    int constants_; ///< The number of constants in the shader.
    int permanent_registers_; ///< The number of registers used by constant and uniform values in this shader.
    int registers_; ///< The maximum number of registers that are used by this shader (variables and temporaries).

public:
    Shader();
    Shader( const char* filename, SymbolTable& symbol_table, ErrorPolicy& error_policy );
    Shader( const char* start, const char* finish, SymbolTable& symbol_table, ErrorPolicy& error_policy );
    
    const std::vector<std::shared_ptr<Symbol> >& symbols() const;
    const std::vector<std::shared_ptr<Value> >& values() const;
    const std::vector<short>& code() const;
    int initialize_address() const;
    int shade_address() const;
    int end_address() const;
    int parameters() const;
    int variables() const;
    int constants() const;
    int permanent_registers() const;
    int registers() const;

    std::shared_ptr<Symbol> find_symbol( const std::string& identitifer ) const;
};

}

}

#endif
