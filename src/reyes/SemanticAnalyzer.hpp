#ifndef REYES_SEMANTICANALYZER_HPP_INCLUDED
#define REYES_SEMANTICANALYZER_HPP_INCLUDED

#include "SyntaxNodeType.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <string>

namespace reyes
{

class SyntaxNode;
class SymbolTable;
class ErrorPolicy;

/**
// Traverse the syntax tree output by the parser and annotate it with type and 
// symbol information.
*/
class SemanticAnalyzer
{
    const SymbolTable& symbol_table_; ///< The SymbolTable to lookup symbols in.
    ErrorPolicy* error_policy_; ///< ErrorPolicy to report errors detected during semantic analysis to.
    bool ambient_light_; ///< True if the most recently analyzed syntax tree was a light shader referring to "Cl" or "Ol" globally.
    mutable int errors_; ///< The number of errors detected during code generation.

public:
    SemanticAnalyzer( const SymbolTable& symbol_table, ErrorPolicy* error_policy = NULL );
    void analyze( SyntaxNode* node, const char* name );
    bool ambient_light() const;

private:
    void error( bool condition, int line, const char* format, ... ) const;

    void analyze_ambient_lighting( SyntaxNode* node );
    void analyze_node( SyntaxNode* node ) const;
    
    void analyze_assign_expectations( SyntaxNode* node ) const;
    void analyze_typecast_expectations( SyntaxNode* node ) const;
    void analyze_expectations( SyntaxNode* node ) const;
    
    void analyze_if_statement( SyntaxNode* node ) const;
    void analyze_while_statement( SyntaxNode* node ) const;
    void analyze_for_statement( SyntaxNode* node ) const;
    void analyze_solar_statement( SyntaxNode* node ) const;
    void analyze_illuminate_statement( SyntaxNode* node ) const;    
    void analyze_illuminance_statement( SyntaxNode* node ) const;    
    void analyze_call( SyntaxNode* node ) const;
    void analyze_assign( SyntaxNode* node ) const;
    void analyze_dot( SyntaxNode* node ) const;
    void analyze_cross( SyntaxNode* node ) const;
    void analyze_multiply( SyntaxNode* node ) const;
    void analyze_divide( SyntaxNode* node ) const;
    void analyze_add( SyntaxNode* node ) const;
    void analyze_subtract( SyntaxNode* node ) const;
    void analyze_less( SyntaxNode* node ) const;
    void analyze_less_equal( SyntaxNode* node ) const;
    void analyze_greater( SyntaxNode* node ) const;
    void analyze_greater_equal( SyntaxNode* node ) const;
    void analyze_equal( SyntaxNode* node ) const;
    void analyze_not_equal( SyntaxNode* node ) const;
    void analyze_and( SyntaxNode* node ) const;
    void analyze_or( SyntaxNode* node ) const;
    void analyze_negate( SyntaxNode* node ) const;
    void analyze_typecast( SyntaxNode* node ) const;
    void analyze_ternary( SyntaxNode* node ) const;
    void analyze_identifier( SyntaxNode* node ) const;
    void analyze_literal( SyntaxNode* node ) const;
    void analyze_texture( SyntaxNode* node ) const;
    void analyze_shadow( SyntaxNode* node ) const;
    void analyze_environment( SyntaxNode* node ) const;
    
    void analyze_storage_promotion( SyntaxNode* node, ValueStorage to_storage ) const;
    void analyze_type_conversion( SyntaxNode* node, ValueType to_type ) const;
    void analyze_binary_operator( const struct OperationMetadata* metadatas, const char* name, SyntaxNode* operator_node ) const;
    const struct OperationMetadata* find_metadata( const struct OperationMetadata* metadata, ValueType lhs, ValueType rhs ) const;
    ValueType type_from_type_node( const SyntaxNode* type_node ) const;
    ValueType type_from_literal( const SyntaxNode* literal_node ) const;    
};

}

#endif
