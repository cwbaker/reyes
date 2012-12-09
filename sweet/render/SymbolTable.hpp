//
// SymbolTable.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_SYMBOLTABLE_HPP_INCLUDED
#define SWEET_RENDER_SYMBOLTABLE_HPP_INCLUDED

#include "declspec.hpp"
#include "AddSymbolHelper.hpp"
#include <sweet/pointer/ptr.hpp>
#include <list>
#include <map>
#include <string>

namespace sweet
{

namespace render
{

class Symbol;
class SyntaxNode;

/**
// Hierarchical symbol table used to define the initial functions recognized
// by the shader parser as well as the parameters and variables that are
// defined during parsing.
*/
class SWEET_RENDER_DECLSPEC SymbolTable
{
    std::list<std::multimap<std::string, ptr<Symbol> > > symbols_; ///< The stack of symbols used to represent the symbol hierarchy.

public:
    SymbolTable();
    AddSymbolHelper add_symbols();
    void push_scope();
    void pop_scope();    
    ptr<Symbol> add_symbol( const std::string& identifier );
    ptr<Symbol> find_symbol( const std::string& identifier ) const;
    ptr<Symbol> find_symbol( const SyntaxNode* node ) const;
    static bool matches( const ptr<Symbol>& symbol, const SyntaxNode* node, const std::vector<ptr<SyntaxNode> >& node_parameters );
};

}

}

#endif
