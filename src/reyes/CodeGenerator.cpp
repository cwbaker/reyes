//
// CodeGenerator.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "CodeGenerator.hpp"
#include "Encoder.hpp"
#include "SyntaxNode.hpp"
#include "Symbol.hpp"
#include "Value.hpp"
#include "Instruction.hpp"
#include "SymbolTable.hpp"
#include "ErrorCode.hpp"
#include "ErrorPolicy.hpp"
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include "assert.hpp"
#include <algorithm>
#include <stdio.h>

using std::min;
using std::max;
using std::find;
using std::vector;
using std::string;
using std::shared_ptr;
using namespace math;
using namespace reyes;

CodeGenerator::Jump::Jump( int address, int distance_address )
: address_( address ),
  distance_address_( distance_address )
{
    REYES_ASSERT( address_ >= 0 );
    REYES_ASSERT( distance_address_ >= 0 );
}

CodeGenerator::Loop::Loop( int begin )
: begin_( begin ),
  jumps_to_begin_(),
  jumps_to_continue_(),
  jumps_to_end_()
{
    const unsigned int JUMPS_TO_BEGIN_RESERVE = 4;
    jumps_to_begin_.reserve( JUMPS_TO_BEGIN_RESERVE );

    const unsigned int JUMPS_TO_CONTINUE_RESERVE = 4;
    jumps_to_continue_.reserve( JUMPS_TO_CONTINUE_RESERVE );

    const unsigned int JUMPS_TO_END_RESERVE = 4;
    jumps_to_end_.reserve( JUMPS_TO_END_RESERVE );
}

CodeGenerator::CodeGenerator( const SymbolTable& symbol_table, ErrorPolicy* error_policy )
: symbol_table_( symbol_table ),
  error_policy_( error_policy ),
  initialize_address_( 0 ),
  shade_address_( 0 ),
  permanent_registers_( 0 ),
  parameters_( 0 ),
  variables_( 0 ),
  constants_( 0 ),
  errors_( 0 ),
  symbols_(),
  values_(),
  temporary_registers_(),
  loops_(),
  index_( 0 ),
  registers_( 0 ),
  encoder_( nullptr )
{
    encoder_ = new Encoder;
    (void) symbol_table_;
    
    const unsigned int TEMPORARY_REGISTERS_RESERVE = 32;
    temporary_registers_.reserve( TEMPORARY_REGISTERS_RESERVE );

    const unsigned int LOOPS_RESERVE = 8;
    loops_.reserve( LOOPS_RESERVE );
}

CodeGenerator::~CodeGenerator()
{
    delete encoder_;
}

void CodeGenerator::generate( SyntaxNode* node, const char* name )
{
    REYES_ASSERT( name );
    
    permanent_registers_ = 0;
    parameters_ = 0;
    variables_ = 0;
    constants_ = 0;
    symbols_.clear();
    values_.clear();
    temporary_registers_.clear();
    loops_.clear();
    index_ = 0;
    registers_ = 0;
    encoder_->clear();

    if ( node && error_policy_->total_errors() == 0 )
    {
        generate_symbols_for_parameters( node->node(0)->node(0) );
        parameters_ = int(symbols_.size());

        generate_symbols( *node->node(0) );
        variables_ = int(symbols_.size()) - parameters_;

        generate_constants( node->node(0) );
        constants_ = int(values_.size());

        generate_indexes_for_symbols();

        initialize_address_ = encoder_->size();
        generate_code_for_list( *node->node(0)->node(0) );
        instruction( INSTRUCTION_HALT );

        shade_address_ = encoder_->size();
        generate_code_for_list( *node->node(0)->node(1) );
        instruction( INSTRUCTION_HALT );

        if ( errors_ > 0 )
        {        
            generate_code_in_case_of_errors();            
            if ( error_policy_ )
            {
                error_policy_->error( RENDER_ERROR_CODE_GENERATION_FAILED, "Generating code for shader '%s' failed", name );
            }
        }
    }
    else
    {
        generate_code_in_case_of_errors();        
    }
}

shared_ptr<Symbol> CodeGenerator::find_symbol( const std::string& identifier ) const
{
    vector<shared_ptr<Symbol> >::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    return i != symbols_.end() ? *i : shared_ptr<Symbol>();
}

shared_ptr<Symbol> CodeGenerator::get_symbol( int index ) const
{
    REYES_ASSERT( index < int(symbols_.size()) );
    return symbols_[index];
}

int CodeGenerator::initialize_address() const
{
    return initialize_address_;
}

int CodeGenerator::shade_address() const
{
    return shade_address_;
}

int CodeGenerator::permanent_registers() const
{
    return permanent_registers_;
}

int CodeGenerator::parameters() const
{
    return parameters_;
}

int CodeGenerator::variables() const
{
    return variables_;
}

int CodeGenerator::constants() const
{
    return constants_;
}

std::vector<shared_ptr<Symbol> >& CodeGenerator::symbols()
{
    return symbols_;
}

const std::vector<shared_ptr<Symbol> >& CodeGenerator::symbols() const
{
    return symbols_;
}

std::vector<shared_ptr<Value> >& CodeGenerator::values()
{
    return values_;
}

const std::vector<shared_ptr<Value> >& CodeGenerator::values() const
{
    return values_;
}

// std::vector<unsigned char>& CodeGenerator::code()
// {
//     return encoder_->code();
// }

const std::vector<unsigned char>& CodeGenerator::code() const
{
    return encoder_->code();
}

int CodeGenerator::registers() const
{
    return registers_;
}

void CodeGenerator::error( bool condition, int /*line*/, const char* format, ... )
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
            
            error_policy_->error( RENDER_ERROR_CODE_GENERATION_ERROR, "%s", message );
        }
    }
}

void CodeGenerator::generate_code_in_case_of_errors()
{
    permanent_registers_ = 0;
    parameters_ = 0;
    variables_ = 0;
    constants_ = 0;
    symbols_.clear();
    values_.clear();
    temporary_registers_.clear();
    loops_.clear();
    index_ = 0;
    registers_ = 0;
    encoder_->clear();

    initialize_address_ = encoder_->size();
    instruction( INSTRUCTION_HALT );

    shade_address_ = encoder_->size();
    instruction( INSTRUCTION_HALT );        
}

void CodeGenerator::generate_symbols( const SyntaxNode& node )
{
    shared_ptr<Symbol> symbol = node.symbol();
    if ( symbol && symbol->storage() != STORAGE_CONSTANT && find(symbols_.begin(), symbols_.end(), symbol) == symbols_.end() )
    {
        symbols_.push_back( symbol );
    }
    
    const vector<shared_ptr<SyntaxNode> >& nodes = node.nodes();
    for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        const SyntaxNode& node = *(*i);
        generate_symbols( node );
    }
}

void CodeGenerator::generate_symbols_for_parameters( SyntaxNode* list_node )
{
    REYES_ASSERT( list_node );
    REYES_ASSERT( list_node->node_type() == SHADER_NODE_LIST );
    
    const vector<shared_ptr<SyntaxNode> >& parameters = list_node->nodes();
    for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = parameters.begin(); i != parameters.end(); ++i )
    {
        const SyntaxNode* parameter_node = i->get();
        REYES_ASSERT( parameter_node );
        REYES_ASSERT( parameter_node->node_type() == SHADER_NODE_VARIABLE );
        
        shared_ptr<Symbol> symbol = parameter_node->symbol();
        REYES_ASSERT( symbol );
        REYES_ASSERT( find(symbols_.begin(), symbols_.end(), symbol) == symbols_.end() );
        symbols_.push_back( symbol );
    }
}

shared_ptr<Value> CodeGenerator::generate_constants( SyntaxNode* node )
{
    REYES_ASSERT( node );
    
    shared_ptr<Value> value;
    
    switch ( node->node_type() )
    {
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        case SHADER_NODE_STRING:
        case SHADER_NODE_TRIPLE:
        case SHADER_NODE_SIXTEENTUPLE:
            if ( node->constant_index() == SyntaxNode::REGISTER_NULL )
            {
                node->set_constant_index( int(values_.size()) );
                value.reset( new Value(node->original_type(), STORAGE_UNIFORM, 1) );
                values_.push_back( value );
                evaluate_expression( value, node );
            }
            break;
            
        case SHADER_NODE_IDENTIFIER:
            if ( node->symbol()->storage() == STORAGE_CONSTANT && node->constant_index() == SyntaxNode::REGISTER_NULL )
            {
                node->set_constant_index( int(values_.size()) );
                value.reset( new Value(TYPE_FLOAT, STORAGE_CONSTANT, 1) );
                values_.push_back( value );
                *value = node->symbol()->value();
            }
            break;
        
        default:
        {
            const vector<shared_ptr<SyntaxNode> >& nodes = node->nodes();
            for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
            {
                SyntaxNode* syntax_node = i->get();
                REYES_ASSERT( syntax_node );
                generate_constants( syntax_node );
            }
            break;
        }
    }
    
    return value;
}

void CodeGenerator::generate_indexes_for_symbols()
{
    int index = 0;
    for ( vector<shared_ptr<Symbol> >::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        symbol->set_index( index );
        ++index;
    }

    int register_index = constants_;
    vector<shared_ptr<Symbol> >::const_iterator i = symbols_.begin(); 
    
    while ( i != symbols_.end() && register_index < constants_ + parameters_ )
    {
        Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        symbol->set_register_index( register_index  );
        ++register_index ;
        ++i;
    }
    REYES_ASSERT( register_index == constants_ + parameters_ );
    
    while ( i != symbols_.end() )
    {
        Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        symbol->set_register_index( register_index );
        ++register_index;
        ++i;
    }
    REYES_ASSERT( register_index == parameters_ + constants_ + variables_ );
    
    index_ = register_index;
    registers_ = register_index;
    permanent_registers_ = register_index;
}

void CodeGenerator::evaluate_expression( shared_ptr<Value> value, const SyntaxNode* node ) const
{
    REYES_ASSERT( value );
    REYES_ASSERT( node );

    switch ( node->node_type() )
    {
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        {
            float* values = value->float_values();
            values[0] = node->real();
            break;
        }
        
        case SHADER_NODE_STRING:
            value->set_string( node->string() );
            break;
            
        case SHADER_NODE_TRIPLE:
        {
            vec3* values = value->vec3_values();
            values[0] = node->vec3();
            break;
        }
        
        case SHADER_NODE_SIXTEENTUPLE:
        {
            mat4x4* values = value->mat4x4_values();
            values[0] = node->mat4x4();
            break;
        }

        case SHADER_NODE_TYPECAST:
            evaluate_expression( value, node->node(1) );
            break; 
           
        default:
        {
            REYES_ASSERT( false );
            float* values = value->float_values();
            values[0] = 1.0f;
            break;
        }
    }
}

int CodeGenerator::assign_instruction_from_type( int instruction, ValueType type ) const
{
    REYES_ASSERT( type > TYPE_NULL && type < TYPE_COUNT );
    const int INSTRUCTION_OFFSET_BY_TYPE[] =
    {
        0, // NULL
        0, // INTEGER
        0, // FLOAT
        0, // COLOR
        0, // POINT
        0, // VECTOR
        0, // NORMAL
        0, // MATRIX
        1 // STRING
    };
    return instruction + INSTRUCTION_OFFSET_BY_TYPE[type];
}

int CodeGenerator::arithmetic_instruction_from_type( int instruction, ValueType type ) const
{
    REYES_ASSERT( type > TYPE_NULL && type < TYPE_COUNT );
    const int INSTRUCTION_OFFSET_BY_TYPE[] =
    {
        0, // NULL
        0, // INTEGER
        0, // FLOAT
        1, // COLOR
        1, // POINT
        1, // VECTOR
        1, // NORMAL
        2, // MATRIX
        0 // STRING
    };
    return instruction + INSTRUCTION_OFFSET_BY_TYPE[type];
}

int CodeGenerator::promote_instruction_from_type( int instruction, ValueType type ) const
{
    REYES_ASSERT( type > TYPE_NULL && type < TYPE_COUNT );
    const int INSTRUCTION_OFFSET_BY_TYPE[] =
    {
        0, // NULL
        0, // INTEGER
        1, // FLOAT
        2, // COLOR
        2, // POINT
        2, // VECTOR
        2, // NORMAL
        3, // MATRIX
        0 // STRING
    };
    return instruction + INSTRUCTION_OFFSET_BY_TYPE[type];
}

int CodeGenerator::generate_binary_expression( const SyntaxNode* expression_node )
{
    REYES_ASSERT( expression_node );
    REYES_ASSERT( expression_node->instruction() != INSTRUCTION_NULL );
    REYES_ASSERT( expression_node->node(0) );
    REYES_ASSERT( expression_node->node(1) );
    
    const SyntaxNode* node = expression_node->node(0);
    const SyntaxNode* other_node = expression_node->node(1);
    int arg0 = generate_expression( *node );
    int arg1 = generate_expression( *other_node );
    CodeGenerator::instruction( 
        expression_node->instruction(),
        node->type(), node->storage(),
        other_node->type(), other_node->storage()
    );
    argument( arg0 );
    argument( arg1 );
    return allocate_register();
}

int CodeGenerator::generate_code_for_assign_expression( int instruction, const SyntaxNode& node )
{
    REYES_ASSERT( node.symbol() );

    const SyntaxNode* expression_node = node.node( 0 );
    int arg1 = generate_expression( *expression_node );
    const Symbol* symbol = node.symbol().get();
    CodeGenerator::instruction( 
        assign_instruction_from_type(instruction, node.symbol()->type()),
        symbol->type(), symbol->storage(),
        expression_node->type(), expression_node->storage()
    );
    argument( symbol->register_index() );
    argument( arg1 );
    return arg1;
}

int CodeGenerator::generate_type_conversion( int register_index, const SyntaxNode& node )
{
    REYES_ASSERT( register_index >= 0 );
    
    if ( node.original_type() != TYPE_NULL && node.original_type() != node.type() )
    {
        REYES_ASSERT( node.original_type() == TYPE_FLOAT );
        REYES_ASSERT( node.type() != TYPE_FLOAT );
        
        ValueType to_type = node.type();
        if ( node.type() != node.original_type() )
        {
            instruction(
                INSTRUCTION_CONVERT,
                node.type(), node.storage(),
                node.original_type(), node.storage()
            );
            argument( register_index );
            register_index = allocate_register();
        }
    }
    return register_index;
}

int CodeGenerator::generate_storage_promotion( int register_index, const SyntaxNode& node )
{
    if ( node.original_storage() != STORAGE_NULL )
    {
        REYES_ASSERT( node.storage() == STORAGE_VARYING );
        instruction( 
            INSTRUCTION_PROMOTE,
            node.type(), node.storage(),
            node.type(), node.original_storage()
        );
        argument( register_index );
        register_index = allocate_register();
    }
    return register_index;
}

void CodeGenerator::generate_code_for_list( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_LIST );
    
    const vector<shared_ptr<SyntaxNode> >& nodes = node.nodes();
    for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        const SyntaxNode& node = *(*i);
        push_register();
        generate_statement( node );
        pop_register();
    }
}

void CodeGenerator::generate_code_for_variable( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_VARIABLE );

    if ( node.node(0)->node_type() != SHADER_NODE_NULL )
    {
        const shared_ptr<Symbol>& symbol = node.symbol();
        const SyntaxNode* expression_node = node.node( 0 );
        int arg0 = symbol->register_index();
        int arg1 = generate_expression( *expression_node );
        instruction( 
            assign_instruction_from_type(INSTRUCTION_ASSIGN, symbol->type()),
            symbol->type(), symbol->storage(),
            expression_node->type(), expression_node->storage()
        );
        argument( arg0 );
        argument( arg1 );
    }
}

void CodeGenerator::generate_code_for_parameters( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_LIST );
    
    const vector<shared_ptr<SyntaxNode> >& parameters = node.nodes();
    for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = parameters.begin(); i != parameters.end(); ++i )
    {
        const SyntaxNode* variable_node = i->get();
        REYES_ASSERT( variable_node );
        REYES_ASSERT( variable_node->node_type() == SHADER_NODE_VARIABLE );
        if ( variable_node->node(0)->node_type() == SHADER_NODE_TYPECAST )
        {
            int register_index = generate_typecast_expression( *variable_node->node(0) );
            variable_node->symbol()->set_register_index( register_index );
            permanent_registers_ = max( permanent_registers_, register_index );
        }
    }
}

void CodeGenerator::generate_statement( const SyntaxNode& node )
{
    switch ( node.node_type() )
    {
        case SHADER_NODE_LIST:
            generate_code_for_list( node );
            break;
            
        case SHADER_NODE_VARIABLE:
            generate_code_for_variable( node );
            break;
            
        case SHADER_NODE_CALL:
            generate_call_expression( node );
            break;
            
        case SHADER_NODE_RETURN:
            generate_return_statement( node );
            break;
            
        case SHADER_NODE_BREAK:
            generate_break_statement( node );
            break;
            
        case SHADER_NODE_CONTINUE:
            generate_continue_statement( node );
            break;
            
        case SHADER_NODE_IF:
            generate_if_statement( node );
            break;
            
        case SHADER_NODE_IF_ELSE:
            generate_if_else_statement( node );
            break;
            
        case SHADER_NODE_WHILE:
            generate_while_statement( node );
            break;
            
        case SHADER_NODE_FOR:
            generate_for_statement( node );
            break;
            
        case SHADER_NODE_AMBIENT:
            generate_ambient_statement( node );
            break;
            
        case SHADER_NODE_SOLAR:
            generate_solar_statement( node );
            break;
            
        case SHADER_NODE_ILLUMINATE:
            generate_illuminate_statement( node );
            break;
            
        case SHADER_NODE_ILLUMINANCE:
            generate_illuminance_statement( node );
            break;
            
        case SHADER_NODE_ASSIGN:    
            generate_code_for_assign_expression( INSTRUCTION_ASSIGN, node );
            break;
            
        case SHADER_NODE_ADD_ASSIGN:
            generate_code_for_assign_expression( INSTRUCTION_ADD_ASSIGN, node );
            break;
            
        case SHADER_NODE_SUBTRACT_ASSIGN:
            generate_code_for_assign_expression( INSTRUCTION_SUBTRACT_ASSIGN, node );
            break;
            
        case SHADER_NODE_MULTIPLY_ASSIGN:
            generate_code_for_assign_expression( INSTRUCTION_MULTIPLY_ASSIGN, node );
            break;
            
        case SHADER_NODE_DIVIDE_ASSIGN:
            generate_code_for_assign_expression( INSTRUCTION_DIVIDE_ASSIGN, node );
            break;
            
        default:
            REYES_ASSERT( false );
            break;
    }
}

void CodeGenerator::generate_if_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_IF );
    
    const SyntaxNode* expression_node = node.node( 0 );
    int mask = generate_expression( *expression_node );
    push_register();
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression_node->storage() );
    argument( mask );

    const SyntaxNode* statement_node = node.node( 1 );
    generate_statement( *statement_node );
    instruction( INSTRUCTION_CLEAR_MASK );
    pop_register();
}

void CodeGenerator::generate_if_else_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_IF_ELSE );
    
    const SyntaxNode* expression_node = node.node( 0 );
    int mask = generate_expression( *expression_node );
    push_register();
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression_node->storage() );
    argument( mask );

    const SyntaxNode* statement_node = node.node( 1 );
    generate_statement( *statement_node );
    instruction( INSTRUCTION_INVERT_MASK );

    const SyntaxNode* else_statement_node = node.node( 2 );
    generate_statement( *else_statement_node );
    instruction( INSTRUCTION_CLEAR_MASK );
    pop_register();
}

void CodeGenerator::generate_while_statement( const SyntaxNode& while_node )
{
    REYES_ASSERT( while_node.node_type() == SHADER_NODE_WHILE );

    push_loop();
    mark_loop_continue();
    push_register();

    const SyntaxNode* expression_node = while_node.node( 0 );
    int mask = generate_expression( *expression_node );
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression_node->storage() );
    argument( mask );
    pop_register();
    jump_to_end( INSTRUCTION_JUMP_EMPTY, 1 );
    push_register();
    const SyntaxNode* statement_node = while_node.node( 1 );
    generate_statement( *statement_node );
    pop_register();
    instruction( INSTRUCTION_CLEAR_MASK );
    jump_to_begin( INSTRUCTION_JUMP, 1 );
    pop_loop();
}

void CodeGenerator::generate_for_statement( const SyntaxNode& for_node )
{
    REYES_ASSERT( for_node.node_type() == SHADER_NODE_FOR );

    push_register();
    const SyntaxNode* initialize_statement_node = for_node.node( 0 );
    generate_statement( *initialize_statement_node );
    pop_register();
    push_loop();
    
    push_register();
    const SyntaxNode* expression_node = for_node.node( 1 );
    int mask = generate_expression( *expression_node );
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression_node->storage() );
    argument( mask );
    pop_register();
    jump_to_end( INSTRUCTION_JUMP_EMPTY, 1 );
    push_register();

    const SyntaxNode* statement_node = for_node.node( 3 );
    generate_statement( *statement_node );
    pop_register();
    mark_loop_continue();
    push_register();

    const SyntaxNode* increment_statement_node = for_node.node( 2 );
    generate_statement( *increment_statement_node );
    pop_register();
    instruction( INSTRUCTION_CLEAR_MASK );
    jump_to_begin( INSTRUCTION_JUMP, 1 );
    pop_loop();
}

void CodeGenerator::generate_ambient_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_AMBIENT );

    int light_color = generate_expression( *node.node(0) );
    int light_opacity = generate_expression( *node.node(1) );
    instruction( INSTRUCTION_AMBIENT );
    argument( light_color );
    argument( light_opacity );
}

void CodeGenerator::generate_solar_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_SOLAR );

    const SyntaxNode* expressions_node = node.node(0);
    if ( expressions_node->nodes().size() == 0 )
    {
        instruction( INSTRUCTION_SOLAR );
    }
    else
    {
        int axis = generate_expression( *expressions_node->node(0) );
        int angle = generate_expression( *expressions_node->node(1) );
        int light_color = generate_expression( *node.node(2) );
        int light_opacity = generate_expression( *node.node(3) );
        instruction( INSTRUCTION_SOLAR_AXIS_ANGLE );
        argument( axis );
        argument( angle );
        argument( light_color );
        argument( light_opacity );
    }
    
    generate_statement( *node.node(1) );
}

void CodeGenerator::generate_illuminate_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ILLUMINATE );

    const SyntaxNode* expressions_node = node.node(0);
    if ( expressions_node->nodes().size() == 1 )
    {
        int position = generate_expression( *expressions_node->node(0) );
        int surface_position = generate_expression( *node.node(2) );
        int light_direction = generate_expression( *node.node(3) );
        int light_color = generate_expression( *node.node(4) );
        int light_opacity = generate_expression( *node.node(5) );
        instruction( INSTRUCTION_ILLUMINATE );
        argument( position );
        argument( surface_position );
        argument( light_direction );
        argument( light_color );
        argument( light_opacity );
        generate_statement( *node.node(1) );
    }
    else
    {
        int position = generate_expression( *expressions_node->node(0) );
        int axis = generate_expression( *expressions_node->node(1) );
        int angle = generate_expression( *expressions_node->node(2) );
        int surface_position = generate_expression( *node.node(2) );
        int light_direction = generate_expression( *node.node(3) );
        int light_color = generate_expression( *node.node(4) );
        int light_opacity = generate_expression( *node.node(5) );
        instruction( INSTRUCTION_ILLUMINATE_AXIS_ANGLE );
        argument( position );
        argument( axis );
        argument( angle );
        argument( surface_position );
        argument( light_direction );
        argument( light_color );
        argument( light_opacity );
        generate_statement( *node.node(1) );
    }
}

void CodeGenerator::generate_illuminance_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ILLUMINANCE );

    const SyntaxNode* expressions_node = node.node(0);
    if ( expressions_node->nodes().size() == 1 )
    {
        /*
        int arg0 = generate_expression( *expressions_node->node(0) );
        instruction( INSTRUCTION_ILLUMINANCE );
        argument( arg0 );
        generate_statement( *node.node(1) );
        */
        REYES_ASSERT( false );
    }
    else
    {
        push_loop();
        jump_to_end( INSTRUCTION_JUMP_ILLUMINANCE, 1 );
    
        push_register();
        int position = generate_expression( *expressions_node->node(0) );
        int axis = generate_expression( *expressions_node->node(1) );
        int angle = generate_expression( *expressions_node->node(2) );
        int light_direction = generate_expression( *node.node(2) );
        int light_color = generate_expression( *node.node(3) );
        int light_opacity = generate_expression( *node.node(4) );

        instruction( INSTRUCTION_ILLUMINANCE_AXIS_ANGLE );
        argument( position );
        argument( axis );
        argument( angle );
        argument( light_direction );
        argument( light_color );
        argument( light_opacity );
        instruction( INSTRUCTION_GENERATE_MASK );
        argument( allocate_register() );
        pop_register();
        
        push_register();
        generate_statement( *node.node(1) );
        pop_register();

        instruction( INSTRUCTION_CLEAR_MASK );        
        jump_to_begin( INSTRUCTION_JUMP, 1 );
        pop_loop();
    }
}

void CodeGenerator::generate_break_statement( const SyntaxNode& break_node )
{
    REYES_ASSERT( break_node.node_type() == SHADER_NODE_BREAK );

    int level = 1;
    if ( !break_node.nodes().empty() )
    {
        REYES_ASSERT( break_node.node(0) && break_node.node(0)->node_type() == SHADER_NODE_INTEGER );
        level = ::atoi( break_node.node(0)->lexeme().c_str() );
    }

    error( loops_.empty(), break_node.line(), "Break outside of a loop" );
    error( level <= 0, break_node.line(), "Break to a negative level" );
    error( level > int(loops_.size()) && !loops_.empty(), break_node.line(), "Break to a level outside of a loop" );

    if ( level > 0 && level <= int(loops_.size()) )
    {
        jump_to_end( INSTRUCTION_JUMP, level );
    }
}

void CodeGenerator::generate_continue_statement( const SyntaxNode& continue_node )
{
    REYES_ASSERT( continue_node.node_type() == SHADER_NODE_CONTINUE );

    int level = 1;
    if ( !continue_node.nodes().empty() )
    {
        REYES_ASSERT( continue_node.node(0) && continue_node.node(0)->node_type() == SHADER_NODE_INTEGER );
        level = ::atoi( continue_node.node(0)->lexeme().c_str() );
    }

    error( loops_.empty(), continue_node.line(), "Continue outside of a loop" );
    error( level <= 0, continue_node.line(), "Continue to a negative level" );
    error( level > int(loops_.size()) && !loops_.empty(), continue_node.line(), "Continue to a level outside of a loop" );

    if ( level > 0 && level <= int(loops_.size()) )
    {
        jump_to_continue( INSTRUCTION_JUMP, level );
    }
}

void CodeGenerator::generate_return_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_RETURN );
}

int CodeGenerator::generate_expression( const SyntaxNode& node )
{
    int index = 0;    
    switch ( node.node_type() )
    {
        case SHADER_NODE_CALL:
            index = generate_call_expression( node );
            break;
            
        case SHADER_NODE_CROSS:
            REYES_ASSERT( false );
            break;
            
        case SHADER_NODE_DOT:
        case SHADER_NODE_MULTIPLY:
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
            index = generate_binary_expression( &node );
            break;
                        
        case SHADER_NODE_DIVIDE:
            index = generate_divide_expression( node );
            break;

        case SHADER_NODE_NEGATE:
            index = generate_negate_expression( node );
            break;
            
        case SHADER_NODE_TERNARY:
            index = generate_ternary_expression( node );
            break;
            
        case SHADER_NODE_TYPECAST:
            index = generate_typecast_expression( node );
            break;
            
        case SHADER_NODE_ASSIGN:    
            index = generate_code_for_assign_expression( INSTRUCTION_ASSIGN, node );
            break;
            
        case SHADER_NODE_ADD_ASSIGN:
            index = generate_code_for_assign_expression( INSTRUCTION_ADD_ASSIGN, node );
            break;
            
        case SHADER_NODE_SUBTRACT_ASSIGN:
            index = generate_code_for_assign_expression( INSTRUCTION_SUBTRACT_ASSIGN, node );
            break;
            
        case SHADER_NODE_MULTIPLY_ASSIGN:
            index = generate_code_for_assign_expression( INSTRUCTION_MULTIPLY_ASSIGN, node );
            break;
            
        case SHADER_NODE_DIVIDE_ASSIGN:
            index = generate_code_for_assign_expression( INSTRUCTION_DIVIDE_ASSIGN, node );
            break;
            
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        case SHADER_NODE_STRING:
        case SHADER_NODE_TRIPLE:
        case SHADER_NODE_SIXTEENTUPLE:
            index = generate_constant_expression( node );
            break;

        case SHADER_NODE_TEXTURE:
            index = generate_vec3_texture_expression( node );
            break;
            
        case SHADER_NODE_SHADOW:
            index = generate_shadow_expression( node );
            break;
            
        case SHADER_NODE_IDENTIFIER:
            index = generate_identifier_expression( node );
            break;
            
        default:
            REYES_ASSERT( false );
            break;
    }    
    index = generate_type_conversion( index, node );
    index = generate_storage_promotion( index, node );
    return index;
}

int CodeGenerator::generate_call_expression( const SyntaxNode& call_node )
{
    REYES_ASSERT( call_node.node_type() == SHADER_NODE_CALL );
       
    const int MAXIMUM_ARGUMENTS = 256;
    int arguments [MAXIMUM_ARGUMENTS] = { 0 };
    REYES_ASSERT( int(call_node.nodes().size()) < MAXIMUM_ARGUMENTS );
    for ( int i = 0; i < int(call_node.nodes().size()); ++i )
    {
        const SyntaxNode* node = call_node.node( i );
        int argument = generate_expression( *node );
        arguments[i] = argument;
    }
    
    instruction( INSTRUCTION_CALL_0 + call_node.nodes().size() );
    argument( call_node.symbol()->index() );
    for ( int i = 0; i < int(call_node.nodes().size()); ++i )
    {
        argument( arguments[i] );
    }    

    return allocate_register();
}

int CodeGenerator::generate_divide_expression( const SyntaxNode& divide_node )
{
    REYES_ASSERT( divide_node.node(0) );
    REYES_ASSERT( divide_node.node(1) );
    
    const SyntaxNode& node = *divide_node.node(0);
    const SyntaxNode& other_node = *divide_node.node(1);
    
    int arg0 = generate_expression( node );
    int arg1 = generate_expression( other_node );
    instruction( 
        INSTRUCTION_DIVIDE,
        node.type(), node.storage(),
        other_node.type(), other_node.storage()
    );
    argument( arg0 );
    argument( arg1 );
    return allocate_register();
}

int CodeGenerator::generate_negate_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_NEGATE );
    int arg0 = generate_expression( *node.node(0) ); 
    instruction( INSTRUCTION_NEGATE, node.type(), node.storage() );
    argument( arg0 );
    return allocate_register();
}

int CodeGenerator::generate_ternary_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TERNARY );
    return index_;
}

int CodeGenerator::generate_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    
    int index = index_;
    if ( node.node(1)->node_type() == SHADER_NODE_TEXTURE )
    {
        index = generate_texture_typecast_expression( node );
    }
    else if ( node.node(1)->node_type() == SHADER_NODE_ENVIRONMENT )
    {
        index = generate_environment_typecast_expression( node );
    }
    else if ( node.node(1)->type() == TYPE_COLOR || node.node(1)->type() == TYPE_POINT || node.node(1)->type() == TYPE_VECTOR || node.node(1)->type() == TYPE_NORMAL )
    {
        index = generate_vec3_typecast_expression( node );
    }
    else if ( node.node(1)->type() == TYPE_MATRIX )
    {
        index = generate_mat4x4_typecast_expression( node );
    }
    else
    {
        index = generate_expression( *node.node(1) );
    }
    return index;
}

int CodeGenerator::generate_vec3_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    
    int index = SyntaxNode::REGISTER_NULL;
    if ( !node.node(0)->nodes().empty() )
    {
        const SyntaxNode* space_expression = node.node( 0 )->node( 0 );
        int arg0 = generate_expression( *space_expression );

        const SyntaxNode* value_expression = node.node( 1 );
        int arg1 = generate_expression( *value_expression );

        switch ( node.node(0)->node_type() )
        {
            case SHADER_NODE_POINT_TYPE:
                instruction( INSTRUCTION_TRANSFORM_POINT, value_expression->type(), value_expression->storage() );
                break;
            
            case SHADER_NODE_VECTOR_TYPE:
                instruction( INSTRUCTION_TRANSFORM_VECTOR, value_expression->type(), value_expression->storage() );
                break;

            case SHADER_NODE_NORMAL_TYPE:
                instruction( INSTRUCTION_TRANSFORM_NORMAL, value_expression->type(), value_expression->storage() );
                break;
                
            case SHADER_NODE_COLOR_TYPE:
                instruction( INSTRUCTION_TRANSFORM_COLOR );
                break;

            default:
                REYES_ASSERT( false );
                instruction( INSTRUCTION_HALT );
                break;            
        }

        argument( arg0 );
        argument( arg1 );
        index = allocate_register();
    }
    else
    {
        index = generate_expression( *node.node(1) );
    }
    return index;
}

int CodeGenerator::generate_mat4x4_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    
    int index = SyntaxNode::REGISTER_NULL;
    if ( !node.node(0)->nodes().empty() )
    {
        REYES_ASSERT( node.node(0)->node_type() == SHADER_NODE_MATRIX_TYPE );
        const SyntaxNode* tospace_node = node.node( 0 )->node( 0 );
        int arg0 = generate_expression( *tospace_node );
        const SyntaxNode* value_node = node.node( 1 );
        int arg1 = generate_expression( *value_node );
        instruction( INSTRUCTION_TRANSFORM_MATRIX, value_node->type(), value_node->storage() );
        argument( arg0 );
        argument( arg1 );
        index = allocate_register();
    }
    else
    {
        index = generate_expression( *node.node(1) );
    }
    return index;
}

int CodeGenerator::generate_texture_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    REYES_ASSERT( node.node(1)->node_type() == SHADER_NODE_TEXTURE );
    
    int index = index_;
    if ( node.node(1)->node_type() == SHADER_NODE_TEXTURE )
    {        
        switch ( node.node(0)->node_type() )
        {
            case SHADER_NODE_FLOAT_TYPE:
                index = generate_float_texture_expression( *node.node(1) );
                break;

            case SHADER_NODE_COLOR_TYPE:
            case SHADER_NODE_POINT_TYPE:
            case SHADER_NODE_VECTOR_TYPE:
            case SHADER_NODE_NORMAL_TYPE:
                index = generate_vec3_texture_expression( *node.node(1) );
                break;
                
            default:
                REYES_ASSERT( false );
                index = index_;
                break;
        }
    }    
    return index;
}

int CodeGenerator::generate_environment_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    REYES_ASSERT( node.node(1)->node_type() == SHADER_NODE_ENVIRONMENT );
    
    int index = index_;
    if ( node.node(1)->node_type() == SHADER_NODE_ENVIRONMENT )
    {        
        switch ( node.node(0)->node_type() )
        {
            case SHADER_NODE_FLOAT_TYPE:
                index = generate_float_environment_expression( *node.node(1) );
                break;

            case SHADER_NODE_COLOR_TYPE:
            case SHADER_NODE_POINT_TYPE:
            case SHADER_NODE_VECTOR_TYPE:
            case SHADER_NODE_NORMAL_TYPE:
                index = generate_vec3_environment_expression( *node.node(1) );
                break;
                
            default:
                REYES_ASSERT( false );
                index = index_;
                break;
        }
    }    
    return index;
}

int CodeGenerator::generate_shadow_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_SHADOW );

    int arg0 = generate_expression( *node.node(0) );
    int arg1 = generate_expression( *node.node(1) );
    int arg2 = generate_expression( *node.node(2) );
    instruction( INSTRUCTION_SHADOW );
    argument( arg0 );
    argument( arg1 );
    argument( arg2 );
    return allocate_register();    
}

int CodeGenerator::generate_float_texture_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TEXTURE );

    int arg0 = generate_expression( *node.node(0) );
    int arg1 = generate_expression( *node.node(1) );
    int arg2 = generate_expression( *node.node(2) );
    instruction( INSTRUCTION_FLOAT_TEXTURE );
    argument( arg0 );
    argument( arg1 );
    argument( arg2 );
    return allocate_register();
}

int CodeGenerator::generate_vec3_texture_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TEXTURE );

    int arg0 = generate_expression( *node.node(0) );
    int arg1 = generate_expression( *node.node(1) );
    int arg2 = generate_expression( *node.node(2) );
    instruction( INSTRUCTION_VEC3_TEXTURE );
    argument( arg0 );
    argument( arg1 );
    argument( arg2 );
    return allocate_register();
}

int CodeGenerator::generate_float_environment_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ENVIRONMENT );

    int arg0 = generate_expression( *node.node(0) );
    int arg1 = generate_expression( *node.node(1) );
    instruction( INSTRUCTION_FLOAT_ENVIRONMENT );
    argument( arg0 );
    argument( arg1 );
    return allocate_register();
}

int CodeGenerator::generate_vec3_environment_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ENVIRONMENT );

    int arg0 = generate_expression( *node.node(0) );
    int arg1 = generate_expression( *node.node(1) );
    instruction( INSTRUCTION_VEC3_ENVIRONMENT );
    argument( arg0 );
    argument( arg1 );
    return allocate_register();
}

int CodeGenerator::generate_constant_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.constant_index() >= 0 && node.constant_index() < int(values_.size()) );
    return node.constant_index();
}

int CodeGenerator::generate_identifier_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_IDENTIFIER );
    REYES_ASSERT( node.symbol() );    
    return node.constant_index() == SyntaxNode::REGISTER_NULL ? node.symbol()->register_index() : node.constant_index();
}

void CodeGenerator::instruction( int instruction )
{
    encoder_->instruction( instruction );
}

void CodeGenerator::instruction( int instruction, int type, int storage )
{
    encoder_->instruction( instruction, type, storage );
}

void CodeGenerator::instruction( int instruction, int type, int storage, int other_type, int other_storage )
{
    encoder_->instruction( instruction, type, storage, other_type, other_storage );
}

void CodeGenerator::argument( int argument )
{
    encoder_->argument( argument );
}

void CodeGenerator::patch_argument( int address, int distance )
{
    encoder_->patch_argument( address, distance );
}

int CodeGenerator::argument_for_patching()
{
    return encoder_->argument_for_patching();
}

int CodeGenerator::address()
{
    return encoder_->address();
}

void CodeGenerator::push_loop()
{
    int begin = address();
    loops_.push_back( Loop(begin) );
}

void CodeGenerator::pop_loop()
{
    REYES_ASSERT( !loops_.empty() );
    const Loop& loop = loops_.back();
    int continue_ = loop.continue_;
    int end = address();
    
    const vector<Jump>& jumps_to_begin = loop.jumps_to_begin_;
    for ( vector<Jump>::const_iterator jump = jumps_to_begin.begin(); jump != jumps_to_begin.end(); ++jump )
    {
        patch_argument( jump->distance_address_, loop.begin_ - jump->address_ );
    }

    const vector<Jump>& jumps_to_continue = loop.jumps_to_continue_;
    for ( vector<Jump>::const_iterator jump = jumps_to_continue.begin(); jump != jumps_to_continue.end(); ++jump )
    {
        patch_argument( jump->distance_address_, continue_ - jump->address_ );
    }

    const vector<Jump>& jumps_to_end = loop.jumps_to_end_;
    for ( vector<Jump>::const_iterator jump = jumps_to_end.begin(); jump != jumps_to_end.end(); ++jump )
    {
        patch_argument( jump->distance_address_, end - jump->address_ );
    }

    loops_.pop_back();
}

void CodeGenerator::mark_loop_continue()
{
    REYES_ASSERT( !loops_.empty() );
    loops_.back().continue_ = address();
}

void CodeGenerator::jump_to_begin( int jump_instruction, int level )
{
    REYES_ASSERT( level > 0 );
    REYES_ASSERT( level <= int(loops_.size()) );

    instruction( jump_instruction );
    int jump_distance_address = argument_for_patching();
    int jump_address = address();
    Loop& loop = loops_[loops_.size() - level];
    loop.jumps_to_begin_.push_back( Jump(jump_address, jump_distance_address) );    
}

void CodeGenerator::jump_to_continue( int jump_instruction, int level )
{
    REYES_ASSERT( level > 0 );
    REYES_ASSERT( level <= int(loops_.size()) );

    instruction( jump_instruction );
    int jump_distance_address = argument_for_patching();
    int jump_address = address();
    Loop& loop = loops_[loops_.size() - level];
    loop.jumps_to_continue_.push_back( Jump(jump_address, jump_distance_address) );
}

void CodeGenerator::jump_to_end( int jump_instruction, int level )
{
    REYES_ASSERT( level > 0 );
    REYES_ASSERT( level <= int(loops_.size()) );
    
    instruction( jump_instruction );
    int jump_distance_address = argument_for_patching();
    int jump_address = address();
    Loop& loop = loops_[loops_.size() - level];
    loop.jumps_to_end_.push_back( Jump(jump_address, jump_distance_address) );
}

int CodeGenerator::allocate_register()
{
    int index = index_;
    ++index_;
    registers_ = max( registers_, index_ );
    return index;
}

void CodeGenerator::push_register()
{
    temporary_registers_.push_back( index_ );
}

void CodeGenerator::pop_register()
{
    REYES_ASSERT( !temporary_registers_.empty() );
    if ( index_ != temporary_registers_.back() )
    {
        index_ = temporary_registers_.back();
        instruction( INSTRUCTION_RESET );
        argument( index_ );
    }
    temporary_registers_.pop_back();
}
