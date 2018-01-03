//
// SyntaxNode.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_SYNTAXNODE_HPP_INCLUDED
#define SWEET_RENDER_SYNTAXNODE_HPP_INCLUDED

#include "declspec.hpp"
#include "SyntaxNodeType.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include "Instruction.hpp"
#include <sweet/math/vec3.hpp>
#include <sweet/math/mat4x4.hpp>
#include <sweet/pointer/ptr.hpp>
#include <string>
#include <vector>

namespace sweet
{

namespace render
{

class Symbol;

/**
// A node in the syntax tree generated when parsing a shader source file.
*/
class SWEET_RENDER_DECLSPEC SyntaxNode
{
public:
    static const int REGISTER_NULL = ~0; ///< Indicates that no register has been assigned.

private:
    SyntaxNodeType node_type_; ///< The node type of this SyntaxNode.
    std::string lexeme_; ///< The lexeme at this SyntaxNode.
    std::vector<ptr<SyntaxNode> > nodes_; ///< The SyntaxNodes that are children of this SyntaxNode.
    ptr<Symbol> symbol_; ///< The Symbol at this SyntaxNode (or null if there is no Symbol at this SyntaxNode).
    int constant_index_; ///< The index of the register that this SyntaxNode's constant value is stored in (or REGISTER_NULL if this SyntaxNode is not a constant).
    ValueType expected_type_; ///< The type that is expected at this SyntaxNode by other SyntaxNodes above it in the syntax tree (or TYPE_NULL if there is no expectation of type).
    ValueType original_type_; ///< The type of the expression at this SyntaxNode before conversion (or TYPE_NULL if this SyntaxNode is not type converted).
    ValueType type_; ///< The type of the expression at this SyntaxNode after conversion (or TYPE_NULL if this SyntaxNode is not an expression).
    ValueStorage expected_storage_; ///< The storage that is expected at this SyntaxNode by other SyntaxNodes above it in the syntax tree (or STORAGE_NULL if there is no expectation of storage).
    ValueStorage original_storage_; ///< The storage of at this SyntaxNode before conversion (or STORAGE_NULL is this SyntaxNode is not promoted).
    ValueStorage storage_; ///< The storage of the expression at this SyntaxNode after conversion (or STORAGE_NULL if this SyntaxNode is not an expression).
    Instruction instruction_; ///< The instruction to generate for this SyntaxNode (or INSTRUCTION_NULL if this SyntaxNode is not part an expression).
    int line_; ///< The line number that this SyntaxNode relates to.
    
public:
    SyntaxNode();    
    SyntaxNode( SyntaxNodeType node_type, int line );
    SyntaxNode( SyntaxNodeType node_type, int line, const char* lexeme );
    SyntaxNode( SyntaxNodeType node_type, int line, const std::string& lexeme );

    int line() const;
    void set_node_type( SyntaxNodeType type );
    SyntaxNodeType node_type() const;    
    
    void set_lexeme( const std::string& lexeme );
    const std::string& lexeme() const;
    float real() const;
    int integer() const;
    const std::string& string() const;
    math::vec3 vec3() const;
    math::mat4x4 mat4x4() const;
    
    void add_node( ptr<SyntaxNode> node );
    void add_node_at_front( ptr<SyntaxNode> node );
    void add_nodes_at_end( const std::vector<ptr<SyntaxNode> >::const_iterator begin, const std::vector<ptr<SyntaxNode> >::const_iterator end );
    SyntaxNode* node( int index ) const;
    const std::vector<ptr<SyntaxNode> >& get_nodes() const;
    
    void set_symbol( ptr<Symbol> symbol );
    ptr<Symbol> get_symbol() const;
    
    void set_constant_index( int index );
    int get_constant_index() const;
    
    void set_expected_type( ValueType type );
    ValueType get_expected_type() const;
    
    void set_type( ValueType type );
    ValueType get_type() const;

    void set_type_for_conversion( ValueType type );
    ValueType get_original_type() const;
    
    void set_expected_storage( ValueStorage storage );
    ValueStorage get_expected_storage() const;
    
    void set_storage( ValueStorage storage );
    ValueStorage get_storage() const;
    
    void set_storage_for_promotion( ValueStorage storage );
    ValueStorage get_original_storage() const;
    
    void set_instruction( Instruction instruction );
    Instruction get_instruction() const;
    
    bool operator==( const SyntaxNode& node ) const;
};

}

}

#endif