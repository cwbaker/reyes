//
// SymbolTable.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "SyntaxNode.hpp"
#include "Scope.hpp"
#include "assert.hpp"
#include <assert.h>

using std::multimap;
using std::list;
using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using namespace reyes;

SymbolTable::SymbolTable()
: scopes_()
, scopes_stack_()
{
    push_scope();
    add_symbols()
        .add_default_symbols()
    ;
}

SymbolTable::~SymbolTable()
{
}

std::shared_ptr<Symbol> SymbolTable::find_symbol( const std::string& identifier ) const
{
    assert( !scopes_stack_.empty() );
    assert( scopes_stack_.back() );
    Scope* scope = scopes_stack_.back();
    return scope->find_symbol( identifier );
}

std::shared_ptr<Symbol> SymbolTable::find_symbol( const SyntaxNode* node ) const
{
    assert( !scopes_stack_.empty() );
    assert( scopes_stack_.back() );
    Scope* scope = scopes_stack_.back();
    return scope->find_symbol( node );
}

Scope* SymbolTable::global_scope() const
{
    REYES_ASSERT( !scopes_.empty() );
    return scopes_.front().get();
}

Scope* SymbolTable::push_scope()
{
    Scope* parent = !scopes_stack_.empty() ? scopes_stack_.back() : nullptr;
    unique_ptr<Scope> scope( new Scope(parent) );
    scopes_stack_.push_back( scope.get() );
    scopes_.push_back( move(scope) );
    return scopes_.back().get();
}

Scope* SymbolTable::pop_scope()
{
    assert( !scopes_stack_.empty() );
    if ( !scopes_stack_.empty() )
    {
        Scope* scope = scopes_stack_.back();
        scopes_stack_.pop_back();
        return scope;
    }
    return nullptr;
}

std::shared_ptr<Symbol> SymbolTable::add_symbol( const std::string& identifier )
{
    assert( !scopes_stack_.empty() );
    Scope* scope = scopes_stack_.back();
    return scope->add_symbol( identifier );
}

AddSymbolHelper SymbolTable::add_symbols( Segment segment )
{
    return AddSymbolHelper( this, segment );
}
