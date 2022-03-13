#ifndef REYES_SYNTAXNODE_HPP_INCLUDED
#define REYES_SYNTAXNODE_HPP_INCLUDED

#include "SyntaxNodeType.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include "Address.hpp"
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <memory>
#include <string>
#include <vector>

namespace reyes
{

class Scope;
class Symbol;

/**
// A node in the syntax tree generated when parsing a shader source file.
*/
class SyntaxNode
{
    SyntaxNodeType node_type_; ///< The node type of this SyntaxNode.
    std::string lexeme_; ///< The lexeme at this SyntaxNode.
    std::vector<std::shared_ptr<SyntaxNode>> nodes_; ///< The SyntaxNodes that are children of this SyntaxNode.
    Scope* scope_; ///< The scope at this node (possibly null).
    std::shared_ptr<Symbol> symbol_; ///< The Symbol at this SyntaxNode (or null if there is no Symbol at this SyntaxNode).
    Address address_; ///< The address that this SyntaxNode's constant value is stored in (or 0 if this SyntaxNode is not a constant).
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
    SyntaxNodeType node_type() const;    
    const std::string& lexeme() const;
    float real() const;
    int integer() const;
    const std::string& string() const;
    math::vec3 vec3() const;
    math::mat4x4 mat4x4() const;
    SyntaxNode* node( int index ) const;
    const std::vector<std::shared_ptr<SyntaxNode>>& nodes() const;
    Scope* scope() const;
    const std::shared_ptr<Symbol>& symbol() const;
    Address address() const;
    ValueType expected_type() const;
    ValueType type() const;
    ValueType original_type() const;
    ValueStorage expected_storage() const;
    ValueStorage storage() const;
    ValueStorage original_storage() const;
    Instruction instruction() const;
    bool operator==( const SyntaxNode& node ) const;
    int count_by_type( SyntaxNodeType type ) const;
    
    void set_node_type( SyntaxNodeType type );
    void set_lexeme( const std::string& lexeme );
    void add_node( std::shared_ptr<SyntaxNode> node );
    void add_node_at_front( std::shared_ptr<SyntaxNode> node );
    void add_nodes_at_end( const std::vector<std::shared_ptr<SyntaxNode>>::const_iterator begin, const std::vector<std::shared_ptr<SyntaxNode>>::const_iterator end );   
    void set_scope( Scope* scope );
    void set_symbol( std::shared_ptr<Symbol> symbol );    
    void set_address( Address address );    
    void set_expected_type( ValueType type );    
    void set_type( ValueType type );
    void set_type_for_conversion( ValueType type );    
    void set_expected_storage( ValueStorage storage );    
    void set_storage( ValueStorage storage );    
    void set_storage_for_promotion( ValueStorage storage );    
    void set_instruction( Instruction instruction );    
};

}

#endif
