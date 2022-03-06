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
#include "Scope.hpp"
#include "SymbolTable.hpp"
#include "ErrorCode.hpp"
#include "ErrorPolicy.hpp"
#include "Segment.hpp"
#include "assert.hpp"
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <string.h>

using std::min;
using std::max;
using std::find;
using std::vector;
using std::string;
using std::shared_ptr;
using std::copy_if;
using std::back_inserter;
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

CodeGenerator::CodeGenerator( ErrorPolicy* error_policy )
: error_policy_( error_policy )
, maximum_vertices_( 16 * 16 )
, initialize_address_( 0 )
, shade_address_( 0 )
, parameters_( 0 )
, variables_( 0 )
, grid_memory_size_( 0 )
, temporary_memory_size_( 0 )
, errors_( 0 )
, symbols_()
, values_()
, loops_()
, encoder_( nullptr )
, constant_data_()
, temporary_addresses_()
, offset_( 0 )
{
    REYES_ASSERT( symbol_table_ );

    encoder_ = new Encoder;
    
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
    
    parameters_ = 0;
    variables_ = 0;
    grid_memory_size_ = 0;
    temporary_memory_size_ = 0;
    symbols_.clear();
    values_.clear();
    loops_.clear();
    encoder_->clear();
    constant_data_.clear();
    temporary_addresses_.clear();
    offset_ = 0;

    if ( node && error_policy_->total_errors() == 0 )
    {
        generate_constants( node );
        generate_symbols( node );

        const SyntaxNode* list_node = node;
        const vector<shared_ptr<SyntaxNode>>& parameters = list_node->nodes();
        parameters_ = int(parameters.size());
        variables_ = int(symbols_.size()) - parameters_;

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

int CodeGenerator::parameters() const
{
    return parameters_;
}

int CodeGenerator::variables() const
{
    return variables_;
}

int CodeGenerator::maximum_vertices() const
{
    return maximum_vertices_;
}

int CodeGenerator::constant_memory_size() const
{
    return int(constant_data_.size());
}

int CodeGenerator::grid_memory_size() const
{
    return grid_memory_size_;
}

int CodeGenerator::temporary_memory_size() const
{
    return temporary_memory_size_;
}

std::vector<shared_ptr<Symbol>>& CodeGenerator::symbols()
{
    return symbols_;
}

const std::vector<shared_ptr<Symbol>>& CodeGenerator::symbols() const
{
    return symbols_;
}

std::vector<shared_ptr<Value>>& CodeGenerator::values()
{
    return values_;
}

const std::vector<shared_ptr<Value>>& CodeGenerator::values() const
{
    return values_;
}

std::vector<unsigned char>& CodeGenerator::constant_data()
{
    return constant_data_;
}

const std::vector<unsigned char>& CodeGenerator::constant_data() const
{
    return constant_data_;
}

const std::vector<unsigned char>& CodeGenerator::code() const
{
    return encoder_->code();
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
    parameters_ = 0;
    variables_ = 0;
    symbols_.clear();
    values_.clear();
    loops_.clear();
    encoder_->clear();

    initialize_address_ = encoder_->size();
    instruction( INSTRUCTION_HALT );

    shade_address_ = encoder_->size();
    instruction( INSTRUCTION_HALT );        
}

void CodeGenerator::generate_constants( SyntaxNode* node )
{
    REYES_ASSERT( node );
    
    switch ( node->node_type() )
    {
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        case SHADER_NODE_STRING:
        case SHADER_NODE_TRIPLE:
        case SHADER_NODE_SIXTEENTUPLE:
            evaluate_constant_expression( node );
            break;
            
        case SHADER_NODE_IDENTIFIER:
            if ( node->symbol()->storage() == STORAGE_CONSTANT )
            {
                Address address = write_constant_float( node->symbol()->value() );
                node->set_address( address );
            }
            break;
        
        default:
        {
            const vector<shared_ptr<SyntaxNode>>& nodes = node->nodes();
            for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
            {
                SyntaxNode* syntax_node = i->get();
                REYES_ASSERT( syntax_node );
                generate_constants( syntax_node );
            }
            break;
        }
    }
}

void CodeGenerator::generate_symbols( const SyntaxNode* node )
{
    REYES_ASSERT( node );

    Scope* grid_scope = node->node( 0 )->scope();
    REYES_ASSERT( grid_scope );
    int offset = grid_scope->enter( SEGMENT_GRID, 0, maximum_vertices_ );
    int string_offset = grid_scope->enter_strings( 0 );

    Scope* parameters_scope = node->node( 0 )->node( 0 )->scope();
    REYES_ASSERT( parameters_scope );
    offset = parameters_scope->enter( SEGMENT_GRID, offset, maximum_vertices_ );
    string_offset = parameters_scope->enter_strings( string_offset );

    Scope* global_scope = node->scope();
    REYES_ASSERT( global_scope );
    offset = global_scope->enter( SEGMENT_GRID, offset, maximum_vertices_ );
    string_offset = global_scope->enter_strings( string_offset );

    auto include_symbol = []( const shared_ptr<Symbol>& symbol )
    {
        return
            symbol->segment() == SEGMENT_GRID ||
            symbol->segment() == SEGMENT_STRING
        ;
    };

    symbols_.clear();
    const vector<shared_ptr<Symbol>>& grid_symbols = grid_scope->symbols();
    copy_if( grid_symbols.begin(), grid_symbols.end(), back_inserter(symbols_), include_symbol ); 
    const vector<shared_ptr<Symbol>>& parameter_symbols = parameters_scope->symbols();
    copy_if( parameter_symbols.begin(), parameter_symbols.end(), back_inserter(symbols_), include_symbol ); 
    const vector<shared_ptr<Symbol>>& global_symbols = global_scope->symbols();
    copy_if( global_symbols.begin(), global_symbols.end(), back_inserter(symbols_), include_symbol );

    grid_memory_size_ = offset;
}

void CodeGenerator::evaluate_constant_expression( SyntaxNode* node )
{
    REYES_ASSERT( node );

    Address address;
    switch ( node->node_type() )
    {
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        {
            address = write_constant_float( node->real() );
            break;
        }
        
        case SHADER_NODE_STRING:
            address = write_constant_string( node->string() );
            break;
            
        case SHADER_NODE_TRIPLE:
        {
            address = write_constant_vec3( node->vec3() );
            break;
        }
        
        case SHADER_NODE_SIXTEENTUPLE:
        {
            address = write_constant_mat4x4( node->mat4x4() );
            break;
        }

        case SHADER_NODE_TYPECAST:
            evaluate_constant_expression( node->node(1) );
            break; 
           
        default:
            REYES_ASSERT( false );
            address = write_constant_float( 1.0f );
            break;
    }

    node->set_address( address );
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

Address CodeGenerator::generate_binary_expression( const SyntaxNode* expression )
{
    REYES_ASSERT( expression );
    REYES_ASSERT( expression->instruction() != INSTRUCTION_NULL );
    REYES_ASSERT( expression->node(0) );
    REYES_ASSERT( expression->node(1) );    
    const SyntaxNode* node = expression->node(0);
    const SyntaxNode* other_node = expression->node(1);
    Address result = allocate_address( expression->type(), expression->storage() );
    Address arg0 = generate_expression( *node );
    Address arg1 = generate_expression( *other_node );
    instruction( expression->instruction(), node->type(), node->storage(), other_node->type(), other_node->storage() );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    return result;
}

Address CodeGenerator::generate_assign( int instruction, const SyntaxNode& node )
{
    REYES_ASSERT( node.symbol() );

    const Symbol* symbol = node.symbol().get();
    const SyntaxNode* expression = node.node( 0 );
    Address arg0 = symbol->address();
    Address arg1 = generate_expression( *expression );

    if ( symbol->type() == TYPE_STRING )
    {
        instruction = INSTRUCTION_STRING_ASSIGN;
    }

    CodeGenerator::instruction( instruction, symbol->type(), symbol->storage(), expression->type(), expression->storage() );
    argument( arg0 );
    argument( arg1 );
    return arg1;
}

Address CodeGenerator::generate_convert( Address address, const SyntaxNode& node )
{
    REYES_ASSERT( address >= 0 );
    
    if ( node.original_type() != TYPE_NULL && node.original_type() != node.type() )
    {
        REYES_ASSERT( node.original_type() == TYPE_FLOAT );
        REYES_ASSERT( node.type() != TYPE_FLOAT );

        // The node's original storage is used because promotion to varying
        // happens after conversion.  If storage was used then conversion
        // incorrectly tries to convert varying values.
        ValueStorage storage = node.original_storage();
        if ( storage == STORAGE_NULL )
        {
            storage = node.storage();
        }
        
        Address result = allocate_address( node.type(), storage );
        instruction( INSTRUCTION_CONVERT, node.type(), storage, node.original_type(), storage );
        argument( result );
        argument( address );
        return result;
    }
    return address;
}

Address CodeGenerator::generate_promote( Address address, const SyntaxNode& node )
{
    REYES_ASSERT( address >= 0 );
    if ( node.original_storage() != STORAGE_NULL )
    {
        REYES_ASSERT( node.storage() == STORAGE_VARYING );
        Address result = allocate_address( node.type(), node.storage() );
        instruction( INSTRUCTION_PROMOTE, node.type(), node.storage(), node.type(), node.original_storage() );
        argument( result );
        argument( address );
        return result;
    }
    return address;
}

void CodeGenerator::generate_code_for_list( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_LIST );

    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
        temporary_memory_size_ = max( offset_, temporary_memory_size_ );
    }
    
    const vector<shared_ptr<SyntaxNode> >& nodes = node.nodes();
    for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
    {
        const SyntaxNode& node = *(*i);
        push_address();
        generate_statement( node );
        pop_address();
    }

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_code_for_variable( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_VARIABLE );
    if ( node.node(0)->node_type() != SHADER_NODE_NULL )
    {
        generate_assign( INSTRUCTION_ASSIGN, node );
    }
}

void CodeGenerator::generate_code_for_parameters( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_LIST );
    
    const vector<shared_ptr<SyntaxNode>>& parameters = node.nodes();
    for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = parameters.begin(); i != parameters.end(); ++i )
    {
        const SyntaxNode* variable_node = i->get();
        REYES_ASSERT( variable_node );
        REYES_ASSERT( variable_node->node_type() == SHADER_NODE_VARIABLE );
        if ( variable_node->node(0)->node_type() == SHADER_NODE_TYPECAST )
        {
            REYES_ASSERT( false );
            // What do I do here wrt. allocating memory for typecast parameters?
            // int register_index = generate_typecast_expression( *variable_node->node(0) );
            // variable_node->symbol()->set_register_index( register_index );
            // permanent_registers_ = max( permanent_registers_, register_index );
            Address address = generate_typecast_expression( *variable_node->node(0) );
            (void) address;            
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
            generate_assign( INSTRUCTION_ASSIGN, node );
            break;
            
        case SHADER_NODE_ADD_ASSIGN:
            generate_assign( INSTRUCTION_ADD_ASSIGN, node );
            break;
            
        case SHADER_NODE_SUBTRACT_ASSIGN:
            generate_assign( INSTRUCTION_SUBTRACT_ASSIGN, node );
            break;
            
        case SHADER_NODE_MULTIPLY_ASSIGN:
            generate_assign( INSTRUCTION_MULTIPLY_ASSIGN, node );
            break;
            
        case SHADER_NODE_DIVIDE_ASSIGN:
            generate_assign( INSTRUCTION_DIVIDE_ASSIGN, node );
            break;
            
        default:
            REYES_ASSERT( false );
            break;
    }
}

void CodeGenerator::generate_if_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_IF );
    
    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    const SyntaxNode* expression = node.node( 0 );
    Address mask = generate_expression( *expression );
    push_address();
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression->storage() );
    argument( mask );

    const SyntaxNode* statement_node = node.node( 1 );
    generate_statement( *statement_node );
    instruction( INSTRUCTION_CLEAR_MASK );
    pop_address();

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_if_else_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_IF_ELSE );
    
    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    const SyntaxNode* expression = node.node( 0 );
    Address mask = generate_expression( *expression );
    push_address();
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression->storage() );
    argument( mask );

    const SyntaxNode* statement_node = node.node( 1 );
    generate_statement( *statement_node );
    instruction( INSTRUCTION_INVERT_MASK );

    const SyntaxNode* else_statement_node = node.node( 2 );
    generate_statement( *else_statement_node );
    instruction( INSTRUCTION_CLEAR_MASK );
    pop_address();

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_while_statement( const SyntaxNode& while_node )
{
    REYES_ASSERT( while_node.node_type() == SHADER_NODE_WHILE );

    int default_base_address = offset_;
    Scope* scope = while_node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    push_loop();
    mark_loop_continue();

    push_address();
    const SyntaxNode* expression = while_node.node( 0 );
    Address mask = generate_expression( *expression );
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression->storage() );
    argument( mask );
    pop_address();
    jump_to_end( INSTRUCTION_JUMP_EMPTY, 1 );

    push_address();
    const SyntaxNode* statement_node = while_node.node( 1 );
    generate_statement( *statement_node );
    pop_address();

    instruction( INSTRUCTION_CLEAR_MASK );
    jump_to_begin( INSTRUCTION_JUMP, 1 );
    pop_loop();

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_for_statement( const SyntaxNode& for_node )
{
    REYES_ASSERT( for_node.node_type() == SHADER_NODE_FOR );

    int default_base_address = offset_;
    Scope* scope = for_node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    push_address();
    const SyntaxNode* initialize_statement = for_node.node( 0 );
    generate_statement( *initialize_statement );
    pop_address();
    push_loop();
    
    push_address();
    const SyntaxNode* expression = for_node.node( 1 );
    Address mask = generate_expression( *expression );
    instruction( INSTRUCTION_GENERATE_MASK, TYPE_INTEGER, expression->storage() );
    argument( mask );
    pop_address();
    jump_to_end( INSTRUCTION_JUMP_EMPTY, 1 );
    push_address();

    const SyntaxNode* statement = for_node.node( 3 );
    generate_statement( *statement );
    pop_address();
    mark_loop_continue();
    push_address();

    const SyntaxNode* increment_statement = for_node.node( 2 );
    generate_statement( *increment_statement );
    pop_address();
    instruction( INSTRUCTION_CLEAR_MASK );
    jump_to_begin( INSTRUCTION_JUMP, 1 );
    pop_loop();

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_ambient_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_AMBIENT );

    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    Address light_color = generate_expression( *node.node(0) );
    Address light_opacity = generate_expression( *node.node(1) );
    instruction( INSTRUCTION_AMBIENT );
    argument( light_color );
    argument( light_opacity );

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_solar_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_SOLAR );

    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    const SyntaxNode* expressions = node.node(0);
    if ( expressions->nodes().size() == 0 )
    {
        instruction( INSTRUCTION_SOLAR );
    }
    else
    {
        Address axis = generate_expression( *expressions->node(0) );
        Address angle = generate_expression( *expressions->node(1) );
        Address light_color = generate_expression( *node.node(2) );
        Address light_opacity = generate_expression( *node.node(3) );
        instruction( INSTRUCTION_SOLAR_AXIS_ANGLE );
        argument( axis );
        argument( angle );
        argument( light_color );
        argument( light_opacity );
    }
    
    generate_statement( *node.node(1) );

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_illuminate_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ILLUMINATE );

    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    const SyntaxNode* expressions = node.node(0);
    if ( expressions->nodes().size() == 1 )
    {
        Address position = generate_expression( *expressions->node(0) );
        Address surface_position = generate_expression( *node.node(2) );
        Address light_direction = generate_expression( *node.node(3) );
        Address light_color = generate_expression( *node.node(4) );
        Address light_opacity = generate_expression( *node.node(5) );
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
        Address position = generate_expression( *expressions->node(0) );
        Address axis = generate_expression( *expressions->node(1) );
        Address angle = generate_expression( *expressions->node(2) );
        Address surface_position = generate_expression( *node.node(2) );
        Address light_direction = generate_expression( *node.node(3) );
        Address light_color = generate_expression( *node.node(4) );
        Address light_opacity = generate_expression( *node.node(5) );
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

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
    }
}

void CodeGenerator::generate_illuminance_statement( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ILLUMINANCE );

    int default_base_address = offset_;
    Scope* scope = node.scope();
    if ( scope )
    {
        offset_ = scope->enter( SEGMENT_TEMPORARY, offset_, maximum_vertices_ );
    }

    const SyntaxNode* expressions = node.node(0);
    if ( expressions->nodes().size() == 1 )
    {
        /*
        int arg0 = generate_expression( *expressions->node(0) );
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

        const SyntaxNode& position_node = *expressions->node(0);
        const SyntaxNode& axis_node = *expressions->node(1);
        const SyntaxNode& angle_node = *expressions->node(2);
        const SyntaxNode& light_direction_node = *node.node(2);
        const SyntaxNode& light_color_node = *node.node(3);
        const SyntaxNode& light_opacity_node = *node.node(4);

        Address position = generate_expression( position_node );
        Address axis = generate_expression( axis_node );
        Address angle = generate_expression( angle_node );
        Address light_direction = generate_expression( light_direction_node );
        Address light_color = generate_expression( light_color_node );
        Address light_opacity = generate_expression( light_opacity_node );

        push_address();
        Address mask = allocate_address( TYPE_INTEGER, STORAGE_VARYING );
        instruction( INSTRUCTION_ILLUMINANCE_AXIS_ANGLE, position_node.type(), position_node.storage(), axis_node.type(), axis_node.storage(), angle_node.type(), angle_node.storage() );
        argument( position );
        argument( axis );
        argument( angle );
        argument( light_direction );
        argument( light_color );
        argument( light_opacity );
        argument( mask );
        instruction( INSTRUCTION_GENERATE_MASK );
        argument( mask );
        pop_address();
        
        push_address();
        generate_statement( *node.node(1) );
        pop_address();

        instruction( INSTRUCTION_CLEAR_MASK );        
        jump_to_begin( INSTRUCTION_JUMP, 1 );
        pop_loop();
    }

    if ( scope )
    {
        offset_ = scope->leave( default_base_address );
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

Address CodeGenerator::generate_expression( const SyntaxNode& node )
{
    Address address;
    switch ( node.node_type() )
    {
        case SHADER_NODE_CALL:
            address = generate_call_expression( node );
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
            address = generate_binary_expression( &node );
            break;
                        
        case SHADER_NODE_DIVIDE:
            address = generate_divide_expression( node );
            break;

        case SHADER_NODE_NEGATE:
            address = generate_negate_expression( node );
            break;
            
        case SHADER_NODE_TERNARY:
            address = generate_ternary_expression( node );
            break;
            
        case SHADER_NODE_TYPECAST:
            address = generate_typecast_expression( node );
            break;
            
        case SHADER_NODE_ASSIGN:    
            address = generate_assign( INSTRUCTION_ASSIGN, node );
            break;
            
        case SHADER_NODE_ADD_ASSIGN:
            address = generate_assign( INSTRUCTION_ADD_ASSIGN, node );
            break;
            
        case SHADER_NODE_SUBTRACT_ASSIGN:
            address = generate_assign( INSTRUCTION_SUBTRACT_ASSIGN, node );
            break;
            
        case SHADER_NODE_MULTIPLY_ASSIGN:
            address = generate_assign( INSTRUCTION_MULTIPLY_ASSIGN, node );
            break;
            
        case SHADER_NODE_DIVIDE_ASSIGN:
            address = generate_assign( INSTRUCTION_DIVIDE_ASSIGN, node );
            break;
            
        case SHADER_NODE_INTEGER:
        case SHADER_NODE_REAL:
        case SHADER_NODE_STRING:
        case SHADER_NODE_TRIPLE:
        case SHADER_NODE_SIXTEENTUPLE:
            address = generate_constant_expression( node );
            break;

        case SHADER_NODE_TEXTURE:
            address = generate_vec3_texture_expression( node );
            break;
            
        case SHADER_NODE_SHADOW:
            address = generate_shadow_expression( node );
            break;
            
        case SHADER_NODE_IDENTIFIER:
            address = generate_identifier_expression( node );
            break;
            
        default:
            REYES_ASSERT( false );
            break;
    }    
    address = generate_convert( address, node );
    address = generate_promote( address, node );
    return address;
}

Address CodeGenerator::generate_call_expression( const SyntaxNode& call_node )
{
    REYES_ASSERT( call_node.node_type() == SHADER_NODE_CALL );

    Address result = allocate_address( call_node.type(), call_node.storage() );

    const int MAXIMUM_ARGUMENTS = 16;
    Address arguments [MAXIMUM_ARGUMENTS] = { 0 };
    REYES_ASSERT( int(call_node.nodes().size()) < MAXIMUM_ARGUMENTS );
    for ( int i = 0; i < int(call_node.nodes().size()); ++i )
    {
        const SyntaxNode* node = call_node.node( i );
        Address argument = generate_expression( *node );
        arguments[i] = argument;
    }
    
    const shared_ptr<Symbol>& symbol = call_node.symbol();
    instruction( INSTRUCTION_CALL, symbol->type(), symbol->storage() );
    argument( call_node.symbol()->index() );
    argument( call_node.nodes().size() );
    argument( result );
    for ( int i = 0; i < int(call_node.nodes().size()); ++i )
    {
        argument( arguments[i] );
    }    

    return result;
}

Address CodeGenerator::generate_divide_expression( const SyntaxNode& divide_node )
{
    REYES_ASSERT( divide_node.node(0) );
    REYES_ASSERT( divide_node.node(1) );
    
    const SyntaxNode& node = *divide_node.node(0);
    const SyntaxNode& other_node = *divide_node.node(1);
    
    Address result = allocate_address( divide_node.type(), divide_node.storage() );
    Address arg0 = generate_expression( node );
    Address arg1 = generate_expression( other_node );
    instruction( INSTRUCTION_DIVIDE, node.type(), node.storage(), other_node.type(), other_node.storage() );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    return result;
}

Address CodeGenerator::generate_negate_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_NEGATE );
    Address result = allocate_address( node.type(), node.storage() );
    Address arg0 = generate_expression( *node.node(0) ); 
    instruction( INSTRUCTION_NEGATE, node.type(), node.storage() );
    argument( result );
    argument( arg0 );
    return result;
}

Address CodeGenerator::generate_ternary_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TERNARY );
    REYES_ASSERT( false );
    return Address();
}

Address CodeGenerator::generate_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    
    if ( node.node(1)->node_type() == SHADER_NODE_TEXTURE )
    {
        return generate_texture_typecast_expression( node );
    }
    else if ( node.node(1)->node_type() == SHADER_NODE_ENVIRONMENT )
    {
        return generate_environment_typecast_expression( node );
    }
    else if ( node.node(1)->type() == TYPE_COLOR || node.node(1)->type() == TYPE_POINT || node.node(1)->type() == TYPE_VECTOR || node.node(1)->type() == TYPE_NORMAL )
    {
        return generate_vec3_typecast_expression( node );
    }
    else if ( node.node(1)->type() == TYPE_MATRIX )
    {
        return generate_mat4x4_typecast_expression( node );
    }
    else
    {
        return generate_expression( *node.node(1) );
    }

    REYES_ASSERT( false );
    return Address();
}

Address CodeGenerator::generate_vec3_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    
    if ( !node.node(0)->nodes().empty() )
    {
        const SyntaxNode* space_expression = node.node( 0 )->node( 0 );
        Address arg0 = generate_expression( *space_expression );

        const SyntaxNode* value_expression = node.node( 1 );
        Address arg1 = generate_expression( *value_expression );

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

        Address result = allocate_address( value_expression->type(), value_expression->storage() );
        argument( result );
        argument( arg0 );
        argument( arg1 );
        return result;
    }

    return generate_expression( *node.node(1) );
}

Address CodeGenerator::generate_mat4x4_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    
    if ( !node.node(0)->nodes().empty() )
    {
        REYES_ASSERT( node.node(0)->node_type() == SHADER_NODE_MATRIX_TYPE );
        const SyntaxNode* tospace_node = node.node( 0 )->node( 0 );
        Address arg0 = generate_expression( *tospace_node );
        const SyntaxNode* value_node = node.node( 1 );
        Address arg1 = generate_expression( *value_node );
        instruction( INSTRUCTION_TRANSFORM_MATRIX, value_node->type(), value_node->storage() );
        Address result = allocate_address( value_node->type(), value_node->storage() );
        argument( result );
        argument( arg0 );
        argument( arg1 );
        return result;
    }
    else
    {
        return generate_expression( *node.node(1) );
    }

    REYES_ASSERT( false );
    return Address();
}

Address CodeGenerator::generate_texture_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    REYES_ASSERT( node.node(1)->node_type() == SHADER_NODE_TEXTURE );
    
    if ( node.node(1)->node_type() == SHADER_NODE_TEXTURE )
    {        
        switch ( node.node(0)->node_type() )
        {
            case SHADER_NODE_FLOAT_TYPE:
                return generate_float_texture_expression( *node.node(1) );
                break;

            case SHADER_NODE_COLOR_TYPE:
            case SHADER_NODE_POINT_TYPE:
            case SHADER_NODE_VECTOR_TYPE:
            case SHADER_NODE_NORMAL_TYPE:
                return generate_vec3_texture_expression( *node.node(1) );
                break;
                
            default:
                REYES_ASSERT( false );
                return 0;
                break;
        }
    }    

    REYES_ASSERT( false );
    return Address();
}

Address CodeGenerator::generate_environment_typecast_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TYPECAST );
    REYES_ASSERT( node.node(1)->node_type() == SHADER_NODE_ENVIRONMENT );
    
    if ( node.node(1)->node_type() == SHADER_NODE_ENVIRONMENT )
    {        
        switch ( node.node(0)->node_type() )
        {
            case SHADER_NODE_FLOAT_TYPE:
                return generate_float_environment_expression( *node.node(1) );
                break;

            case SHADER_NODE_COLOR_TYPE:
            case SHADER_NODE_POINT_TYPE:
            case SHADER_NODE_VECTOR_TYPE:
            case SHADER_NODE_NORMAL_TYPE:
                return generate_vec3_environment_expression( *node.node(1) );
                break;
                
            default:
                REYES_ASSERT( false );
                break;
        }
    }    

    REYES_ASSERT( false );
    return Address();
}

Address CodeGenerator::generate_shadow_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_SHADOW );

    const SyntaxNode* texture_name = node.node( 0 );
    const SyntaxNode* position = node.node( 1 );
    const SyntaxNode* bias = node.node( 2 );
    Address result = allocate_address( TYPE_FLOAT, position->storage() );
    Address arg0 = generate_expression( *texture_name );
    Address arg1 = generate_expression( *position );
    Address arg2 = generate_expression( *bias );
    instruction( INSTRUCTION_SHADOW );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    argument( arg2 );
    return result;
}

Address CodeGenerator::generate_float_texture_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TEXTURE );

    const SyntaxNode* texture_name = node.node( 0 );
    const SyntaxNode* s = node.node( 1 );
    const SyntaxNode* t = node.node( 2 );
    ValueStorage storage = s->storage() == STORAGE_VARYING || t->storage() == STORAGE_VARYING ? STORAGE_VARYING : STORAGE_UNIFORM;
    Address result = allocate_address( TYPE_FLOAT, storage );
    Address arg0 = generate_expression( *texture_name );
    Address arg1 = generate_expression( *s );
    Address arg2 = generate_expression( *t );
    instruction( INSTRUCTION_FLOAT_TEXTURE );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    argument( arg2 );
    return result;
}

Address CodeGenerator::generate_vec3_texture_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_TEXTURE );

    const SyntaxNode* texture_name = node.node( 0 );
    const SyntaxNode* s = node.node( 1 );
    const SyntaxNode* t = node.node( 2 );
    ValueStorage storage = s->storage() == STORAGE_VARYING || t->storage() == STORAGE_VARYING ? STORAGE_VARYING : STORAGE_UNIFORM;
    Address result = allocate_address( TYPE_VECTOR, storage );
    Address arg0 = generate_expression( *texture_name );
    Address arg1 = generate_expression( *s );
    Address arg2 = generate_expression( *t );
    instruction( INSTRUCTION_VEC3_TEXTURE );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    argument( arg2 );
    return result;
}

Address CodeGenerator::generate_float_environment_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ENVIRONMENT );
    const SyntaxNode* texture_name = node.node( 0 );
    const SyntaxNode* direction = node.node( 1 );
    Address result = allocate_address( TYPE_FLOAT, direction->storage() );
    Address arg0 = generate_expression( *texture_name );
    Address arg1 = generate_expression( *direction );
    instruction( INSTRUCTION_FLOAT_ENVIRONMENT );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    return result;
}

Address CodeGenerator::generate_vec3_environment_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_ENVIRONMENT );

    const SyntaxNode* texture_name = node.node( 0 );
    const SyntaxNode* direction = node.node( 1 );
    Address result = allocate_address( TYPE_FLOAT, direction->storage() );
    Address arg0 = generate_expression( *texture_name );
    Address arg1 = generate_expression( *direction );
    instruction( INSTRUCTION_VEC3_ENVIRONMENT );
    argument( result );
    argument( arg0 );
    argument( arg1 );
    return result;
}

Address CodeGenerator::generate_constant_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.constant_address() >= 0 && node.constant_address() < int(constant_data_.size()) );
    return node.address();
}

Address CodeGenerator::generate_identifier_expression( const SyntaxNode& node )
{
    REYES_ASSERT( node.node_type() == SHADER_NODE_IDENTIFIER );
    const shared_ptr<Symbol>& symbol = node.symbol();
    REYES_ASSERT( symbol );
    if ( symbol->storage() == STORAGE_CONSTANT )
    {
        REYES_ASSERT( node.constant_address() >= 0 );
        return node.address();
    }
    return symbol->address();
}

void CodeGenerator::instruction( int instruction )
{
    encoder_->instruction( instruction );
}

void CodeGenerator::instruction( int instruction, ValueType type, ValueStorage storage )
{
    encoder_->instruction( instruction, type, storage );
}

void CodeGenerator::instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1 )
{
    encoder_->instruction( instruction, type0, storage0, type1, storage1 );
}

void CodeGenerator::instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1, ValueType type2, ValueStorage storage2 )
{
    encoder_->instruction( instruction, type0, storage0, type1, storage1, type2, storage2 );
}

void CodeGenerator::argument( int argument )
{
    encoder_->argument( argument );
}

void CodeGenerator::argument( Address address )
{
    argument( address.value() );
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

Address CodeGenerator::write_constant( const void* value, int size )
{
    REYES_ASSERT( size >= 0 );
    int offset = int(constant_data_.size());
    constant_data_.insert( constant_data_.end(), size, 0 );
    memcpy( &constant_data_[offset], value, size );
    return Address( SEGMENT_CONSTANT, offset );    
}

Address CodeGenerator::write_constant_float( float value )
{
    return write_constant( &value, sizeof(value) );
}

Address CodeGenerator::write_constant_vec3( const math::vec3& value )
{
    return write_constant( &value, sizeof(value) );
}

Address CodeGenerator::write_constant_mat4x4( const math::mat4x4& value )
{
    return write_constant( &value, sizeof(value) );
}

Address CodeGenerator::write_constant_string( const std::string& value )
{
    return write_constant( value.data(), value.size() + 1 );
}

Address CodeGenerator::allocate_address( ValueType type, ValueStorage storage )
{
    int offset = offset_;
    offset_ += size_by_type_and_storage( type, storage );
    temporary_memory_size_ = max( temporary_memory_size_, offset_ );
    return Address( SEGMENT_TEMPORARY, offset );
}

void CodeGenerator::push_address()
{
    temporary_addresses_.push_back( offset_ );
}

void CodeGenerator::pop_address()
{
    REYES_ASSERT( !temporary_addresses_.empty() );
    offset_ = temporary_addresses_.back();
    temporary_addresses_.pop_back();
}

int CodeGenerator::size_by_type_and_storage( ValueType type, ValueStorage storage ) const
{
    int size_by_type = 0;
    switch ( type )
    {
        case TYPE_NULL: 
            size_by_type = 0;
            break;

        case TYPE_INTEGER:
            size_by_type = sizeof(int);
            break;

        case TYPE_FLOAT:
            size_by_type = sizeof(float);
            break;

        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
            size_by_type = 3 * sizeof(float);
            break;

        case TYPE_COLOR:
            size_by_type = 4 * sizeof(float);
            break;

        case TYPE_MATRIX:
            size_by_type = 16 * sizeof(float);
            break;

        default:
            REYES_ASSERT( false );
            size_by_type = 0;
            break;
    }

    int size_by_storage = storage == STORAGE_VARYING ? maximum_vertices_ : 1;
    return size_by_type * size_by_storage;
}
