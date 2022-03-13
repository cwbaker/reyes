#pragma once

#include "Symbol.hpp"
#include "Segment.hpp"
#include <memory>
#include <string>
#include <map>

namespace reyes
{

class SyntaxNode;

class Scope
{
    const Scope* parent_;
    std::multimap<std::string, std::shared_ptr<Symbol>> symbol_by_identifier_;
    std::vector<std::shared_ptr<Symbol>> symbols_;
    bool entered_;

public:
    Scope( const Scope* parent );
    std::shared_ptr<Symbol> find_symbol( const std::string& identifier ) const;
    std::shared_ptr<Symbol> find_symbol( const SyntaxNode* node ) const;
    Symbol* symbol( int index ) const;
    const std::vector<std::shared_ptr<Symbol>>& symbols() const;
    std::shared_ptr<Symbol> add_symbol( const std::string& identifier );
    int enter( Segment segment, int start_offset, int maximum_varying_length = 512 );
    int enter_strings( int start_offset );
    int leave( int base_offset );

private:
    static bool matches( const Symbol* symbol, const SyntaxNode* node, const std::vector<std::shared_ptr<SyntaxNode>>& node_parameters );    
};

}
