//
// SemanticAnalyzer.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "SemanticAnalyzer.hpp"
#include "SyntaxNode.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "ValueType.hpp"
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include "ErrorCode.hpp"
#include "ErrorPolicy.hpp"
#include "assert.hpp"
#include <algorithm>
#include <stdio.h>

using std::max;
using std::string;
using std::vector;
using std::shared_ptr;
using namespace reyes;

namespace reyes
{

enum ValueTypeMask
{
    INTEGER = 1 << TYPE_INTEGER,
    FLOAT = 1 << TYPE_FLOAT,
    COLOR = 1 << TYPE_COLOR,
    POINT = 1 << TYPE_POINT,
    VECTOR = 1 << TYPE_VECTOR,
    NORMAL = 1 << TYPE_NORMAL,
    MATRIX = 1 << TYPE_MATRIX,
    STRING = 1 << TYPE_STRING
};

struct OperationMetadata
{
    int lhs;
    int rhs;
    ValueType result;
    Instruction instruction;
};

}

SemanticAnalyzer::SemanticAnalyzer( SymbolTable* symbol_table, ErrorPolicy* error_policy )
: symbol_table_( symbol_table ),
  error_policy_( error_policy ),
  ambient_light_( false ),
  errors_( 0 )
{
    REYES_ASSERT( symbol_table_ );
}

void SemanticAnalyzer::analyze( SyntaxNode* node, const char* name )
{
    REYES_ASSERT( name );
    
    ambient_light_ = false;
    errors_ = 0;

    if ( node && error_policy_->errors() == 0 )
    {
        analyze_ambient_lighting( node->node(0) );
        analyze_node( node->node(0) );

        if ( errors_ > 0 && error_policy_ )
        {
            error_policy_->error( RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED, "Analysis of shader '%s' failed", name );
        }
    }
}

void SemanticAnalyzer::error( bool condition, int line, const char* format, ... ) const
{       
    if ( condition )
    {
        ++errors_;
        
        if ( error_policy_ )
        {
            char message [1024];
            va_list args;
            va_start( args, format );
            vsnprintf( message, sizeof(message), format, args );
            va_end( args );
            
            error_policy_->error( RENDER_ERROR_SEMANTIC_ERROR, "(%d): %s", line, message );
        }
    }
}

void SemanticAnalyzer::analyze_ambient_lighting( SyntaxNode* node )
{
    REYES_ASSERT( node );
    
    if ( node->node_type() == SHADER_NODE_LIGHT_SHADER )
    {
        int ambient_assignments = 0;
        int solar_statements = 0;
        int illuminate_statements = 0;
        
        const vector<shared_ptr<SyntaxNode>>& statements = node->node(1)->nodes();        
        for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = statements.begin(); i != statements.end(); ++i )
        {
            const SyntaxNode* node = i->get();
            REYES_ASSERT( node );
            ambient_assignments += node->node_type() >= SHADER_NODE_ASSIGN && node->node_type() <= SHADER_NODE_DIVIDE_ASSIGN && (node->lexeme() == "Cl" || node->lexeme() == "Ol");
            solar_statements += node->node_type() == SHADER_NODE_SOLAR;
            illuminate_statements += node->node_type() == SHADER_NODE_ILLUMINATE;
        }
        
        if ( ambient_assignments > 0 )
        {
            error( solar_statements > 0, node->line(), "Illegal assignment to 'Cl' or 'Ol' with solar statements" );
            error( illuminate_statements > 0, node->line(), "Illegal assignment to 'Cl' or 'Ol' with illuminate statements" );
        }
    }
}

void SemanticAnalyzer::analyze_nodes( SyntaxNode* node ) const
{
    REYES_ASSERT( node );    
    const vector<shared_ptr<SyntaxNode>>& nodes = node->nodes();
    for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        SyntaxNode* syntax_node = i->get();
        REYES_ASSERT( syntax_node );
        analyze_node( syntax_node );
    }
}

void SemanticAnalyzer::analyze_node( SyntaxNode* node ) const
{
    REYES_ASSERT( node );
    
    switch ( node->node_type() )
    {
        case SHADER_NODE_VARIABLE:
            analyze_variable_expectations( node );
            break;

        case SHADER_NODE_ASSIGN:
        case SHADER_NODE_ADD_ASSIGN:
        case SHADER_NODE_SUBTRACT_ASSIGN:
        case SHADER_NODE_MULTIPLY_ASSIGN:
        case SHADER_NODE_DIVIDE_ASSIGN:
            analyze_assign_expectations( node );
            break;

        case SHADER_NODE_TYPECAST:
            analyze_typecast_expectations( node );
            break;
        
        case SHADER_NODE_DOT:
        case SHADER_NODE_CROSS:
        case SHADER_NODE_MULTIPLY:
        case SHADER_NODE_DIVIDE:
        case SHADER_NODE_ADD:
        case SHADER_NODE_SUBTRACT:
        case SHADER_NODE_GREATER:
        case SHADER_NODE_GREATER_EQUAL:
        case SHADER_NODE_LESS:
        case SHADER_NODE_LESS_EQUAL:
        case SHADER_NODE_EQUAL:
        case SHADER_NODE_NOT_EQUAL:
        case SHADER_NODE_AND:
        case SHADER_NODE_OR:
        case SHADER_NODE_NEGATE:
        case SHADER_NODE_TERNARY:
            analyze_expectations( node );
            break;
            
        default:
            break;
    }

    switch ( node->node_type() )
    {
        case SHADER_NODE_LIGHT_SHADER:
            analyze_light_shader( node );
            break;

        case SHADER_NODE_SURFACE_SHADER:
            analyze_surface_shader( node );
            break;

        case SHADER_NODE_VOLUME_SHADER:
            analyze_volume_shader( node );
            break;

        case SHADER_NODE_DISPLACEMENT_SHADER:
            analyze_displacement_shader( node );
            break;

        case SHADER_NODE_IMAGER_SHADER:
            analyze_imager_shader( node );
            break;

        case SHADER_NODE_FUNCTION:
            analyze_function( node );
            break;

        case SHADER_NODE_VARIABLE:
            analyze_variable( node );
            break;

        case SHADER_NODE_CALL:
            analyze_call( node );
            break;
    
        case SHADER_NODE_IF:
        case SHADER_NODE_IF_ELSE:
            analyze_if_statement( node );
            break;

        case SHADER_NODE_WHILE:
            analyze_while_statement( node );
            break;

        case SHADER_NODE_FOR:
            analyze_for_statement( node );
            break;

        case SHADER_NODE_SOLAR:
            analyze_solar_statement( node );
            break;
            
        case SHADER_NODE_ILLUMINATE:
            analyze_illuminate_statement( node );
            break;
            
        case SHADER_NODE_ILLUMINANCE:
            analyze_illuminance_statement( node );
            break;

        case SHADER_NODE_ASSIGN:
        case SHADER_NODE_ADD_ASSIGN:
        case SHADER_NODE_SUBTRACT_ASSIGN:
        case SHADER_NODE_MULTIPLY_ASSIGN:
        case SHADER_NODE_DIVIDE_ASSIGN:
            analyze_assign( node );
            break;
        
        case SHADER_NODE_DOT:
            analyze_dot( node );
            break;
            
        case SHADER_NODE_CROSS:
            analyze_cross( node );
            break;
        
        case SHADER_NODE_MULTIPLY:
            analyze_multiply( node );
            break;
        
        case SHADER_NODE_DIVIDE:
            analyze_divide( node );
            break;
        
        case SHADER_NODE_ADD:
            analyze_add( node );
            break;
        
        case SHADER_NODE_SUBTRACT:
            analyze_subtract( node );
            break;

        case SHADER_NODE_GREATER:
            analyze_greater( node );
            break;
        
        case SHADER_NODE_GREATER_EQUAL:
            analyze_greater_equal( node );
            break;
        
        case SHADER_NODE_LESS:
            analyze_less( node );
            break;
        
        case SHADER_NODE_LESS_EQUAL:
            analyze_less_equal( node );
            break;
        
        case SHADER_NODE_EQUAL:
            analyze_equal( node );
            break;
        
        case SHADER_NODE_NOT_EQUAL:
            analyze_not_equal( node );
            break;
        
        case SHADER_NODE_AND:
            analyze_and( node );
            break;
            
        case SHADER_NODE_OR:
            analyze_or( node );
            break;
            
        case SHADER_NODE_NEGATE:
            analyze_negate( node );
            break;
            
        case SHADER_NODE_TERNARY:
            analyze_ternary( node );
            break;
            
        case SHADER_NODE_TYPECAST:
            analyze_typecast( node );
            break;

        case SHADER_NODE_IDENTIFIER:
            analyze_identifier( node );
            break;

        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        case SHADER_NODE_STRING:
        case SHADER_NODE_TRIPLE:
        case SHADER_NODE_SIXTEENTUPLE:
            analyze_literal( node );
            break;
        
        case SHADER_NODE_TEXTURE:
            analyze_texture( node );
            break;

        case SHADER_NODE_SHADOW:
            analyze_shadow( node );
            break;

        case SHADER_NODE_ENVIRONMENT:
            analyze_environment( node );
            break;

        default:
            analyze_nodes( node );
            node->set_type( TYPE_NULL );
            node->set_storage( STORAGE_NULL );
            node->set_instruction( INSTRUCTION_NULL );            
            break;
    }
}

void SemanticAnalyzer::analyze_variable_expectations( SyntaxNode* variable ) const
{
    const string& identifier = variable->lexeme();
    const SyntaxNode* storage_node = variable->node( 1 );
    const SyntaxNode* type_node = variable->node( 2 );

    ValueStorage storage = storage_from_syntax_node( storage_node );
    ValueType type = type_from_syntax_node( type_node );
    error( type == TYPE_NULL, variable->line(), "variable '%s' has no type", identifier.c_str() );

    shared_ptr<Symbol> symbol = add_symbol( identifier );
    symbol->set_storage( storage );
    symbol->set_type( type );

    variable->set_symbol( symbol );
    variable->set_type( type );
    variable->set_storage( storage );

    analyze_assign_expectations( variable );
}

void SemanticAnalyzer::analyze_assign_expectations( SyntaxNode* assign ) const
{
    REYES_ASSERT( assign );

    shared_ptr<Symbol> symbol = assign->symbol();
    if ( !symbol )
    {
        symbol = find_symbol( assign->lexeme() );
        error( !symbol, assign->line(), "Unrecognized symbol '%s' in assignment", assign->lexeme().c_str() );
        if ( symbol )
        {
            assign->set_symbol( symbol );
        }
    }

    if ( symbol )
    {
        ValueStorage expected_storage = symbol->storage();        
        const vector<shared_ptr<SyntaxNode>>& nodes = assign->nodes();
        for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
        {
            SyntaxNode* syntax_node = i->get();
            REYES_ASSERT( syntax_node );
            syntax_node->set_expected_storage( expected_storage );
        }
    }
}

void SemanticAnalyzer::analyze_typecast_expectations( SyntaxNode* node ) const
{
    REYES_ASSERT( node );
    REYES_ASSERT( node->node_type() == SHADER_NODE_TYPECAST );

    ValueType expected_type = type_from_syntax_node( node->node(0) );
    ValueStorage expected_storage = node->expected_storage();
    
    const vector<shared_ptr<SyntaxNode>>& nodes = node->nodes();
    for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        SyntaxNode* syntax_node = i->get();
        REYES_ASSERT( syntax_node );
        syntax_node->set_expected_type( expected_type );
        syntax_node->set_expected_storage( expected_storage );
    }
}

void SemanticAnalyzer::analyze_expectations( SyntaxNode* node ) const
{
    REYES_ASSERT( node );
    
    ValueType expected_type = node->expected_type();
    ValueStorage expected_storage = node->expected_storage();
    
    const vector<shared_ptr<SyntaxNode>>& nodes = node->nodes();
    for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        SyntaxNode* syntax_node = i->get();
        REYES_ASSERT( syntax_node );
        syntax_node->set_expected_type( expected_type );
        syntax_node->set_expected_storage( expected_storage );
    }
}

void SemanticAnalyzer::analyze_if_statement( SyntaxNode* node ) const
{
    Scope* scope = push_scope();
    node->set_scope( scope );
    analyze_nodes( node );
    pop_scope();

    analyze_storage_promotion( node->node(0), STORAGE_VARYING );
}

void SemanticAnalyzer::analyze_while_statement( SyntaxNode* node ) const
{
    Scope* scope = push_scope();
    node->set_scope( scope );
    analyze_nodes( node );
    pop_scope();

    analyze_storage_promotion( node->node(0), STORAGE_VARYING );
}

void SemanticAnalyzer::analyze_for_statement( SyntaxNode* node ) const
{    
    Scope* scope = push_scope();
    node->set_scope( scope );
    analyze_nodes( node );
    pop_scope();

    analyze_storage_promotion( node->node(1), STORAGE_VARYING );
}

void SemanticAnalyzer::analyze_solar_statement( SyntaxNode* solar ) const
{
    Scope* scope = push_scope();
    solar->set_scope( scope );
    symbol_table_->add_symbols().add_lighting_symbols();
    analyze_nodes( solar );
    pop_scope();

    const SyntaxNode* expressions = solar->node( 0 );
    if ( !expressions->nodes().empty() )
    {
        error( expressions->node(0)->storage() == STORAGE_VARYING, solar->line(), "The 'solar' statement axis must be constant or uniform" );
        error( expressions->node(1)->storage() == STORAGE_VARYING, solar->line(), "The 'solar' statement angle must be constant or uniform" );
        error( expressions->node(1)->type() != TYPE_FLOAT, solar->line(), "The 'solar' statement angle must be a float" );
    }
}

void SemanticAnalyzer::analyze_illuminate_statement( SyntaxNode* illuminate ) const
{
    Scope* scope = push_scope();
    illuminate->set_scope( scope );
    symbol_table_->add_symbols().add_lighting_symbols();
    analyze_nodes( illuminate );
    pop_scope();

    const SyntaxNode* expressions = illuminate->node( 0 );
    error( expressions->node(0)->storage() == STORAGE_VARYING, illuminate->line(), "The 'illuminate' statement position must be constant or uniform" );
    if ( expressions->nodes().size() > 1 )
    {
        error( expressions->node(1)->storage() == STORAGE_VARYING, illuminate->line(), "The 'illuminate' statement axis must be constant or uniform" );
        error( expressions->node(2)->storage() == STORAGE_VARYING, illuminate->line(), "The 'illuminate' statement angle must be constant or uniform" );
        error( expressions->node(2)->type() != TYPE_FLOAT, illuminate->line(), "The 'illuminate' statement axis must be a float" );
    }
}

void SemanticAnalyzer::analyze_illuminance_statement( SyntaxNode* illuminance ) const
{
    REYES_ASSERT( illuminance );

    Scope* scope = push_scope();
    illuminance->set_scope( scope );
    symbol_table_->add_symbols().add_illuminance_symbols();
    analyze_nodes( illuminance );
    pop_scope();

    const SyntaxNode* expressions = illuminance->node( 0 );
    if ( expressions->nodes().size() == 1 )
    {
        analyze_storage_promotion( expressions->node(0), STORAGE_VARYING );
        error( true, illuminance->line(), "The unconditional 'illuminance' statement is not supported" );
    }
    else
    {
        analyze_storage_promotion( expressions->node(0), STORAGE_VARYING );
        analyze_storage_promotion( expressions->node(1), STORAGE_VARYING );
        error( expressions->node(2)->storage() == STORAGE_VARYING, illuminance->line(), "The 'illuminance' statement angle must be constant or uniform" );
        error( expressions->node(2)->type() != TYPE_FLOAT, illuminance->line(), "The 'illuminance' statement angle must be a float" );
    }    
}

void SemanticAnalyzer::analyze_call( SyntaxNode* node ) const
{
    analyze_nodes( node );

    shared_ptr<Symbol> symbol = symbol_table_->find_symbol( node );
    error( !symbol, node->line(), "Unrecognized function '%s'", node->lexeme().c_str() );
    if ( symbol )
    {
        node->set_symbol( symbol );
        node->set_type( symbol->type() );
        node->set_storage( symbol->storage() );
        
        const vector<SymbolParameter>& parameters = symbol->parameters();
        const vector<shared_ptr<SyntaxNode>>& nodes = node->nodes();
        REYES_ASSERT( parameters.size() == nodes.size() );
        const int MAXIMUM_PARAMETERS = 5;
        error( parameters.size() > MAXIMUM_PARAMETERS, node->line(), "The call '%s' has more than the %d parameters", symbol->identifier().c_str(), MAXIMUM_PARAMETERS );
        for ( unsigned int i = 0; i < parameters.size(); ++i )
        {
            analyze_type_conversion( nodes[i].get(), parameters[i].type() );
            analyze_storage_promotion( nodes[i].get(), parameters[i].storage() );
        }
    }
}

void SemanticAnalyzer::analyze_shader( SyntaxNode* shader ) const
{
    const string& identifier = shader->lexeme();
    SyntaxNode* parameters = shader->node( 0 );
    SyntaxNode* statements = shader->node( 1 );

    shared_ptr<Symbol> symbol = add_symbol( identifier );
    shader->set_symbol( symbol );

    parameters->set_scope( push_scope() );
    analyze_nodes( parameters );
    statements->set_scope( push_scope() );
    analyze_nodes( statements );
    pop_scope();
    pop_scope();
}

void SemanticAnalyzer::analyze_light_shader( SyntaxNode* light_shader ) const
{
    Scope* scope = push_scope();
    light_shader->set_scope( scope );
    symbol_table_->add_symbols().add_light_symbols();
    analyze_shader( light_shader );

    int solar_statements = light_shader->count_by_type( SHADER_NODE_SOLAR );
    int illuminate_statements = light_shader->count_by_type( SHADER_NODE_ILLUMINATE );
    if ( solar_statements == 0 && illuminate_statements == 0 )
    {
        const char* LIGHT_COLOR = "Cl";
        shared_ptr<SyntaxNode> light_color( new SyntaxNode(SHADER_NODE_IDENTIFIER, light_shader->line(), LIGHT_COLOR) );
        light_color->set_symbol( symbol_table_->find_symbol(LIGHT_COLOR) );

        const char* LIGHT_OPACITY = "Ol";
        shared_ptr<SyntaxNode> light_opacity( new SyntaxNode(SHADER_NODE_IDENTIFIER, light_shader->line(), LIGHT_OPACITY) );
        light_opacity->set_symbol( symbol_table_->find_symbol(LIGHT_OPACITY) );

        shared_ptr<SyntaxNode> ambient( new SyntaxNode(SHADER_NODE_AMBIENT, light_shader->line()) );
        ambient->add_node( light_color );
        ambient->add_node( light_opacity );

        SyntaxNode* statements = light_shader->node( 1 );
        statements->add_node_at_front( ambient );
    }

    pop_scope();
}

void SemanticAnalyzer::analyze_surface_shader( SyntaxNode* surface_shader ) const
{
    Scope* scope = push_scope();
    surface_shader->set_scope( scope );
    symbol_table_->add_symbols().add_surface_symbols();
    analyze_shader( surface_shader );
    pop_scope();
}

void SemanticAnalyzer::analyze_volume_shader( SyntaxNode* volume_shader ) const
{
    Scope* scope = push_scope();
    volume_shader->set_scope( scope );
    symbol_table_->add_symbols().add_volume_symbols();
    analyze_shader( volume_shader );
    pop_scope();
}

void SemanticAnalyzer::analyze_displacement_shader( SyntaxNode* displacement_shader ) const
{
    Scope* scope = push_scope();
    displacement_shader->set_scope( scope );
    symbol_table_->add_symbols().add_displacement_symbols();
    analyze_shader( displacement_shader );
    pop_scope();
}

void SemanticAnalyzer::analyze_imager_shader( SyntaxNode* imager_shader ) const
{
    Scope* scope = push_scope();
    imager_shader->set_scope( scope );
    symbol_table_->add_symbols().add_imager_symbols();
    analyze_shader( imager_shader );
    pop_scope();
}

void SemanticAnalyzer::analyze_function( SyntaxNode* function ) const
{
    Scope* scope = push_scope();
    analyze_nodes( function );
    pop_scope();

    const string& identifier = function->lexeme();
    const SyntaxNode* type_node = function->node( 2 );
    ValueType type = type_from_syntax_node( type_node );

    shared_ptr<Symbol> symbol = add_symbol( identifier );
    symbol->set_type( type );

    function->set_type( type );
    function->set_symbol( symbol );
}

void SemanticAnalyzer::analyze_variable( SyntaxNode* variable ) const
{
    analyze_assign( variable );
    const string& identifier = variable->lexeme();
    const SyntaxNode* expression = variable->node( 0 );
    const shared_ptr<Symbol>& symbol = variable->symbol();
    if ( symbol->storage() == STORAGE_NULL )
    {
        symbol->set_storage( max(expression->storage(), STORAGE_UNIFORM) );
    }
    bool assigning_varying_to_uniform = variable->is_uniform() && expression->is_varying();
    error( assigning_varying_to_uniform, variable->line(), "Varying assignment to uniform '%s'", identifier.c_str() );
}

void SemanticAnalyzer::analyze_assign( SyntaxNode* node ) const
{
    analyze_nodes( node );
    shared_ptr<Symbol> symbol = node->symbol();
    if ( symbol )
    {
        static const OperationMetadata ASSIGNMENT_METADATA[] =
        {
            { INTEGER, INTEGER, TYPE_NULL, INSTRUCTION_NULL },
            { FLOAT, FLOAT, TYPE_NULL, INSTRUCTION_NULL },
            { COLOR, FLOAT | COLOR, TYPE_NULL, INSTRUCTION_NULL },
            { POINT | VECTOR | NORMAL, FLOAT | POINT | VECTOR | NORMAL, TYPE_NULL, INSTRUCTION_NULL },
            { MATRIX, FLOAT | MATRIX },
            { STRING, STRING },
            { 0, 0, TYPE_NULL, INSTRUCTION_NULL }
        };        
        const OperationMetadata* metadata = find_metadata( ASSIGNMENT_METADATA, symbol->type(), node->node(0)->type() );
        error( !metadata && node->node(0)->node_type() != SHADER_NODE_NULL, node->line(), "Invalid types in assignment to '%s'", node->lexeme().c_str() );        
        error( symbol->storage() == STORAGE_CONSTANT, node->line(), "Assignment to constant '%s'", symbol->identifier().c_str() );
        
        analyze_type_conversion( node->node(0), symbol->type() );
        analyze_storage_promotion( node->node(0), symbol->storage() );
        node->set_type( symbol->type() );
        node->set_storage( symbol->storage() );
    }
}

void SemanticAnalyzer::analyze_dot( SyntaxNode* node ) const
{
    static const OperationMetadata DOT_METADATA[] =
    {
        { COLOR, COLOR, TYPE_FLOAT, INSTRUCTION_DOT },
        { POINT | VECTOR | NORMAL, POINT | VECTOR | NORMAL, TYPE_FLOAT, INSTRUCTION_DOT },
        { 0, 0, TYPE_NULL, INSTRUCTION_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( DOT_METADATA, ".", node );
}

void SemanticAnalyzer::analyze_cross( SyntaxNode* node ) const
{
    static const OperationMetadata CROSS_METADATA[] =
    {
        { POINT | VECTOR | NORMAL, POINT | VECTOR | NORMAL, TYPE_VECTOR, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( CROSS_METADATA, "%", node );
}

void SemanticAnalyzer::analyze_multiply( SyntaxNode* node ) const
{
    static const OperationMetadata MULTIPLY_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_FLOAT, INSTRUCTION_MULTIPLY },
        { COLOR, COLOR, TYPE_COLOR, INSTRUCTION_MULTIPLY },
        { POINT, POINT, TYPE_POINT, INSTRUCTION_MULTIPLY },
        { POINT | NORMAL | VECTOR, POINT | NORMAL | VECTOR, TYPE_VECTOR, INSTRUCTION_MULTIPLY },
        { MATRIX, MATRIX, TYPE_MATRIX, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL }
    };            
    analyze_nodes( node );
    analyze_binary_operator( MULTIPLY_METADATA, "*", node );
}

void SemanticAnalyzer::analyze_divide( SyntaxNode* node ) const
{
    static const OperationMetadata DIVIDE_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_FLOAT, INSTRUCTION_DIVIDE },
        { COLOR, FLOAT, TYPE_COLOR, INSTRUCTION_DIVIDE },
        { POINT, FLOAT, TYPE_POINT, INSTRUCTION_DIVIDE },
        { NORMAL, FLOAT, TYPE_NORMAL, INSTRUCTION_DIVIDE },
        { VECTOR, FLOAT, TYPE_VECTOR, INSTRUCTION_DIVIDE },
        { FLOAT, MATRIX, TYPE_MATRIX, INSTRUCTION_NULL },
        { MATRIX, MATRIX, TYPE_MATRIX, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL }
    };            

    analyze_nodes( node );
    analyze_storage_promotion( node->node(0), node->node(1)->storage() );
    analyze_storage_promotion( node->node(1), node->node(0)->storage() );

    const OperationMetadata* metadata = find_metadata( DIVIDE_METADATA, node->node(0)->type(), node->node(1)->type() );
    error( metadata == NULL, node->line(), "Invalid arguments to '/' operator" );
    if ( metadata )
    {
        node->set_type( metadata->result );
        node->set_storage( max(node->node(0)->storage(), node->node(1)->storage()) );
    }
}

void SemanticAnalyzer::analyze_add( SyntaxNode* node ) const
{
    const OperationMetadata ADD_METADATA[] = 
    {
        { FLOAT, FLOAT, TYPE_FLOAT, INSTRUCTION_ADD },
        { COLOR, COLOR, TYPE_COLOR, INSTRUCTION_ADD },
        { VECTOR | NORMAL, VECTOR | NORMAL, TYPE_VECTOR, INSTRUCTION_ADD },
        { POINT | VECTOR | NORMAL, POINT | VECTOR | NORMAL, TYPE_POINT, INSTRUCTION_ADD },
        { MATRIX, MATRIX, TYPE_MATRIX, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( ADD_METADATA, "+", node );
}

void SemanticAnalyzer::analyze_subtract( SyntaxNode* node ) const
{
    const OperationMetadata SUBTRACT_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_FLOAT, INSTRUCTION_SUBTRACT },
        { COLOR, COLOR, TYPE_COLOR, INSTRUCTION_SUBTRACT },
        { POINT | NORMAL | VECTOR, POINT | NORMAL | VECTOR, TYPE_VECTOR, INSTRUCTION_SUBTRACT },
        { MATRIX, MATRIX, TYPE_MATRIX, INSTRUCTION_SUBTRACT }
    };
    analyze_nodes( node );
    analyze_binary_operator( SUBTRACT_METADATA, "-", node );
}

void SemanticAnalyzer::analyze_less( SyntaxNode* node ) const
{
    const OperationMetadata LESS_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_INTEGER, INSTRUCTION_LESS },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( LESS_METADATA, "<", node );
}

void SemanticAnalyzer::analyze_less_equal( SyntaxNode* node ) const
{
    const OperationMetadata LESS_EQUAL_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_INTEGER, INSTRUCTION_LESS_EQUAL },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( LESS_EQUAL_METADATA, "<=", node );
}

void SemanticAnalyzer::analyze_greater( SyntaxNode* node ) const
{
    const OperationMetadata GREATER_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_INTEGER, INSTRUCTION_GREATER },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( GREATER_METADATA, ">", node );
}

void SemanticAnalyzer::analyze_greater_equal( SyntaxNode* node ) const
{
    const OperationMetadata GREATER_EQUAL_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_INTEGER, INSTRUCTION_GREATER_EQUAL },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( GREATER_EQUAL_METADATA, ">=", node );
}

void SemanticAnalyzer::analyze_equal( SyntaxNode* node ) const
{
    const OperationMetadata EQUAL_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_INTEGER, INSTRUCTION_EQUAL },
        { COLOR, COLOR, TYPE_INTEGER, INSTRUCTION_EQUAL },
        { POINT | NORMAL | VECTOR, POINT | NORMAL | VECTOR, TYPE_INTEGER, INSTRUCTION_EQUAL },
        { MATRIX, MATRIX, TYPE_INTEGER, INSTRUCTION_NULL },
        { STRING, STRING, TYPE_INTEGER, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( EQUAL_METADATA, "==", node );
}

void SemanticAnalyzer::analyze_not_equal( SyntaxNode* node ) const
{
    const OperationMetadata NOT_EQUAL_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_INTEGER, INSTRUCTION_NOT_EQUAL },
        { COLOR, COLOR, TYPE_INTEGER, INSTRUCTION_NOT_EQUAL },
        { POINT | NORMAL | VECTOR, POINT | NORMAL | VECTOR, TYPE_INTEGER, INSTRUCTION_NOT_EQUAL },
        { MATRIX, MATRIX, TYPE_INTEGER, INSTRUCTION_NULL },
        { STRING, STRING, TYPE_INTEGER, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( NOT_EQUAL_METADATA, "!=", node );
}

void SemanticAnalyzer::analyze_and( SyntaxNode* node ) const
{
    const OperationMetadata AND_METADATA[] =            
    {
        { INTEGER, INTEGER, TYPE_INTEGER, INSTRUCTION_AND },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( AND_METADATA, "&&", node );
}

void SemanticAnalyzer::analyze_or( SyntaxNode* node ) const
{
    const OperationMetadata OR_METADATA[] =            
    {
        { INTEGER, INTEGER, TYPE_INTEGER, INSTRUCTION_OR },
        { 0, 0, TYPE_NULL }
    };
    analyze_nodes( node );
    analyze_binary_operator( OR_METADATA, "||", node );
}

void SemanticAnalyzer::analyze_negate( SyntaxNode* node ) const
{    
    analyze_nodes( node );
    node->set_type( node->node(0)->type() );
    node->set_storage( node->node(0)->storage() );    
}

void SemanticAnalyzer::analyze_typecast( SyntaxNode* node ) const
{
    analyze_nodes( node );
    node->set_type( type_from_syntax_node(node->node(0)) );
    node->set_storage( node->node(1)->storage() );
    analyze_type_conversion( node->node(1), node->type() );
}

void SemanticAnalyzer::analyze_ternary( SyntaxNode* node ) const
{
    const OperationMetadata TERNARY_METADATA[] =
    {
        { FLOAT, FLOAT, TYPE_FLOAT, INSTRUCTION_NULL },
        { COLOR, COLOR, TYPE_COLOR, INSTRUCTION_NULL },
        { VECTOR | NORMAL, VECTOR | NORMAL, TYPE_VECTOR, INSTRUCTION_NULL },
        { POINT | VECTOR | NORMAL, POINT | VECTOR | NORMAL, TYPE_POINT, INSTRUCTION_NULL },
        { MATRIX, MATRIX, TYPE_MATRIX, INSTRUCTION_NULL },
        { 0, 0, TYPE_NULL, INSTRUCTION_NULL }
    };

    REYES_ASSERT( node );
    REYES_ASSERT( node->nodes().size() == 3 );
    REYES_ASSERT( node->node(1) );
    REYES_ASSERT( node->node(2) );    
    
    analyze_nodes( node );
    const OperationMetadata* metadata = find_metadata( TERNARY_METADATA, node->node(1)->type(), node->node(2)->type() );
    error( metadata == NULL, node->line(), "Invalid arguments to ternary operator" );
    if ( metadata )
    {
        node->set_type( metadata->result );
        node->set_storage( max(node->node(1)->storage(), node->node(2)->storage()) );
    }
}

void SemanticAnalyzer::analyze_identifier( SyntaxNode* node ) const
{
    analyze_nodes( node );
    shared_ptr<Symbol> symbol = find_symbol( node->lexeme() );
    error( !symbol, node->line(), "Unrecognized symbol '%s'", node->lexeme().c_str() );
    if ( symbol )
    {
        node->set_symbol( symbol );
        node->set_type( symbol->type() );
        node->set_storage( symbol->storage() );
    }
}

void SemanticAnalyzer::analyze_literal( SyntaxNode* node ) const
{
    analyze_nodes( node );
    node->set_type( type_from_literal(node) );
    node->set_storage( STORAGE_CONSTANT );
}

void SemanticAnalyzer::analyze_texture( SyntaxNode* node ) const
{
    analyze_nodes( node );
    node->set_type( TYPE_COLOR );
    node->set_storage( STORAGE_VARYING );
}

void SemanticAnalyzer::analyze_shadow( SyntaxNode* node ) const
{
    analyze_nodes( node );
    node->set_type( TYPE_FLOAT );
    node->set_storage( STORAGE_VARYING );
}

void SemanticAnalyzer::analyze_environment( SyntaxNode* node ) const
{
    analyze_nodes( node );
    node->set_type( TYPE_COLOR );
    node->set_storage( STORAGE_VARYING );
}

void SemanticAnalyzer::analyze_storage_promotion( SyntaxNode* node, ValueStorage to_storage ) const
{
    REYES_ASSERT( node );
    if ( node->storage() != STORAGE_VARYING && to_storage == STORAGE_VARYING )
    {
        node->set_storage_for_promotion( to_storage );
    }
}

void SemanticAnalyzer::analyze_type_conversion( SyntaxNode* node, ValueType to_type  ) const
{
    REYES_ASSERT( node );
    if ( node->type() == TYPE_FLOAT && to_type != TYPE_FLOAT )    
    {
        node->set_type_for_conversion( to_type );
    }
}

void SemanticAnalyzer::analyze_binary_operator( const OperationMetadata* operation_metadata, const char* name, SyntaxNode* node ) const
{
    REYES_ASSERT( operation_metadata );
    REYES_ASSERT( name );
    REYES_ASSERT( node );
    REYES_ASSERT( node->nodes().size() == 2 );
    REYES_ASSERT( node->node(0) );
    REYES_ASSERT( node->node(1) );    
    
    analyze_type_conversion( node->node(0), node->node(1)->type() );
    analyze_storage_promotion( node->node(0), node->node(1)->storage() );
    analyze_type_conversion( node->node(1), node->node(0)->type() );
    analyze_storage_promotion( node->node(1), node->node(0)->storage() );

    const OperationMetadata* metadata = find_metadata( operation_metadata, node->node(0)->type(), node->node(1)->type() );
    error( !metadata, node->line(), "Invalid arguments to '%s' operator", name );
    if ( metadata )
    {
        node->set_type( metadata->result );
        node->set_storage( max(node->node(0)->storage(), node->node(1)->storage()) );
        node->set_instruction( metadata->instruction );
    }
}

const OperationMetadata* SemanticAnalyzer::find_metadata( const OperationMetadata* metadata, ValueType lhs, ValueType rhs ) const
{
    while ( metadata->lhs != 0 && ((metadata->lhs & (1 << lhs)) == 0 || (metadata->rhs & (1 << rhs)) == 0) )
    {
        ++metadata;
    }
    return metadata->lhs != 0 ? metadata : NULL;
}

ValueStorage SemanticAnalyzer::storage_from_syntax_node( const SyntaxNode* node ) const
{
    REYES_ASSERT( node );

    ValueStorage storage = STORAGE_NULL;
    switch( node->node_type() )
    {    
        case SHADER_NODE_NULL:
            storage = STORAGE_NULL;
            break;
            
        case SHADER_NODE_UNIFORM:
            storage = STORAGE_UNIFORM;
            break;
            
        case SHADER_NODE_VARYING:
            storage = STORAGE_VARYING;
            break;
            
        default:
            REYES_ASSERT( false );
            storage = STORAGE_NULL;
            break;
    }
    return storage;
}

ValueType SemanticAnalyzer::type_from_syntax_node( const SyntaxNode* node ) const
{
    REYES_ASSERT( node );
    
    ValueType type = TYPE_NULL;
    switch ( node->node_type() )
    {
        case SHADER_NODE_FLOAT_TYPE:
            type = TYPE_FLOAT;
            break;
            
        case SHADER_NODE_STRING_TYPE:
            type = TYPE_STRING;
            break;
            
        case SHADER_NODE_COLOR_TYPE:
            type = TYPE_COLOR;
            break;
            
        case SHADER_NODE_POINT_TYPE:
            type = TYPE_POINT;
            break;
            
        case SHADER_NODE_VECTOR_TYPE:
            type = TYPE_VECTOR;
            break;
            
        case SHADER_NODE_NORMAL_TYPE:
            type = TYPE_NORMAL;
            break;
            
        case SHADER_NODE_MATRIX_TYPE:
            type = TYPE_MATRIX;
            break;
            
        default:
            REYES_ASSERT( false );
            type = TYPE_NULL;
            break;
    }
    return type;
}

ValueType SemanticAnalyzer::type_from_literal( const SyntaxNode* expression_node ) const
{
    REYES_ASSERT( expression_node );

    ValueType type = TYPE_NULL;
    switch ( expression_node->node_type() )
    {
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
            type = TYPE_FLOAT;
            break;
            
        case SHADER_NODE_STRING:
            type = TYPE_STRING;
            break;
            
        case SHADER_NODE_TRIPLE:
            type = TYPE_VECTOR;
            break;
            
        case SHADER_NODE_SIXTEENTUPLE:
            type = TYPE_MATRIX;
            break;
            
        case SHADER_NODE_IDENTIFIER:
            REYES_ASSERT( expression_node->symbol() );
            type = expression_node->symbol()->type();
            break;
            
        default:    
            REYES_ASSERT( false );
            type = TYPE_NULL;
            break;
    }
    return type;
}

shared_ptr<Symbol> SemanticAnalyzer::add_symbol( const std::string& identifier ) const
{
    REYES_ASSERT( symbol_table_ );
    return symbol_table_->add_symbol( identifier );
}

shared_ptr<Symbol> SemanticAnalyzer::find_symbol( const std::string& identifier ) const
{
    REYES_ASSERT( symbol_table_ );
    return symbol_table_->find_symbol( identifier );
}

Scope* SemanticAnalyzer::push_scope() const
{
    REYES_ASSERT( symbol_table_ );
    return symbol_table_->push_scope();
}

Scope* SemanticAnalyzer::pop_scope() const
{
    REYES_ASSERT( symbol_table_ );
    return symbol_table_->pop_scope();
}
