//
// VirtualMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "VirtualMachine.hpp"
#include "Renderer.hpp"
#include "Value.hpp"
#include "Shader.hpp"
#include "Symbol.hpp"
#include "Texture.hpp"
#include "Grid.hpp"
#include "Light.hpp"
#include "Instruction.hpp"
#include "color_functions.hpp"
#include <reyes/reyes_virtual_machine/add.hpp>
#include <reyes/reyes_virtual_machine/subtract.hpp>
#include <reyes/reyes_virtual_machine/multiply.hpp>
#include <reyes/reyes_virtual_machine/divide.hpp>
#include <reyes/reyes_virtual_machine/dot.hpp>
#include <reyes/reyes_virtual_machine/negate.hpp>
#include <reyes/reyes_virtual_machine/equal.hpp>
#include <reyes/reyes_virtual_machine/not_equal.hpp>
#include <reyes/reyes_virtual_machine/greater.hpp>
#include <reyes/reyes_virtual_machine/greater_equal.hpp>
#include <reyes/reyes_virtual_machine/less.hpp>
#include <reyes/reyes_virtual_machine/less_equal.hpp>
#include <reyes/reyes_virtual_machine/logical_and.hpp>
#include <reyes/reyes_virtual_machine/logical_or.hpp>
#include <reyes/reyes_virtual_machine/assign.hpp>
#include <reyes/reyes_virtual_machine/add_assign.hpp>
#include <reyes/reyes_virtual_machine/subtract_assign.hpp>
#include <reyes/reyes_virtual_machine/multiply_assign.hpp>
#include <reyes/reyes_virtual_machine/divide_assign.hpp>
#include <reyes/reyes_virtual_machine/promote.hpp>
#include <reyes/reyes_virtual_machine/convert.hpp>
#include <reyes/reyes_virtual_machine/transform.hpp>
#include <reyes/reyes_virtual_machine/vtransform.hpp>
#include <reyes/reyes_virtual_machine/Dispatch.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"
#include <algorithm>
#include <limits.h>

using std::max;
using std::swap;
using std::make_pair;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

VirtualMachine::ConditionMask::ConditionMask()
: mask_(),
  processed_( 0 )
{
}

void VirtualMachine::ConditionMask::generate( std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_INTEGER );
    
    const int size = value->size();
    const int* values = value->int_values();

    mask_.insert( mask_.end(), size, 0 );
    processed_ = 0;
    unsigned char* mask = &mask_[0];
    for ( unsigned int i = 0; i < size; ++i )
    {
        int process = values[i];
        mask[i] = process != 0;
        processed_ += process != 0;
    }
}

void VirtualMachine::ConditionMask::generate( const ConditionMask& condition_mask, std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_INTEGER );
    REYES_ASSERT( condition_mask.mask_.size() == value->size() );
    
    const int size = value->size();
    const int* values = value->int_values();
    const unsigned char* existing_mask = &condition_mask.mask_[0];

    mask_.insert( mask_.end(), size, 0 );
    processed_ = 0;
    unsigned char* mask = &mask_[0];
    for ( unsigned int i = 0; i < size; ++i )
    {
        int process = values[i] && existing_mask[i];
        mask[i] = process != 0;
        processed_ += process != 0;
    }
}

void VirtualMachine::ConditionMask::invert()
{
    const unsigned int size = mask_.size();
    unsigned char* mask = &mask_[0];
    
    processed_ = 0;
    for ( unsigned int i = 0; i < size; ++i )
    {
        int process = !mask[i];
        mask[i] = process != 0;
        processed_ += process != 0;
    }
}

bool VirtualMachine::ConditionMask::empty() const
{
    return processed_ == 0;
}

VirtualMachine::VirtualMachine()
: renderer_( NULL ),
  grid_( NULL ),
  shader_( NULL ),
  values_(),
  registers_(),
  register_index_( 0 ),
  light_index_( INT_MAX ),
  code_begin_( NULL ),
  code_end_( NULL ),
  masks_(),
  code_( NULL )
{
}

VirtualMachine::VirtualMachine( const Renderer& renderer )
: renderer_( &renderer ),
  grid_( NULL ),
  shader_( NULL ),
  values_(),
  registers_(),
  register_index_( 0 ),
  light_index_( INT_MAX ),
  code_begin_( NULL ),
  code_end_( NULL ),
  masks_(),
  code_( NULL )
{
}

void VirtualMachine::initialize( Grid& parameters, Shader& shader )
{
    // @todo
    //  It's not really correct to set VirtualMachine::grid_ to the address of
    //  the parameters Grid passed to VirtualMachine::initialize() but it's 
    //  done at least for now to hopefully prevent crashes in the 
    //  VirtualMachine when it refers to grid_.
    grid_ = &parameters;
    shader_ = &shader;

    const vector<shared_ptr<Symbol>>& symbols = shader.symbols();
    for ( int i = 0; i < shader.parameters(); ++i )
    {
        const shared_ptr<Symbol>& symbol = symbols[i];
        REYES_ASSERT( symbol );
        parameters.add_value( symbol->identifier(), symbol->type() );
    }

    construct( shader.initialize_address(), shader.shade_address() );
    initialize_registers( parameters );
    execute();
    
    shader_ = NULL;
    grid_ = NULL;
}

void VirtualMachine::shade( Grid& globals, Grid& parameters, Shader& shader )
{   
    grid_ = &globals;
    shader_ = &shader;
    
    construct( shader.shade_address(), shader.end_address() );
    initialize_registers( parameters );
    initialize_registers( globals );
    execute();
    
    shader_ = NULL;
    grid_ = NULL;
}

void VirtualMachine::construct( int start, int finish )
{
    REYES_ASSERT( shader_ );
    REYES_ASSERT( !shader_->code().empty() );
    REYES_ASSERT( start >= 0 && start <= int(shader_->code().size()) );
    REYES_ASSERT( finish >= 0 && finish <= int(shader_->code().size()) );
    REYES_ASSERT( start <= finish );

    code_begin_ = &shader_->code().front() + start;
    code_end_ = &shader_->code().front() + finish;
    register_index_ = shader_->permanent_registers();
    
    values_.reserve( shader_->registers() );
    while ( values_.size() < shader_->registers() )
    {
        shared_ptr<Value> value( new Value() );
        values_.push_back( value );
    }
    
    registers_.clear();
    registers_.reserve( shader_->registers() );
    registers_.insert( registers_.end(), max(shader_->registers() - int(registers_.size()), 0), shared_ptr<Value>() );

    // Initialize registers for constants.
    REYES_ASSERT( shader_->constants() == int(shader_->values().size()) );
    unsigned int register_index = 0;
    while ( register_index < shader_->values().size() )
    {
        shared_ptr<Value>& value = values_[register_index];
        registers_[register_index] = shader_->values()[register_index];
        ++register_index;
    }
       
    // Initialize registers for global, local, and temporary values.
    while ( register_index < shader_->registers() )
    {
        registers_[register_index] = values_[register_index];
        ++register_index;
    }

    // @todo
    //  The number of masks reserved implicitly limits the number of nested
    //  for, while, if, solar, and illuminate statements in the shader 
    //  language so I should make sure that limit is enforced during parsing
    //  or code generation.
    const unsigned int MASKS_RESERVE = 8;
    masks_.reserve( MASKS_RESERVE );
}

void VirtualMachine::initialize_registers( Grid& grid )
{
    const map<string, shared_ptr<Value>>& values = grid.values_by_identifier();
    for ( map<string, shared_ptr<Value>>::const_iterator i = values.begin(); i != values.end(); ++i )
    {
        Symbol* symbol = shader_->find_symbol( i->first ).get();
        if ( symbol )
        {
            registers_[symbol->register_index()] = i->second;
        }
    }
}

void VirtualMachine::execute()
{
    REYES_ASSERT( code_begin_ );
    REYES_ASSERT( code_end_ );
    REYES_ASSERT( code_begin_ <= code_end_ );
    
    code_ = code_begin_;
    while ( code_ < code_end_ )
    {
        switch ( instruction() )
        {
            case INSTRUCTION_HALT:    
                execute_halt();
                break;                

            case INSTRUCTION_RESET:
                execute_reset();
                break;
                
            case INSTRUCTION_CLEAR_MASK:
                execute_clear_mask();
                break;
            
            case INSTRUCTION_GENERATE_MASK:
                execute_generate_mask();
                break;
                
            case INSTRUCTION_INVERT_MASK:
                execute_invert_mask();
                break;
                
            case INSTRUCTION_JUMP_EMPTY:
                execute_jump_empty();
                break;
            
            case INSTRUCTION_JUMP_NOT_EMPTY:
                execute_jump_not_empty();
                break;
            
            case INSTRUCTION_JUMP_ILLUMINANCE:
                execute_jump_illuminance();
                break;
            
            case INSTRUCTION_JUMP:
                execute_jump();
                break;
            
            case INSTRUCTION_TRANSFORM:
                execute_transform();
                break;
                
            case INSTRUCTION_TRANSFORM_VECTOR:
                execute_transform_vector();
                break;
                
            case INSTRUCTION_TRANSFORM_NORMAL:
                execute_transform_normal();
                break;
                
            case INSTRUCTION_TRANSFORM_COLOR:
                execute_transform_color();
                break;
                
            case INSTRUCTION_TRANSFORM_MATRIX:
                execute_transform_matrix();
                break;
                
            case INSTRUCTION_DOT:
                execute_dot();
                break;
                
            case INSTRUCTION_MULTIPLY:
                execute_multiply();
                break;
                
            case INSTRUCTION_DIVIDE:
                execute_divide();
                break;
                
            case INSTRUCTION_ADD:
                execute_add();
                break;
            
            case INSTRUCTION_SUBTRACT:
                execute_subtract();
                break;
            
            case INSTRUCTION_GREATER:
                execute_greater();
                break;
            
            case INSTRUCTION_GREATER_EQUAL:
                execute_greater_equal();
                break;
            
            case INSTRUCTION_LESS:
                execute_less();
                break;
            
            case INSTRUCTION_LESS_EQUAL:
                execute_less_equal();
                break;
            
            case INSTRUCTION_AND:
                execute_and();
                break;
            
            case INSTRUCTION_OR:
                execute_or();
                break;
            
            case INSTRUCTION_EQUAL:
                execute_equal();
                break;
            
            case INSTRUCTION_NOT_EQUAL:
                execute_not_equal();
                break;
                        
            case INSTRUCTION_NEGATE:
                execute_negate();
                break;
                
            case INSTRUCTION_CONVERT:
                execute_convert();
                break;

            case INSTRUCTION_PROMOTE:
                execute_promote();
                break;
            
            case INSTRUCTION_ASSIGN:
                execute_assign();
                break;
                        
            case INSTRUCTION_ASSIGN_STRING:
                execute_assign_string();
                break;
            
            case INSTRUCTION_ADD_ASSIGN:
                execute_add_assign();
                break;
            
            case INSTRUCTION_MULTIPLY_ASSIGN:
                execute_multiply_assign();
                break;
            
            case INSTRUCTION_DIVIDE_ASSIGN:
                execute_divide_assign();
                break;
            
            case INSTRUCTION_FLOAT_TEXTURE:
                execute_float_texture();
                break;
            
            case INSTRUCTION_VEC3_TEXTURE:
                execute_vec3_texture();
                break;
            
            case INSTRUCTION_FLOAT_ENVIRONMENT:
                execute_float_environment();
                break;
            
            case INSTRUCTION_VEC3_ENVIRONMENT:
                execute_vec3_environment();
                break;
            
            case INSTRUCTION_SHADOW:
                execute_shadow();
                break;
                
            case INSTRUCTION_CALL_0:
                execute_call_0();
                break;
                
            case INSTRUCTION_CALL_1:
                execute_call_1();
                break;
                
            case INSTRUCTION_CALL_2:
                execute_call_2();
                break;
            
            case INSTRUCTION_CALL_3:
                execute_call_3();
                break;

            case INSTRUCTION_CALL_4:
                execute_call_4();
                break;

            case INSTRUCTION_CALL_5:
                execute_call_5();                
                break;

            case INSTRUCTION_AMBIENT:
                execute_ambient();
                break;
            
            case INSTRUCTION_SOLAR:
            case INSTRUCTION_SOLAR_AXIS_ANGLE:
                execute_solar_axis_angle();
                break;
            
            case INSTRUCTION_ILLUMINATE:
                execute_illuminate();
                break;

            case INSTRUCTION_ILLUMINATE_AXIS_ANGLE:
                execute_illuminate_axis_angle();
                break;
            
            case INSTRUCTION_ILLUMINANCE_AXIS_ANGLE:
                execute_illuminance_axis_angle();
                break;

            default:    
                REYES_ASSERT( false );
                execute_halt();
                break;                
        }
    }
}

void VirtualMachine::jump_illuminance( int distance )
{
    const int lights = int(grid_->lights().size());
    if ( light_index_ < lights )    
    {
        ++light_index_;        
    }
    else
    {
        light_index_ = 0;
    }
    
    while ( light_index_ < lights && grid_->get_light(light_index_)->type() == LIGHT_AMBIENT )
    {
        ++light_index_;
    }
    
    if ( light_index_ >= lights )
    {
        light_index_ = INT_MAX;
        jump( distance );
    }
}

void VirtualMachine::jump( int distance )
{
    REYES_ASSERT( code_ + distance >= code_begin_ && code_ + distance < code_end_ );
    code_ += distance;
}

int VirtualMachine::instruction()
{
    return word();
}

int VirtualMachine::argument()
{
    return quad();
}

int VirtualMachine::word()
{
    REYES_ASSERT( code_ );
    REYES_ASSERT( code_end_ );
    REYES_ASSERT( code_ < code_end_ );
    int value = *(const short*) code_;
    code_ += sizeof(short);
    return value;
}

int VirtualMachine::quad()
{
    REYES_ASSERT( code_ );
    REYES_ASSERT( code_end_ );
    REYES_ASSERT( code_ < code_end_ );
    int value = *(const int*) code_;
    code_ += sizeof(int);
    return value;
}

void VirtualMachine::execute_halt()
{
    code_ = code_end_;
}

void VirtualMachine::execute_reset()
{
    word();
    int index = argument();
    reset_register( index );
}

void VirtualMachine::execute_clear_mask()
{
    word();
    pop_mask();
}

void VirtualMachine::execute_generate_mask()
{
    word();
    int mask = argument();
    push_mask( registers_[mask] );
}

void VirtualMachine::execute_invert_mask()
{
    word();
    invert_mask();
}

void VirtualMachine::execute_jump_empty()
{
    word();
    int distance = argument();
    if ( mask_empty() )
    {
        jump( distance );
        pop_mask();
    }
}

void VirtualMachine::execute_jump_not_empty()
{
    word();
    int distance = argument();
    if ( !mask_empty() )
    {
        jump( distance );
    }
}

void VirtualMachine::execute_jump_illuminance()
{
    word();
    int distance = argument();
    jump_illuminance( distance );
}

void VirtualMachine::execute_jump()
{
    word();
    int distance = argument();
    jump( distance );
}

void VirtualMachine::execute_transform()
{
    int dispatch = word();
    const shared_ptr<Value>& result = registers_[allocate_register()];
    const shared_ptr<Value>& fromspace = registers_[argument()];
    const shared_ptr<Value>& point = registers_[argument()];
    REYES_ASSERT( renderer_ );
    result->reset( point->type(), point->storage(), point->size() );
    transform( 
        dispatch,
        result->vec3_values(),
        renderer_->transform_from( fromspace->string_value() ),
        point->vec3_values(),
        point->size()
    );
}

void VirtualMachine::execute_transform_vector()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& fromspace = registers_[argument()];
    const shared_ptr<Value>& vector = registers_[argument()];
    REYES_ASSERT( renderer_ );
    result->reset( vector->type(), vector->storage(), vector->size() );
    vtransform( 
        dispatch,
        result->vec3_values(),
        renderer_->transform_from( fromspace->string_value() ),
        vector->vec3_values(),
        vector->size()
    );
}

void VirtualMachine::execute_transform_normal()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int fromspace = argument();
    int vector = argument();
    REYES_ASSERT( renderer_ );
    result->ntransform( renderer_->transform_from(registers_[fromspace]->string_value()), registers_[vector] );
}

void VirtualMachine::execute_transform_color()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int fromspace = argument();
    int color = argument();
    REYES_ASSERT( renderer_ );
    ctransform( *renderer_, *grid_, result, registers_[fromspace], registers_[color] );
}

void VirtualMachine::execute_transform_matrix()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int tospace = argument();
    int matrix = argument();
    REYES_ASSERT( renderer_ );
    result->transform_matrix( renderer_->transform_to(registers_[tospace]->string_value()), registers_[matrix] );
}

void VirtualMachine::execute_dot()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    const unsigned int length = max( lhs->size(), rhs->size() );
    result->reset( TYPE_FLOAT, max(lhs->storage(), rhs->storage()), length );
    dot( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        length
    );
}

void VirtualMachine::execute_multiply()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( lhs->type(), max(lhs->storage(), rhs->storage()), lhs->size() );
    multiply( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_divide()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( lhs->type(), max(lhs->storage(), rhs->storage()), lhs->size() );
    divide( 
        dispatch, 
        reinterpret_cast<float*>(result->values()), 
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_add()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( lhs->type(), max(lhs->storage(), rhs->storage()), lhs->size() );
    add( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_subtract()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( lhs->type(), max(lhs->storage(), rhs->storage()), lhs->size() );
    subtract( 
        dispatch,
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_greater()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    greater(
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size() 
    );
}

void VirtualMachine::execute_greater_equal()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    greater_equal(
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size() 
    );
}

void VirtualMachine::execute_less()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    less(
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size() 
    );
}

void VirtualMachine::execute_less_equal()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    less_equal(
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size() 
    );
}

void VirtualMachine::execute_and()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    logical_and( 
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const int*>(lhs->values()),
        reinterpret_cast<const int*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_or()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    logical_or( 
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const int*>(lhs->values()),
        reinterpret_cast<const int*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_equal()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( lhs->type(), max(lhs->storage(), rhs->storage()), lhs->size() );
    equal(
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_not_equal()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& lhs = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( TYPE_INTEGER, max(lhs->storage(), rhs->storage()), lhs->size() );
    not_equal( 
        dispatch,
        reinterpret_cast<int*>(result->values()),
        reinterpret_cast<const float*>(lhs->values()),
        reinterpret_cast<const float*>(rhs->values()),
        lhs->size()
    );
}

void VirtualMachine::execute_negate()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& value = registers_[argument()];
    result->reset( value->type(), value->storage(), value->size() );
    negate( dispatch, reinterpret_cast<float*>(result->values()), reinterpret_cast<const float*>(value->values()), value->size() );
}

void VirtualMachine::execute_convert()
{
    int dispatch = word();
    ValueType type = TYPE_NULL;
    switch ( dispatch >> 8 )
    {
        case DISPATCH_U1:
        case DISPATCH_V1:
            type = TYPE_FLOAT;
            break;

        case DISPATCH_U3:
        case DISPATCH_V3:
            type = TYPE_VECTOR;
            break;

        case DISPATCH_U16:
        case DISPATCH_V16:
            type = TYPE_MATRIX;
            break;

        default:
            break;
    }    

    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( type, rhs->storage(), rhs->size() );
    convert( 
        dispatch,
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        rhs->size()
    );
}

void VirtualMachine::execute_promote()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[allocate_register()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    result->reset( rhs->type(), STORAGE_VARYING, grid_->size() );
    promote( 
        dispatch,
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        grid_->size()
    );
}

void VirtualMachine::execute_assign()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    const unsigned char* mask = rhs->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->reset( rhs->type(), rhs->storage(), rhs->size() );
    assign(
        dispatch,
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        mask,
        rhs->size()
    );
}

void VirtualMachine::execute_assign_string()
{
    word();
    shared_ptr<Value> result = registers_[argument()];
    shared_ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->assign_string( value, mask );
}

void VirtualMachine::execute_add_assign()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    const unsigned char* mask = rhs->storage() == STORAGE_VARYING ? get_mask() : NULL;
    add_assign( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        mask,
        rhs->size()
    );
}

void VirtualMachine::execute_subtract_assign()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    const unsigned char* mask = rhs->storage() == STORAGE_VARYING ? get_mask() : NULL;
    subtract_assign( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        mask,
        rhs->size()
    );
}

void VirtualMachine::execute_multiply_assign()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    const unsigned char* mask = rhs->storage() == STORAGE_VARYING ? get_mask() : NULL;
    multiply_assign( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        mask,
        rhs->size()
    );
}

void VirtualMachine::execute_divide_assign()
{
    int dispatch = word();
    shared_ptr<Value> result = registers_[argument()];
    const shared_ptr<Value>& rhs = registers_[argument()];
    const unsigned char* mask = rhs->storage() == STORAGE_VARYING ? get_mask() : NULL;
    divide_assign( 
        dispatch, 
        reinterpret_cast<float*>(result->values()),
        reinterpret_cast<const float*>(rhs->values()),
        mask,
        rhs->size()
    );
}

void VirtualMachine::execute_float_texture()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int s = argument();
    int t = argument();
    REYES_ASSERT( renderer_ );
    float_texture( *renderer_, result, registers_[texturename], registers_[s], registers_[t] );
}

void VirtualMachine::execute_vec3_texture()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int s = argument();
    int t = argument();
    REYES_ASSERT( renderer_ );
    vec3_texture( *renderer_, result, registers_[texturename], registers_[s], registers_[t] );
}

void VirtualMachine::execute_float_environment()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int direction = argument();
    REYES_ASSERT( renderer_ );
    float_environment( *renderer_, result, registers_[texturename], registers_[direction] );
}

void VirtualMachine::execute_vec3_environment()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int direction = argument();
    REYES_ASSERT( renderer_ );
    vec3_environment( *renderer_, result, registers_[texturename], registers_[direction] );
}

void VirtualMachine::execute_shadow()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int position = argument();
    int bias = argument();
    REYES_ASSERT( renderer_ );
    shadow( *renderer_, result, registers_[texturename], registers_[position], registers_[bias] );
}

void VirtualMachine::execute_call_0()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    shared_ptr<Symbol> symbol = shader_->symbols()[argument()];
    REYES_ASSERT( symbol->function() );
    typedef void (*FunctionType)( const Renderer&, const Grid&, shared_ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result );
}

void VirtualMachine::execute_call_1()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    shared_ptr<Symbol> symbol = shader_->symbols()[argument()];
    shared_ptr<Value> arg0 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, shared_ptr<Value>, shared_ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0 );
}

void VirtualMachine::execute_call_2()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    shared_ptr<Symbol> symbol = shader_->symbols()[argument()];
    shared_ptr<Value> arg0 = registers_[argument()];
    shared_ptr<Value> arg1 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1 );
}

void VirtualMachine::execute_call_3()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    shared_ptr<Symbol> symbol = shader_->symbols()[argument()];
    shared_ptr<Value> arg0 = registers_[argument()];
    shared_ptr<Value> arg1 = registers_[argument()];
    shared_ptr<Value> arg2 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1, arg2 );
}

void VirtualMachine::execute_call_4()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    shared_ptr<Symbol> symbol = shader_->symbols()[argument()];
    shared_ptr<Value> arg0 = registers_[argument()];
    shared_ptr<Value> arg1 = registers_[argument()];
    shared_ptr<Value> arg2 = registers_[argument()];
    shared_ptr<Value> arg3 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1, arg2, arg3 );
}

void VirtualMachine::execute_call_5()
{
    word();
    shared_ptr<Value> result = registers_[allocate_register()];
    shared_ptr<Symbol> symbol = shader_->symbols()[argument()];
    shared_ptr<Value> arg0 = registers_[argument()];
    shared_ptr<Value> arg1 = registers_[argument()];
    shared_ptr<Value> arg2 = registers_[argument()];
    shared_ptr<Value> arg3 = registers_[argument()];
    shared_ptr<Value> arg4 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value>, shared_ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1, arg2, arg3, arg4 );
}

void VirtualMachine::execute_ambient()
{
    word();

    shared_ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();
    
    shared_ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();
    
    shared_ptr<Light> light( new Light(LIGHT_AMBIENT, light_color, light_opacity, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f) );
    grid_->add_light( light );                
}

void VirtualMachine::execute_solar_axis_angle()
{
    word();

    shared_ptr<Value> axis = registers_[argument()];
    shared_ptr<Value> angle = registers_[argument()];
    
    shared_ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();
    
    shared_ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();
    
    shared_ptr<Light> light( new Light(LIGHT_SOLAR_AXIS_ANGLE, light_color, light_opacity, axis->vec3_value(), axis->vec3_value(), angle->float_value()) );
    grid_->add_light( light );                
}

void VirtualMachine::execute_illuminate()
{
    word();

    shared_ptr<Value> P = registers_[argument()];
    shared_ptr<Value> Ps = registers_[argument()];
    shared_ptr<Value> L = registers_[argument()];
    L->light_to_surface_vector( Ps, P->vec3_value() );

    shared_ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();

    shared_ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();

    shared_ptr<Light> light( new Light(LIGHT_ILLUMINATE, light_color, light_opacity, P->vec3_value(), vec3(0.0f, 0.0f, 0.0f), 0.0f) );
    grid_->add_light( light );
}

void VirtualMachine::execute_illuminate_axis_angle()
{
    word();

    shared_ptr<Value> P = registers_[argument()];
    shared_ptr<Value> axis = registers_[argument()];
    shared_ptr<Value> angle = registers_[argument()];                
    shared_ptr<Value> Ps = registers_[argument()];
    shared_ptr<Value> L = registers_[argument()];
    L->light_to_surface_vector( Ps, P->vec3_value() );

    shared_ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();

    shared_ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();

    shared_ptr<Light> light( new Light(LIGHT_ILLUMINATE_AXIS_ANGLE, light_color, light_opacity, P->vec3_value(), axis->vec3_value(), angle->float_value()) );
    grid_->add_light( light );
}

void VirtualMachine::execute_illuminance_axis_angle()
{
    word();
    shared_ptr<Value> P = registers_[argument()];
    shared_ptr<Value> axis = registers_[argument()];
    shared_ptr<Value> angle = registers_[argument()];
    shared_ptr<Value> L = registers_[argument()];
    shared_ptr<Value> light_color = registers_[argument()];
    shared_ptr<Value> light_opacity = registers_[argument()];                
    shared_ptr<Value> result = registers_[allocate_register()];
    const Light* light = grid_->get_light( light_index_ );                
    result->illuminance_axis_angle( P, axis, angle, light );
    L->surface_to_light_vector( P, light );
    light_color->reset( TYPE_COLOR, STORAGE_VARYING, grid_->size() );
    assign( DISPATCH_V3V3, (float*) light_color->values(), (const float*) light->color()->values(), nullptr, grid_->size() );
    light_opacity->reset( TYPE_COLOR, STORAGE_VARYING, grid_->size() );
    assign( DISPATCH_V3V3, (float*) light_opacity->values(), (const float*) light->opacity()->values(), nullptr, grid_->size() );
}


void VirtualMachine::float_texture( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> s, std::shared_ptr<Value> t ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( texturename->type() == TYPE_STRING );
    REYES_ASSERT( s );
    REYES_ASSERT( s->type() == TYPE_FLOAT );
    REYES_ASSERT( s->storage() == STORAGE_VARYING );
    REYES_ASSERT( t );
    REYES_ASSERT( t->type() == TYPE_FLOAT );
    REYES_ASSERT( t->storage() == STORAGE_VARYING );
    REYES_ASSERT( s->size() == t->size() );

    result->reset( TYPE_FLOAT, STORAGE_VARYING, s->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const float* s_values = s->float_values();    
        const float* t_values = t->float_values();
        float* values = result->float_values();
        for ( unsigned int i = 0; i < s->size(); ++i )
        {
            values[i] = texture->color( s_values[i], t_values[i] ).x;
        }
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::vec3_texture( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> s, std::shared_ptr<Value> t ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( texturename->type() == TYPE_STRING );
    REYES_ASSERT( s );
    REYES_ASSERT( s->type() == TYPE_FLOAT );
    REYES_ASSERT( s->storage() == STORAGE_VARYING );
    REYES_ASSERT( t );
    REYES_ASSERT( t->type() == TYPE_FLOAT );
    REYES_ASSERT( t->storage() == STORAGE_VARYING );
    REYES_ASSERT( s->size() == t->size() );

    result->reset( TYPE_COLOR, STORAGE_VARYING, s->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const float* s_values = s->float_values();    
        const float* t_values = t->float_values();    
        vec3* values = result->vec3_values();
        for ( unsigned int i = 0; i < s->size(); ++i )
        {
            values[i] = vec3( texture->color(s_values[i], t_values[i]) );
        }    
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::float_environment( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> direction ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( texturename->type() == TYPE_STRING );
    REYES_ASSERT( direction );
    REYES_ASSERT( direction->storage() == STORAGE_VARYING  );

    result->reset( TYPE_FLOAT, STORAGE_VARYING, direction->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const vec3* directions = direction->vec3_values();    
        float* values = result->float_values();
        for ( unsigned int i = 0; i < direction->size(); ++i )
        {
            values[i] = texture->environment( math::normalize(directions[i]) ).x;
        }
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::vec3_environment( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> direction ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( texturename->type() == TYPE_STRING );
    REYES_ASSERT( direction );
    REYES_ASSERT( direction->storage() == STORAGE_VARYING  );

    result->reset( TYPE_COLOR, STORAGE_VARYING, direction->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const vec3* directions = direction->vec3_values();    
        vec3* values = result->vec3_values();
        for ( unsigned int i = 0; i < direction->size(); ++i )
        {
            values[i] = vec3( texture->environment(math::normalize(directions[i])) );
        }
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::shadow( const Renderer& renderer, std::shared_ptr<Value> result, std::shared_ptr<Value> texturename, std::shared_ptr<Value> position, std::shared_ptr<Value> bias ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( texturename->type() == TYPE_STRING );
    REYES_ASSERT( position );
    REYES_ASSERT( position->type() == TYPE_POINT );
    REYES_ASSERT( position->storage() == STORAGE_VARYING );
    REYES_ASSERT( bias );
    REYES_ASSERT( bias->type() == TYPE_FLOAT );

    result->reset( TYPE_FLOAT, STORAGE_VARYING, position->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const mat4x4 world = inverse( renderer.camera_transform() );
        const float bias_value = *bias->float_values();
        const vec3* position_values = position->vec3_values();
        float* values = result->float_values();
        for ( unsigned int i = 0; i < position->size(); ++i )
        {
            values[i] = texture->shadow( world * vec4(position_values[i], 1.0f), bias_value );
        }
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::push_mask( std::shared_ptr<Value> value )
{
    REYES_ASSERT( value );
    REYES_ASSERT( value->type() == TYPE_INTEGER );

    if ( masks_.empty() )
    {
        REYES_ASSERT( masks_.capacity() > masks_.size() );
        masks_.push_back( ConditionMask() );
        masks_.back().generate( value );
    }
    else
    {    
        REYES_ASSERT( masks_.capacity() > masks_.size() );
        ConditionMask& existing_mask = masks_.back();
        masks_.push_back( ConditionMask() );
        masks_.back().generate( existing_mask, value );
    }
}

void VirtualMachine::pop_mask()
{
    REYES_ASSERT( !masks_.empty() );
    masks_.pop_back();
}

void VirtualMachine::invert_mask()
{
    REYES_ASSERT( !masks_.empty() );
    masks_.back().invert();
}

/**
// Does the condition mask specify that no values need to be processed?
//
// @return
//  True if the condition mask specifies that no values need to be processed
//  otherwise false.
*/
bool VirtualMachine::mask_empty() const
{
    REYES_ASSERT( !masks_.empty() );
    return masks_.back().empty();
}

const unsigned char* VirtualMachine::get_mask() const
{
    return !masks_.empty() ? &masks_.back().mask_[0] : NULL;
}

void VirtualMachine::reset_register( int index )
{
    REYES_ASSERT( shader_ );
    REYES_ASSERT( index >= shader_->permanent_registers() );
    register_index_ = index;
}

int VirtualMachine::allocate_register()
{
    return register_index_++;
}
