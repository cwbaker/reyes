#ifndef SWEET_RENDER_SHADERPARSER_HPP_INCLUDED
#define SWEET_RENDER_SHADERPARSER_HPP_INCLUDED

#include "declspec.hpp"
#include <memory>

namespace sweet
{

namespace render
{

class SyntaxNode;
class SymbolTable;
class ErrorPolicy;

/**
// Parse shader source files into a syntax tree to be passed to the semantic
// analyzer and code generator.
*/
class SWEET_RENDER_DECLSPEC ShaderParser
{
    SymbolTable& symbol_table_; ///< The symbol table to use when parsing (preloaded with functions and variables).
    ErrorPolicy* error_policy_; ///< The error policy to report errors to.

public:
    ShaderParser( SymbolTable& symbol_table, ErrorPolicy* error_policy = 0 );
    std::shared_ptr<SyntaxNode> parse( const char* filename );
    std::shared_ptr<SyntaxNode> parse( const char* begin, const char* end );
};

}

}

#endif