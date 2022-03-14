//
// Scope.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "Scope.hpp"
#include "SyntaxNode.hpp"
#include <assert.h>

using std::string;
using std::vector;
using std::multimap;
using std::shared_ptr;
using std::make_shared;
using namespace reyes;

Scope::Scope( const Scope* parent )
: parent_( parent )
, symbol_by_identifier_()
, symbols_()
, entered_( false )
{
}

std::shared_ptr<Symbol> Scope::find_symbol( const std::string& identifier ) const
{
    multimap<string, std::shared_ptr<Symbol>>::const_iterator i = symbol_by_identifier_.find( identifier );
    if ( i != symbol_by_identifier_.end() )
    {
        return i->second;
    }
    if ( parent_ )
    {
        return parent_->find_symbol( identifier );
    }
    return std::shared_ptr<Symbol>();
}

std::shared_ptr<Symbol> Scope::find_symbol( const SyntaxNode* node ) const
{
    assert( node );
    auto range = symbol_by_identifier_.equal_range( node->lexeme() );
    for ( auto i = range.first; i != range.second; ++i )
    {
        if ( matches(i->second.get(), node, node->nodes()) )
        {
            return i->second;
        }
    }
    if ( parent_ )
    {
        return parent_->find_symbol( node );
    }
    return nullptr;
}

Symbol* Scope::symbol( int index ) const
{
    assert( index >= 0 && index < int(symbols_.size()) );
    return index >= 0 && index < int(symbols_.size()) ? symbols_[index].get() : nullptr;
}

const std::vector<std::shared_ptr<Symbol>>& Scope::symbols() const
{
    return symbols_;
}

std::shared_ptr<Symbol> Scope::add_symbol( const std::string& identifier )
{
    assert( !identifier.empty() );
    int index = int(symbols_.size());
    shared_ptr<Symbol> symbol = make_shared<Symbol>( identifier, index );
    symbol_by_identifier_.emplace( make_pair(identifier, symbol) );
    symbols_.push_back( symbol );
    return symbol;
}

int Scope::enter( Segment segment, int start_offset, int maximum_varying_length )
{
    assert( start_offset >= 0 );
    assert( maximum_varying_length >= 0 );
    int offset = start_offset;
    if ( !entered_ )
    {
        entered_ = true;
        for ( const shared_ptr<Symbol>& symbol : symbols_ )
        {
            assert( symbol );
            if ( symbol->storage() != STORAGE_NULL && symbol->storage() != STORAGE_CONSTANT && symbol->type() != TYPE_STRING && !symbol->function() )
            {
                int size = symbol->size_by_type_and_storage( maximum_varying_length );
                assert( size != 0 );
                symbol->set_address( Address(segment, offset) );
                offset += size;            
            }
        }
    }
    return offset;        
}

int Scope::enter_strings( int start_offset )
{
    assert( start_offset >= 0 );
    int offset = start_offset;
    for ( const shared_ptr<Symbol>& symbol : symbols_ )
    {
        assert( symbol );
        if ( symbol->type() == TYPE_STRING && !symbol->function() )
        {
            symbol->set_address( Address(SEGMENT_STRING, offset) );
            offset += 1;
        }
    }
    return offset;
}

int Scope::leave( int base_offset )
{
    assert( entered_ );
    entered_ = false;
    return base_offset;
}

bool Scope::matches( const Symbol* symbol, const SyntaxNode* node, const std::vector<shared_ptr<SyntaxNode>>& node_parameters )
{
    assert( symbol );
    assert( node );
    bool matches_return = symbol->matches_return( node->expected_type(), node->expected_storage() );
    const vector<SymbolParameter>& symbol_parameters = symbol->parameters();
    vector<SymbolParameter>::const_iterator i = symbol_parameters.begin();
    vector<shared_ptr<SyntaxNode>>::const_iterator j = node_parameters.begin();
    while ( i != symbol_parameters.end() && j != node_parameters.end() && i->matches((*j)->type(), (*j)->storage()) )
    {
        ++i;
        ++j;
    }
    return matches_return && i == symbol_parameters.end() && j == node_parameters.end();
}
