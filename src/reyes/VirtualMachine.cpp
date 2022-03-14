//
// VirtualMachine.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "VirtualMachine.hpp"
#include "Renderer.hpp"
#include "SymbolTable.hpp"
#include "Scope.hpp"
#include "Shader.hpp"
#include "Symbol.hpp"
#include "Texture.hpp"
#include "Grid.hpp"
#include "Light.hpp"
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include <reyes/reyes_virtual_machine/color_functions.hpp>
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
#include <reyes/reyes_virtual_machine/illuminance_illuminate.hpp>
#include <reyes/reyes_virtual_machine/illuminance_solar.hpp>
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
#include <reyes/reyes_virtual_machine/ntransform.hpp>
#include <reyes/reyes_virtual_machine/ctransform.hpp>
#include <reyes/reyes_virtual_machine/mtransform.hpp>
#include <reyes/reyes_virtual_machine/Dispatch.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <math/vec4.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"
#include <algorithm>
#include <limits.h>
#include <string.h>

using std::max;
using std::swap;
using std::make_pair;
using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

VirtualMachine::VirtualMachine()
: renderer_( nullptr )
, symbol_table_( nullptr )
, grid_( nullptr )
, shader_( nullptr )
, light_index_( INT_MAX )
, length_( 0 )
, constant_memory_size_( 0 )
, constant_memory_( nullptr )
, grid_memory_size_( 0 )
, grid_memory_( nullptr )
, temporary_memory_size_( 0 )
, temporary_memory_( nullptr )
, code_begin_( nullptr )
, code_end_( nullptr )
, masks_()
, code_( nullptr )
{
    symbol_table_ = new SymbolTable;
}

VirtualMachine::VirtualMachine( const Renderer& renderer )
: renderer_( &renderer )
, symbol_table_( nullptr )
, grid_( nullptr )
, shader_( nullptr )
, light_index_( INT_MAX )
, length_( 0 )
, constant_memory_size_( 0 )
, constant_memory_( nullptr )
, grid_memory_size_( 0 )
, grid_memory_( nullptr )
, temporary_memory_size_( 0 )
, temporary_memory_( nullptr )
, code_begin_( nullptr )
, code_end_( nullptr )
, masks_()
, code_( nullptr )
{
    symbol_table_ = new SymbolTable;
}

VirtualMachine::~VirtualMachine()
{
    if ( temporary_memory_ )
    {
        free( temporary_memory_ );
        temporary_memory_ = nullptr;
    }

    delete symbol_table_;
    symbol_table_ = nullptr;
}

void VirtualMachine::initialize( Grid& grid, Shader& shader )
{
    grid_ = &grid;
    shader_ = &shader;
    construct( shader.initialize_address(), shader.shade_address() );
    execute();
    shader_ = nullptr;
    grid_ = nullptr;
}

void VirtualMachine::shade( Grid& grid, Shader& shader )
{   
    grid_ = &grid;
    shader_ = &shader;
    construct( shader.shade_address(), shader.end_address() );
    execute();
    shader_ = nullptr;
    grid_ = nullptr;
}

void VirtualMachine::construct( int start, int finish )
{
    REYES_ASSERT( grid_ );
    REYES_ASSERT( shader_ );
    REYES_ASSERT( !shader_->code().empty() );
    REYES_ASSERT( start >= 0 && start <= int(shader_->code().size()) );
    REYES_ASSERT( finish >= 0 && finish <= int(shader_->code().size()) );
    REYES_ASSERT( start <= finish );

    code_begin_ = &shader_->code().front() + start;
    code_end_ = &shader_->code().front() + finish;
    length_ = grid_->size();

    int capacity = shader_->temporary_memory_size();
    if ( capacity > temporary_memory_size_ )
    {
        if ( temporary_memory_ )
        {
            free( temporary_memory_ );
            temporary_memory_ = nullptr;
            temporary_memory_size_ = 0;
        }
        temporary_memory_ = reinterpret_cast<unsigned char*>( malloc(capacity) );
        temporary_memory_size_ = capacity;
    }

    constant_memory_size_ = shader_->constant_memory_size();
    constant_memory_ = shader_->constant( 0 );

    grid_memory_size_ = shader_->grid_memory_size();
    grid_memory_ = grid_->memory();

    // @todo
    //  The number of masks reserved implicitly limits the number of nested
    //  for, while, if, solar, and illuminate statements in the shader 
    //  language so I should make sure that limit is enforced during parsing
    //  or code generation.
    const unsigned int MASKS_RESERVE = 8;
    masks_.reserve( MASKS_RESERVE );
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
            
            case INSTRUCTION_TRANSFORM_POINT:
                execute_transform_point();
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
                
            case INSTRUCTION_CALL:
                execute_call();
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
    return byte();
}

int VirtualMachine::dispatch()
{
    return byte() | (byte() << 8) | (byte() << 16);
}

int VirtualMachine::argument()
{
    return quad();
}

int VirtualMachine::byte()
{
    REYES_ASSERT( code_ );
    REYES_ASSERT( code_end_ );
    REYES_ASSERT( code_ + 1 <= code_end_ );
    int value = *(const char*) code_;
    code_ += sizeof(char);
    return value;
}

int VirtualMachine::word()
{
    REYES_ASSERT( code_ );
    REYES_ASSERT( code_end_ );
    REYES_ASSERT( code_ + sizeof(short) <= code_end_ );
    int value = *(const short*) code_;
    code_ += sizeof(short);
    return value;
}

int VirtualMachine::quad()
{
    REYES_ASSERT( code_ );
    REYES_ASSERT( code_end_ );
    REYES_ASSERT( code_ + sizeof(int) <= code_end_ );
    int value = *(const int*) code_;
    code_ += sizeof(int);
    return value;
}

void VirtualMachine::execute_halt()
{
    code_ = code_end_;
}

void VirtualMachine::execute_clear_mask()
{
    dispatch();
    pop_mask();
}

void VirtualMachine::execute_generate_mask()
{
    dispatch();
    float* mask = lookup_float( quad() );
    push_mask( mask, length_ );
}

void VirtualMachine::execute_invert_mask()
{
    dispatch();
    invert_mask();
}

void VirtualMachine::execute_jump_empty()
{
    dispatch();
    int distance = argument();
    if ( mask_empty() )
    {
        jump( distance );
        pop_mask();
    }
}

void VirtualMachine::execute_jump_not_empty()
{
    dispatch();
    int distance = argument();
    if ( !mask_empty() )
    {
        jump( distance );
    }
}

void VirtualMachine::execute_jump_illuminance()
{
    dispatch();
    int distance = argument();
    jump_illuminance( distance );
}

void VirtualMachine::execute_jump()
{
    dispatch();
    int distance = argument();
    jump( distance );
}

void VirtualMachine::execute_transform_point()
{
    int dispatch = VirtualMachine::dispatch();
    vec3* result = lookup_vec3( quad() );
    const char* fromspace = lookup_string( quad() );
    const vec3* point = lookup_vec3( quad() );
    REYES_ASSERT( renderer_ );
    transform( 
        dispatch,
        result,
        renderer_->transform_from( fromspace ),
        point,
        length_
    );
}

void VirtualMachine::execute_transform_vector()
{
    int dispatch = VirtualMachine::dispatch();
    vec3* result = lookup_vec3( quad() );
    const char* fromspace = lookup_string( quad() );
    const vec3* vector = lookup_vec3( quad() );
    REYES_ASSERT( renderer_ );
    vtransform( 
        dispatch,
        result,
        renderer_->transform_from( fromspace ),
        vector,
        length_
    );
}

void VirtualMachine::execute_transform_normal()
{
    int dispatch = VirtualMachine::dispatch();
    vec3* result = lookup_vec3( quad() );
    const char* fromspace = lookup_string( quad() );
    const vec3* normal = lookup_vec3( quad() );
    REYES_ASSERT( renderer_ );
    ntransform( 
        dispatch,
        result,
        renderer_->transform_from( fromspace ),
        normal,
        length_
    );
}

void VirtualMachine::execute_transform_color()
{
    int dispatch = VirtualMachine::dispatch();
    vec3* result = lookup_vec3( quad() );
    const char* fromspace = lookup_string( quad() );
    const vec3* color = lookup_vec3( quad() );
    ctransform( 
        dispatch,
        result,
        fromspace,
        color,
        length_
    );
}

void VirtualMachine::execute_transform_matrix()
{
    int dispatch = VirtualMachine::dispatch();
    mat4x4* result = lookup_mat4x4( quad() );
    const char* tospace = lookup_string( quad() );
    const mat4x4* matrix = lookup_mat4x4( quad() );
    REYES_ASSERT( renderer_ );
    mtransform( 
        dispatch,
        result,
        renderer_->transform_to( tospace ),
        matrix,
        length_
    );
}

void VirtualMachine::execute_dot()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    dot( 
        dispatch, 
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_multiply()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    multiply( 
        dispatch, 
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_divide()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    divide( 
        dispatch, 
        result, 
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_add()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    add( 
        dispatch, 
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_subtract()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    subtract( 
        dispatch,
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_greater()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    greater(
        dispatch,
        result,
        lhs,
        rhs,
        length_ 
    );
}

void VirtualMachine::execute_greater_equal()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    greater_equal(
        dispatch,
        result,
        lhs,
        rhs,
        length_ 
    );
}

void VirtualMachine::execute_less()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    less(
        dispatch,
        result,
        lhs,
        rhs,
        length_ 
    );
}

void VirtualMachine::execute_less_equal()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    less_equal(
        dispatch,
        result,
        lhs,
        rhs,
        length_ 
    );
}

void VirtualMachine::execute_and()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const int* lhs = lookup_int( quad() );
    const int* rhs = lookup_int( quad() );
    logical_and( 
        dispatch,
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_or()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const int* lhs = lookup_int( quad() );
    const int* rhs = lookup_int( quad() );
    logical_or( 
        dispatch,
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_equal()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    equal(
        dispatch,
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_not_equal()
{
    int dispatch = VirtualMachine::dispatch();
    int* result = lookup_int( quad() );
    const float* lhs = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    not_equal( 
        dispatch,
        result,
        lhs,
        rhs,
        length_
    );
}

void VirtualMachine::execute_negate()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* value = lookup_float( quad() );
    negate(
        dispatch,
        result,
        value,
        length_
    );
}

void VirtualMachine::execute_convert()
{
    int dispatch = VirtualMachine::dispatch();
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

    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    convert( 
        dispatch,
        result,
        rhs,
        length_
    );
}

void VirtualMachine::execute_promote()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    promote( 
        dispatch,
        result,
        rhs,
        length_
    );
}

void VirtualMachine::execute_assign()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    const unsigned char* mask = VirtualMachine::mask( dispatch );
    assign(
        dispatch,
        result,
        rhs,
        mask,
        length_
    );
}

void VirtualMachine::execute_add_assign()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    const unsigned char* mask = VirtualMachine::mask( dispatch );
    add_assign( 
        dispatch, 
        result,
        rhs,
        mask,
        length_
    );
}

void VirtualMachine::execute_subtract_assign()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    const unsigned char* mask = VirtualMachine::mask( dispatch );
    subtract_assign( 
        dispatch, 
        result,
        rhs,
        mask,
        length_
    );
}

void VirtualMachine::execute_multiply_assign()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    const unsigned char* mask = VirtualMachine::mask( dispatch );
    multiply_assign( 
        dispatch, 
        result,
        rhs,
        mask,
        length_
    );
}

void VirtualMachine::execute_divide_assign()
{
    int dispatch = VirtualMachine::dispatch();
    float* result = lookup_float( quad() );
    const float* rhs = lookup_float( quad() );
    const unsigned char* mask = VirtualMachine::mask( dispatch );
    assert( false );
    divide_assign( 
        dispatch, 
        result,
        rhs,
        mask,
        length_
    );
}

void VirtualMachine::execute_float_texture()
{
    dispatch();
    float* result = lookup_float( quad() );
    const char* texturename = lookup_string( quad() );
    const float* s = lookup_float( quad() );
    const float* t = lookup_float( quad() );
    REYES_ASSERT( renderer_ );
    float_texture( *renderer_, result, texturename, s, t, length_ );
}

void VirtualMachine::execute_vec3_texture()
{
    dispatch();
    vec3* result = lookup_vec3( quad() );
    const char* texturename = lookup_string( quad() );
    const float* s = lookup_float( quad() );
    const float* t = lookup_float( quad() );
    REYES_ASSERT( renderer_ );
    vec3_texture( *renderer_, result, texturename, s, t, length_ );
}

void VirtualMachine::execute_float_environment()
{
    dispatch();
    float* result = lookup_float( quad() );
    const char* texturename = lookup_string( quad() );
    const vec3* direction = lookup_vec3( quad() );
    REYES_ASSERT( renderer_ );
    float_environment( *renderer_, result, texturename, direction, length_ );
}

void VirtualMachine::execute_vec3_environment()
{
    dispatch();
    vec3* result = lookup_vec3( quad() );
    const char* texturename = lookup_string( quad() );
    const vec3* direction = lookup_vec3( quad() );
    REYES_ASSERT( renderer_ );
    vec3_environment( *renderer_, result, texturename, direction, length_ );
}

void VirtualMachine::execute_shadow()
{
    dispatch();
    float* result = lookup_float( quad() );
    const char* texturename = lookup_string( quad() );
    const vec3* position = lookup_vec3( quad() );
    const float* bias = lookup_float( quad() );
    REYES_ASSERT( renderer_ );
    shadow( *renderer_, result, texturename, position, bias, length_ );
}

void VirtualMachine::execute_call()
{
    const int MAXIMUM_ARGUMENTS = 16;
    int dispatch = VirtualMachine::dispatch();
    int index = argument();
    int length = argument();
    void* arguments [MAXIMUM_ARGUMENTS + 1] = {};
    arguments[0] = lookup( argument() );
    for ( int i = 0; i < length; ++i )
    {
        arguments[i + 1] = lookup( argument() );
    }

    typedef void (*FunctionType)( const Renderer&, const Grid&, int, void** );
    const Symbol* symbol = symbol_table_->global_scope()->symbol( index );
    REYES_ASSERT( symbol );
    REYES_ASSERT( symbol->function() );
    FunctionType function = reinterpret_cast<FunctionType>( symbol->function() );
    REYES_ASSERT( renderer_ );
    (*function)( *renderer_, *grid_, dispatch, arguments );
}

void VirtualMachine::execute_ambient()
{
    int dispatch = VirtualMachine::dispatch();
    (void) dispatch;
    vec3* light_color = lookup_vec3( quad() );
    float* light_opacity = lookup_float( quad() );
    memset( light_color, 0, sizeof(vec3) * length_ );
    memset( light_opacity, 0, sizeof(float) * length_ );    
    shared_ptr<Light> light( new Light(LIGHT_AMBIENT, light_color, light_opacity, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), 0.0f) );
    grid_->add_light( light );                
}

void VirtualMachine::execute_solar_axis_angle()
{
    int dispatch = VirtualMachine::dispatch();
    (void) dispatch;

    const math::vec3* axis = lookup_vec3( quad() );
    const float* angle = lookup_float( quad() );
    math::vec3* light_color = lookup_vec3( quad() );
    float* light_opacity = lookup_float( quad() );

    memset( light_color, 0, sizeof(vec3) * length_ );
    memset( light_opacity, 0, sizeof(float) * length_ );

    shared_ptr<Light> light( new Light(LIGHT_SOLAR_AXIS_ANGLE, light_color, light_opacity, axis[0], axis[0], angle[0]) );
    grid_->add_light( light );             
}

void VirtualMachine::execute_illuminate()
{
    int dispatch = VirtualMachine::dispatch();
    (void) dispatch;

    const math::vec3* P = lookup_vec3( quad() );
    const math::vec3* Ps = lookup_vec3( quad() );
    math::vec3* L = lookup_vec3( quad() );
    math::vec3* light_color = lookup_vec3( quad() );
    float* light_opacity = lookup_float( quad() );

    vec3 light_position = P[0];
    for ( int i = 0; i < length_; ++i )
    {
        L[i] = Ps[i] - light_position;
    }

    memset( light_color, 0, sizeof(vec3) * length_ );
    memset( light_opacity, 0, sizeof(float) * length_ );

    shared_ptr<Light> light( new Light(LIGHT_ILLUMINATE, light_color, light_opacity, P[0], vec3(0.0f, 0.0f, 0.0f), 0.0f) );
    grid_->add_light( light );
}

void VirtualMachine::execute_illuminate_axis_angle()
{
    int dispatch = VirtualMachine::dispatch();
    (void) dispatch;

    const vec3* P = lookup_vec3( quad() );
    const vec3* axis = lookup_vec3( quad() );
    const float* angle = lookup_float( quad() );
    const vec3* Ps = lookup_vec3( quad() );
    vec3* L = lookup_vec3( quad() );
    vec3* light_color = lookup_vec3( quad() );
    float* light_opacity = lookup_float( quad() );

    vec3 light_position = P[0];
    for ( int i = 0; i < length_; ++i )
    {
        L[i] = Ps[i] - light_position;
    }

    memset( light_color, 0, sizeof(vec3) * length_ );
    memset( light_opacity, 0, sizeof(float) * length_ );

    shared_ptr<Light> light( new Light(LIGHT_ILLUMINATE_AXIS_ANGLE, light_color, light_opacity, P[0], axis[0], angle[0]) );
    grid_->add_light( light );
}

void VirtualMachine::execute_illuminance_axis_angle()
{
    int dispatch = VirtualMachine::dispatch();
    const vec3* P = lookup_vec3( quad() );
    const vec3* axis = lookup_vec3( quad() );
    const float* angle = lookup_float( quad() );
    vec3* L = lookup_vec3( quad() );
    vec3* light_color = lookup_vec3( quad() );
    float* light_opacity = lookup_float( quad() );
    int* mask = lookup_int( quad() );

    const Light* light = grid_->get_light( light_index_ );
    const math::vec3& light_position = light->position();
    switch ( light->type() )
    {
        case LIGHT_SOLAR_AXIS:
        case LIGHT_SOLAR_AXIS_ANGLE:
            illuminance_solar( dispatch, mask, &light_position, axis, angle, length_ );
            break;

        case LIGHT_ILLUMINATE:
        case LIGHT_ILLUMINATE_AXIS_ANGLE:
            illuminance_illuminate( dispatch, mask, &light_position, P, axis, angle, length_ );
            break;
    }
    light->surface_to_light_vector( P, L, length_ );

    assign( DISPATCH_V3V3, (float*) light_color, (const float*) light->color(), nullptr, length_ );
    assign( DISPATCH_V3V3, light_opacity, light->opacity(), nullptr, length_ );
}


void VirtualMachine::float_texture( const Renderer& renderer, float* result, const char* texturename, const float* s, const float* t, int length ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( s );
    REYES_ASSERT( t );
    REYES_ASSERT( length >= 0 );

    const Texture* texture = renderer.find_texture( texturename );
    if ( texture && texture->valid() )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = texture->color( s[i], t[i] ).x;
        }
    }
    else
    {
        memset( result, 0, sizeof(float) * length );
    }
}

void VirtualMachine::vec3_texture( const Renderer& renderer, math::vec3* result, const char* texturename, const float* s, const float* t, int length ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( s );
    REYES_ASSERT( t );
    REYES_ASSERT( length >= 0 );

    const Texture* texture = renderer.find_texture( texturename );
    if ( texture && texture->valid() )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = vec3( texture->color(s[i], t[i]) );
        }    
    }
    else
    {
        memset( result, 0, sizeof(vec3) * length );
    }
}

void VirtualMachine::float_environment( const Renderer& renderer, float* result, const char* texturename, const math::vec3* direction, int length ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( direction );
    REYES_ASSERT( length >= 0 );

    const Texture* texture = renderer.find_texture( texturename );
    if ( texture && texture->valid() )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = texture->environment( normalize(direction[i]) ).x;
        }
    }
    else
    {
        memset( result, 0, sizeof(float) * length );
    }
}

void VirtualMachine::vec3_environment( const Renderer& renderer, math::vec3* result, const char* texturename, const math::vec3* direction, int length ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( direction );
    REYES_ASSERT( length >= 0 );

    const Texture* texture = renderer.find_texture( texturename );
    if ( texture && texture->valid() )
    {
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = vec3( texture->environment(normalize(direction[i])) );
        }
    }
    else
    {
        memset( result, 0, sizeof(vec3) * length );
    }
}

void VirtualMachine::shadow( const Renderer& renderer, float* result, const char* texturename, const math::vec3* position, const float* bias, int length ) const
{
    REYES_ASSERT( result );
    REYES_ASSERT( texturename );
    REYES_ASSERT( position );
    REYES_ASSERT( bias );
    REYES_ASSERT( length >= 0 );

    const Texture* texture = renderer.find_texture( texturename );
    if ( texture && texture->valid() )
    {
        const mat4x4 world = inverse( renderer.camera_transform() );
        const float bias_ = bias[0];
        for ( unsigned int i = 0; i < length; ++i )
        {
            result[i] = texture->shadow( world * vec4(position[i], 1.0f), bias_ );
        }
    }
    else
    {
        memset( result, 0, sizeof(float) * length );
    }
}

void VirtualMachine::push_mask( const float* values, int length )
{
    REYES_ASSERT( values );
    REYES_ASSERT( length >= 0 );

    if ( masks_.empty() )
    {
        REYES_ASSERT( masks_.capacity() > masks_.size() );
        masks_.push_back( ConditionMask() );
        masks_.back().generate( values, length );
    }
    else
    {    
        REYES_ASSERT( masks_.capacity() > masks_.size() );
        ConditionMask& existing_mask = masks_.back();
        masks_.push_back( ConditionMask() );
        masks_.back().generate( existing_mask, values, length );
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

const unsigned char* VirtualMachine::mask( unsigned int dispatch ) const
{
    if ( (dispatch & DISPATCH_VARYING) != 0 && !masks_.empty() )
    {
        return &masks_.back().mask()[0];
    }
    return nullptr;
}

void* VirtualMachine::lookup( Address address )
{
    void* result = nullptr;
    switch ( address.segment() )
    {
        case SEGMENT_CONSTANT:
            result = const_cast<unsigned char*>( shader_->constant(address.offset()) );
            break;

        case SEGMENT_TEMPORARY:
            result = temporary_memory_ + address.offset();
            break;

        case SEGMENT_GRID:
            result = grid_memory_ + address.offset();
            break;

        case SEGMENT_LIGHT:
            break;

        case SEGMENT_STRING:
            result = grid_->string_value( address.offset() );
            break;

        case SEGMENT_NULL:
        default:
            break;
    }
    return result;
}

const char* VirtualMachine::lookup_string( Address address )
{
    return reinterpret_cast<const char*>( lookup(address) );
}

float* VirtualMachine::lookup_float( Address address )
{
    return reinterpret_cast<float*>( lookup(address) );
}

int* VirtualMachine::lookup_int( Address address )
{
    return reinterpret_cast<int*>( lookup_float(address) );
}

math::vec3* VirtualMachine::lookup_vec3( Address address )
{
    return reinterpret_cast<math::vec3*>( lookup_float(address) );
}

math::vec4* VirtualMachine::lookup_vec4( Address address )
{
    return reinterpret_cast<math::vec4*>( lookup_float(address) );
}

math::mat4x4* VirtualMachine::lookup_mat4x4( Address address )
{
    return reinterpret_cast<math::mat4x4*>( lookup_float(address) );
}
