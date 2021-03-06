#ifndef REYES_SYMBOLTABLE_HPP_INCLUDED
#define REYES_SYMBOLTABLE_HPP_INCLUDED

#include "AddSymbolHelper.hpp"
#include <list>
#include <map>
#include <string>
#include <memory>

namespace reyes
{

class Symbol;
class SyntaxNode;

/**
// Hierarchical symbol table used to define the initial functions recognized
// by the shader parser as well as the parameters and variables that are
// defined during parsing.
*/
class SymbolTable
{
    std::list<std::multimap<std::string, std::shared_ptr<Symbol>>> symbols_; ///< The stack of symbols used to represent the symbol hierarchy.

public:
    SymbolTable();
    AddSymbolHelper add_symbols();
    void push_scope();
    void pop_scope();    
    std::shared_ptr<Symbol> add_symbol( const std::string& identifier );
    std::shared_ptr<Symbol> find_symbol( const std::string& identifier ) const;
    std::shared_ptr<Symbol> find_symbol( const SyntaxNode* node ) const;
    static bool matches( const std::shared_ptr<Symbol>& symbol, const SyntaxNode* node, const std::vector<std::shared_ptr<SyntaxNode>>& node_parameters );
};

}

#endif
