#ifndef REYES_SYMBOLTABLE_HPP_INCLUDED
#define REYES_SYMBOLTABLE_HPP_INCLUDED

#include "AddSymbolHelper.hpp"
#include <list>
#include <map>
#include <string>
#include <memory>

namespace reyes
{

class Scope;
class Symbol;
class SyntaxNode;

/**
// Hierarchical symbol table used to define the initial functions recognized
// by the shader parser as well as the parameters and variables that are
// defined during parsing.
*/
class SymbolTable
{
    std::vector<std::unique_ptr<Scope>> scopes_;
    std::vector<Scope*> scopes_stack_;

public:
    SymbolTable();
    ~SymbolTable();
    std::shared_ptr<Symbol> find_symbol( const std::string& identifier ) const;
    std::shared_ptr<Symbol> find_symbol( const SyntaxNode* node ) const;
    Scope* global_scope() const;
    Scope* push_scope();
    Scope* pop_scope();
    std::shared_ptr<Symbol> add_symbol( const std::string& identifier );
    AddSymbolHelper add_symbols( Segment segment = SEGMENT_GRID );
};

}

#endif
