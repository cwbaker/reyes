//
// VirtualMachine.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
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
#include <sweet/math/vec4.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>

using std::max;
using std::swap;
using std::make_pair;
using std::map;
using std::string;
using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

VirtualMachine::ConditionMask::ConditionMask()
: mask_(),
  processed_( 0 )
{
}

void VirtualMachine::ConditionMask::generate( ptr<Value> value )
{
    SWEET_ASSERT( value );
    SWEET_ASSERT( value->type() == TYPE_INTEGER );
    
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

void VirtualMachine::ConditionMask::generate( const ConditionMask& condition_mask, ptr<Value> value )
{
    SWEET_ASSERT( value );
    SWEET_ASSERT( value->type() == TYPE_INTEGER );
    SWEET_ASSERT( condition_mask.mask_.size() == value->size() );
    
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

    const vector<ptr<Symbol>>& symbols = shader.symbols();
    for ( int i = 0; i < shader.parameters(); ++i )
    {
        const ptr<Symbol>& symbol = symbols[i];
        SWEET_ASSERT( symbol );
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
    SWEET_ASSERT( shader_ );
    SWEET_ASSERT( !shader_->code().empty() );
    SWEET_ASSERT( start >= 0 && start <= int(shader_->code().size()) );
    SWEET_ASSERT( finish >= 0 && finish <= int(shader_->code().size()) );
    SWEET_ASSERT( start <= finish );

    code_begin_ = &shader_->code().front() + start;
    code_end_ = &shader_->code().front() + finish;
    register_index_ = shader_->permanent_registers();
    
    values_.reserve( shader_->registers() );
    while ( values_.size() < shader_->registers() )
    {
        ptr<Value> value( new Value() );
        values_.push_back( value );
    }
    
    registers_.clear();
    registers_.reserve( shader_->registers() );
    registers_.insert( registers_.end(), max(shader_->registers() - int(registers_.size()), 0), ptr<Value>() );

    // Initialize registers for constants.
    SWEET_ASSERT( shader_->constants() == int(shader_->values().size()) );
    unsigned int register_index = 0;
    while ( register_index < shader_->values().size() )
    {
        ptr<Value>& value = values_[register_index];
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
    const map<string, ptr<Value>>& values = grid.values_by_identifier();
    for ( map<string, ptr<Value>>::const_iterator i = values.begin(); i != values.end(); ++i )
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
    SWEET_ASSERT( code_begin_ );
    SWEET_ASSERT( code_end_ );
    SWEET_ASSERT( code_begin_ <= code_end_ );
    
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
                
            case INSTRUCTION_MULTIPLY_FLOAT:
                execute_multiply_float();
                break;
                
            case INSTRUCTION_MULTIPLY_VEC3:
                execute_multiply_vec3();
                break;
                
            case INSTRUCTION_DIVIDE_FLOAT:
                execute_divide_float();
                break;
                
            case INSTRUCTION_DIVIDE_VEC3:
                execute_divide_vec3();
                break;
                
            case INSTRUCTION_ADD_FLOAT:
                execute_add_float();
                break;
            
            case INSTRUCTION_ADD_VEC3:
                execute_add_vec3();
                break;
            
            case INSTRUCTION_SUBTRACT_FLOAT:
                execute_subtract_float();
                break;
            
            case INSTRUCTION_SUBTRACT_VEC3:
                execute_subtract_vec3();
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
            
            case INSTRUCTION_EQUAL_FLOAT:
                execute_equal_float();
                break;
            
            case INSTRUCTION_EQUAL_VEC3:
                execute_equal_vec3();
                break;
            
            case INSTRUCTION_NOT_EQUAL_FLOAT:
                execute_not_equal_float();
                break;
            
            case INSTRUCTION_NOT_EQUAL_VEC3:
                execute_not_equal_vec3();
                break;
            
            case INSTRUCTION_NEGATE_FLOAT:
                execute_negate_float();
                break;
                
            case INSTRUCTION_NEGATE_VEC3:
                execute_negate_vec3();
                break;
                
            case INSTRUCTION_PROMOTE_INTEGER:
                execute_promote_integer();
                break;
            
            case INSTRUCTION_PROMOTE_FLOAT:
                execute_promote_float();
                break;
            
            case INSTRUCTION_PROMOTE_VEC3:
                execute_promote_vec3();
                break;
            
            case INSTRUCTION_FLOAT_TO_COLOR:
                execute_float_to_color();
                break;
            
            case INSTRUCTION_FLOAT_TO_POINT:
                execute_float_to_point();
                break;
            
            case INSTRUCTION_FLOAT_TO_VECTOR:
                execute_float_to_vector();
                break;
            
            case INSTRUCTION_FLOAT_TO_NORMAL:
                execute_float_to_normal();
                break;
            
            case INSTRUCTION_FLOAT_TO_MATRIX:
                execute_float_to_matrix();
                break;
            
            case INSTRUCTION_ASSIGN_FLOAT:
                execute_assign_float();
                break;
            
            case INSTRUCTION_ASSIGN_VEC3:
                execute_assign_vec3();
                break;
            
            case INSTRUCTION_ASSIGN_MAT4X4:
                execute_assign_mat4x4();
                break;
            
            case INSTRUCTION_ASSIGN_INTEGER:
                execute_assign_integer();
                break;
            
            case INSTRUCTION_ASSIGN_STRING:
                execute_assign_string();
                break;
            
            case INSTRUCTION_ADD_ASSIGN_FLOAT:
                execute_add_assign_float();
                break;
            
            case INSTRUCTION_ADD_ASSIGN_VEC3:
                execute_add_assign_vec3();
                break;
            
            case INSTRUCTION_MULTIPLY_ASSIGN_FLOAT:
                execute_multiply_assign_float();
                break;
            
            case INSTRUCTION_MULTIPLY_ASSIGN_VEC3:
                execute_multiply_assign_vec3();
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
                SWEET_ASSERT( false );
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
    SWEET_ASSERT( code_ + distance >= code_begin_ && code_ + distance < code_end_ );
    code_ += distance;
}

int VirtualMachine::instruction()
{
    SWEET_ASSERT( code_ );
    SWEET_ASSERT( code_end_ );
    SWEET_ASSERT( code_ < code_end_ );
    return *code_++;    
}

int VirtualMachine::argument()
{
    SWEET_ASSERT( code_ );
    SWEET_ASSERT( code_end_ );
    SWEET_ASSERT( code_ < code_end_ );
    return *code_++;    
}

void VirtualMachine::execute_halt()
{
    code_ = code_end_;
}

void VirtualMachine::execute_reset()
{
    int index = argument();
    reset_register( index );
}

void VirtualMachine::execute_clear_mask()
{
    pop_mask();
}

void VirtualMachine::execute_generate_mask()
{
    int mask = argument();
    push_mask( registers_[mask] );
}

void VirtualMachine::execute_invert_mask()
{
    invert_mask();
}

void VirtualMachine::execute_jump_empty()
{
    int distance = argument();
    if ( mask_empty() )
    {
        jump( distance );
        pop_mask();
    }
}

void VirtualMachine::execute_jump_not_empty()
{
    int distance = argument();
    if ( !mask_empty() )
    {
        jump( distance );
    }
}

void VirtualMachine::execute_jump_illuminance()
{
    int distance = argument();
    jump_illuminance( distance );
}

void VirtualMachine::execute_jump()
{
    int distance = argument();
    jump( distance );
}

void VirtualMachine::execute_transform()
{
    ptr<Value> result = registers_[allocate_register()];
    int fromspace = argument();
    int point = argument();
    SWEET_ASSERT( renderer_ );
    result->transform( renderer_->transform_from(registers_[fromspace]->string_value()), registers_[point] );
}

void VirtualMachine::execute_transform_vector()
{
    ptr<Value> result = registers_[allocate_register()];
    int fromspace = argument();
    int vector = argument();
    SWEET_ASSERT( renderer_ );
    result->vtransform( renderer_->transform_from(registers_[fromspace]->string_value()), registers_[vector] );
}

void VirtualMachine::execute_transform_normal()
{
    ptr<Value> result = registers_[allocate_register()];
    int fromspace = argument();
    int vector = argument();
    SWEET_ASSERT( renderer_ );
    result->ntransform( renderer_->transform_from(registers_[fromspace]->string_value()), registers_[vector] );
}

void VirtualMachine::execute_transform_color()
{
    ptr<Value> result = registers_[allocate_register()];
    int fromspace = argument();
    int color = argument();
    SWEET_ASSERT( renderer_ );
    ctransform( *renderer_, *grid_, result, registers_[fromspace], registers_[color] );
}

void VirtualMachine::execute_transform_matrix()
{
    ptr<Value> result = registers_[allocate_register()];
    int tospace = argument();
    int matrix = argument();
    SWEET_ASSERT( renderer_ );
    result->transform_matrix( renderer_->transform_to(registers_[tospace]->string_value()), registers_[matrix] );
}

void VirtualMachine::execute_dot()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->dot_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_multiply_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->multiply_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_multiply_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->multiply_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_divide_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->divide_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_divide_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->divide_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_add_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->add_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_add_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->add_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_subtract_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->subtract_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_subtract_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->subtract_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_greater()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->greater_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_greater_equal()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->greater_equal_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_less()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->less_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_less_equal()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->less_equal_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_and()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->logical_and( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_or()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->logical_or( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_equal_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->equal_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_equal_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->equal_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_not_equal_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->not_equal_float( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_not_equal_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int lhs = argument();
    int rhs = argument();
    result->not_equal_vec3( registers_[lhs], registers_[rhs] );
}

void VirtualMachine::execute_negate_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->negate_float( registers_[rhs] );
}

void VirtualMachine::execute_negate_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->negate_vec3( registers_[rhs] );
}

void VirtualMachine::execute_promote_integer()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->promote_integer( grid_->size(), registers_[rhs] );
}

void VirtualMachine::execute_promote_float()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->promote_float( grid_->size(), registers_[rhs] );
}

void VirtualMachine::execute_promote_vec3()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->promote_vec3( grid_->size(), registers_[rhs] );
}

void VirtualMachine::execute_float_to_color()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->float_to_vec3( TYPE_COLOR, registers_[rhs] );
}

void VirtualMachine::execute_float_to_point()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->float_to_vec3( TYPE_POINT, registers_[rhs] );
}

void VirtualMachine::execute_float_to_vector()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->float_to_vec3( TYPE_VECTOR, registers_[rhs] );
}

void VirtualMachine::execute_float_to_normal()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->float_to_vec3( TYPE_NORMAL, registers_[rhs] );
}

void VirtualMachine::execute_float_to_matrix()
{
    ptr<Value> result = registers_[allocate_register()];
    int rhs = argument();
    result->float_to_mat4x4( registers_[rhs] );
}

void VirtualMachine::execute_assign_float()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->assign_float( value, mask );
}

void VirtualMachine::execute_assign_vec3()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->assign_vec3( value, mask );
}

void VirtualMachine::execute_assign_mat4x4()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->assign_mat4x4( value, mask );
}

void VirtualMachine::execute_assign_integer()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->assign_integer( value, mask );
}

void VirtualMachine::execute_assign_string()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->assign_string( value, mask );
}

void VirtualMachine::execute_add_assign_float()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->add_assign_float( value, mask );
}

void VirtualMachine::execute_add_assign_vec3()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->add_assign_vec3( value, mask );
}

void VirtualMachine::execute_multiply_assign_float()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->multiply_assign_float( value, mask );
}

void VirtualMachine::execute_multiply_assign_vec3()
{
    ptr<Value> result = registers_[argument()];
    ptr<Value> value = registers_[argument()];
    const unsigned char* mask = value->storage() == STORAGE_VARYING ? get_mask() : NULL;
    result->multiply_assign_vec3( value, mask );
}

void VirtualMachine::execute_float_texture()
{
    ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int s = argument();
    int t = argument();
    SWEET_ASSERT( renderer_ );
    float_texture( *renderer_, result, registers_[texturename], registers_[s], registers_[t] );
}

void VirtualMachine::execute_vec3_texture()
{
    ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int s = argument();
    int t = argument();
    SWEET_ASSERT( renderer_ );
    vec3_texture( *renderer_, result, registers_[texturename], registers_[s], registers_[t] );
}

void VirtualMachine::execute_float_environment()
{
    ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int direction = argument();
    SWEET_ASSERT( renderer_ );
    float_environment( *renderer_, result, registers_[texturename], registers_[direction] );
}

void VirtualMachine::execute_vec3_environment()
{
    ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int direction = argument();
    SWEET_ASSERT( renderer_ );
    vec3_environment( *renderer_, result, registers_[texturename], registers_[direction] );
}

void VirtualMachine::execute_shadow()
{
    ptr<Value> result = registers_[allocate_register()];
    int texturename = argument();
    int position = argument();
    int bias = argument();
    SWEET_ASSERT( renderer_ );
    shadow( *renderer_, result, registers_[texturename], registers_[position], registers_[bias] );
}

void VirtualMachine::execute_call_0()
{
    ptr<Value> result = registers_[allocate_register()];
    ptr<Symbol> symbol = shader_->symbols()[argument()];
    SWEET_ASSERT( symbol->function() );
    typedef void (*FunctionType)( const Renderer&, const Grid&, ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    SWEET_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result );
}

void VirtualMachine::execute_call_1()
{
    ptr<Value> result = registers_[allocate_register()];
    ptr<Symbol> symbol = shader_->symbols()[argument()];
    ptr<Value> arg0 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, ptr<Value>, ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    SWEET_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0 );
}

void VirtualMachine::execute_call_2()
{
    ptr<Value> result = registers_[allocate_register()];
    ptr<Symbol> symbol = shader_->symbols()[argument()];
    ptr<Value> arg0 = registers_[argument()];
    ptr<Value> arg1 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, ptr<Value>, ptr<Value>, ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    SWEET_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1 );
}

void VirtualMachine::execute_call_3()
{
    ptr<Value> result = registers_[allocate_register()];
    ptr<Symbol> symbol = shader_->symbols()[argument()];
    ptr<Value> arg0 = registers_[argument()];
    ptr<Value> arg1 = registers_[argument()];
    ptr<Value> arg2 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, ptr<Value>, ptr<Value>, ptr<Value>, ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    SWEET_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1, arg2 );
}

void VirtualMachine::execute_call_4()
{
    ptr<Value> result = registers_[allocate_register()];
    ptr<Symbol> symbol = shader_->symbols()[argument()];
    ptr<Value> arg0 = registers_[argument()];
    ptr<Value> arg1 = registers_[argument()];
    ptr<Value> arg2 = registers_[argument()];
    ptr<Value> arg3 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, ptr<Value>, ptr<Value>, ptr<Value>, ptr<Value>, ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    SWEET_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1, arg2, arg3 );
}

void VirtualMachine::execute_call_5()
{
    ptr<Value> result = registers_[allocate_register()];
    ptr<Symbol> symbol = shader_->symbols()[argument()];
    ptr<Value> arg0 = registers_[argument()];
    ptr<Value> arg1 = registers_[argument()];
    ptr<Value> arg2 = registers_[argument()];
    ptr<Value> arg3 = registers_[argument()];
    ptr<Value> arg4 = registers_[argument()];
    typedef void (*FunctionType)( const Renderer&, const Grid&, ptr<Value>, ptr<Value>, ptr<Value>, ptr<Value>, ptr<Value>, ptr<Value> );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    SWEET_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, result, arg0, arg1, arg2, arg3, arg4 );
}

void VirtualMachine::execute_ambient()
{
    ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();
    
    ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();
    
    ptr<Light> light( new Light(LIGHT_AMBIENT, light_color, light_opacity, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f) );
    grid_->add_light( light );                
}

void VirtualMachine::execute_solar_axis_angle()
{
    ptr<Value> axis = registers_[argument()];
    ptr<Value> angle = registers_[argument()];
    
    ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();
    
    ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();
    
    ptr<Light> light( new Light(LIGHT_SOLAR_AXIS_ANGLE, light_color, light_opacity, axis->vec3_value(), axis->vec3_value(), angle->float_value()) );
    grid_->add_light( light );                
}

void VirtualMachine::execute_illuminate()
{
    ptr<Value> P = registers_[argument()];
    ptr<Value> Ps = registers_[argument()];
    ptr<Value> L = registers_[argument()];
    L->light_to_surface_vector( Ps, P->vec3_value() );

    ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();

    ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();

    ptr<Light> light( new Light(LIGHT_ILLUMINATE, light_color, light_opacity, P->vec3_value(), vec3(0.0f, 0.0f, 0.0f), 0.0f) );
    grid_->add_light( light );
}

void VirtualMachine::execute_illuminate_axis_angle()
{
    ptr<Value> P = registers_[argument()];
    ptr<Value> axis = registers_[argument()];
    ptr<Value> angle = registers_[argument()];                
    ptr<Value> Ps = registers_[argument()];
    ptr<Value> L = registers_[argument()];
    L->light_to_surface_vector( Ps, P->vec3_value() );

    ptr<Value>& light_color = registers_[argument()];
    light_color.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_color->zero();

    ptr<Value>& light_opacity = registers_[argument()];
    light_opacity.reset( new Value(TYPE_COLOR, STORAGE_VARYING, grid_->size()) );
    light_opacity->zero();

    ptr<Light> light( new Light(LIGHT_ILLUMINATE_AXIS_ANGLE, light_color, light_opacity, P->vec3_value(), axis->vec3_value(), angle->float_value()) );
    grid_->add_light( light );
}

void VirtualMachine::execute_illuminance_axis_angle()
{
    ptr<Value> P = registers_[argument()];
    ptr<Value> axis = registers_[argument()];
    ptr<Value> angle = registers_[argument()];
    ptr<Value> L = registers_[argument()];
    ptr<Value> light_color = registers_[argument()];
    ptr<Value> light_opacity = registers_[argument()];                
    ptr<Value> result = registers_[allocate_register()];
    const Light* light = grid_->get_light( light_index_ );                
    result->illuminance_axis_angle( P, axis, angle, light );
    L->surface_to_light_vector( P, light );
    light_color->assign_vec3( light->color(), NULL );
    light_opacity->assign_vec3( light->opacity(), NULL );
}


void VirtualMachine::float_texture( const Renderer& renderer, ptr<Value> result, ptr<Value> texturename, ptr<Value> s, ptr<Value> t ) const
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( texturename );
    SWEET_ASSERT( texturename->type() == TYPE_STRING );
    SWEET_ASSERT( s );
    SWEET_ASSERT( s->type() == TYPE_FLOAT );
    SWEET_ASSERT( s->storage() == STORAGE_VARYING );
    SWEET_ASSERT( t );
    SWEET_ASSERT( t->type() == TYPE_FLOAT );
    SWEET_ASSERT( t->storage() == STORAGE_VARYING );
    SWEET_ASSERT( s->size() == t->size() );

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

void VirtualMachine::vec3_texture( const Renderer& renderer, ptr<Value> result, ptr<Value> texturename, ptr<Value> s, ptr<Value> t ) const
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( texturename );
    SWEET_ASSERT( texturename->type() == TYPE_STRING );
    SWEET_ASSERT( s );
    SWEET_ASSERT( s->type() == TYPE_FLOAT );
    SWEET_ASSERT( s->storage() == STORAGE_VARYING );
    SWEET_ASSERT( t );
    SWEET_ASSERT( t->type() == TYPE_FLOAT );
    SWEET_ASSERT( t->storage() == STORAGE_VARYING );
    SWEET_ASSERT( s->size() == t->size() );

    result->reset( TYPE_COLOR, STORAGE_VARYING, s->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const float* s_values = s->float_values();    
        const float* t_values = t->float_values();    
        vec3* values = result->vec3_values();
        for ( unsigned int i = 0; i < s->size(); ++i )
        {
            values[i] = texture->color( s_values[i], t_values[i] );
        }    
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::float_environment( const Renderer& renderer, ptr<Value> result, ptr<Value> texturename, ptr<Value> direction ) const
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( texturename );
    SWEET_ASSERT( texturename->type() == TYPE_STRING );
    SWEET_ASSERT( direction );
    SWEET_ASSERT( direction->storage() == STORAGE_VARYING  );

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

void VirtualMachine::vec3_environment( const Renderer& renderer, ptr<Value> result, ptr<Value> texturename, ptr<Value> direction ) const
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( texturename );
    SWEET_ASSERT( texturename->type() == TYPE_STRING );
    SWEET_ASSERT( direction );
    SWEET_ASSERT( direction->storage() == STORAGE_VARYING  );

    result->reset( TYPE_COLOR, STORAGE_VARYING, direction->size() );

    const Texture* texture = renderer.find_texture( texturename->string_value().c_str() );
    if ( texture && texture->valid() )
    {
        const vec3* directions = direction->vec3_values();    
        vec3* values = result->vec3_values();
        for ( unsigned int i = 0; i < direction->size(); ++i )
        {
            values[i] = texture->environment( math::normalize(directions[i]) );
        }
    }
    else
    {
        result->zero();
    }
}

void VirtualMachine::shadow( const Renderer& renderer, ptr<Value> result, ptr<Value> texturename, ptr<Value> position, ptr<Value> bias ) const
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( texturename );
    SWEET_ASSERT( texturename->type() == TYPE_STRING );
    SWEET_ASSERT( position );
    SWEET_ASSERT( position->type() == TYPE_POINT );
    SWEET_ASSERT( position->storage() == STORAGE_VARYING );
    SWEET_ASSERT( bias );
    SWEET_ASSERT( bias->type() == TYPE_FLOAT );

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

void VirtualMachine::push_mask( ptr<Value> value )
{
    SWEET_ASSERT( value );
    SWEET_ASSERT( value->type() == TYPE_INTEGER );

    if ( masks_.empty() )
    {
        SWEET_ASSERT( masks_.capacity() > masks_.size() );
        masks_.push_back( ConditionMask() );
        masks_.back().generate( value );
    }
    else
    {    
        SWEET_ASSERT( masks_.capacity() > masks_.size() );
        ConditionMask& existing_mask = masks_.back();
        masks_.push_back( ConditionMask() );
        masks_.back().generate( existing_mask, value );
    }
}

void VirtualMachine::pop_mask()
{
    SWEET_ASSERT( !masks_.empty() );
    masks_.pop_back();
}

void VirtualMachine::invert_mask()
{
    SWEET_ASSERT( !masks_.empty() );
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
    SWEET_ASSERT( !masks_.empty() );
    return masks_.back().empty();
}

const unsigned char* VirtualMachine::get_mask() const
{
    return !masks_.empty() ? &masks_.back().mask_[0] : NULL;
}

void VirtualMachine::reset_register( int index )
{
    SWEET_ASSERT( shader_ );
    SWEET_ASSERT( index >= shader_->permanent_registers() );
    register_index_ = index;
}

int VirtualMachine::allocate_register()
{
    return register_index_++;
}
