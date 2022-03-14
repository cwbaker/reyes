//
// Encoder.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Encoder.hpp"
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <reyes/reyes_virtual_machine/Dispatch.hpp>
#include "assert.hpp"

using namespace reyes;

Encoder::Encoder()
: code_()
{
}

size_t Encoder::size() const
{
    return code_.size();
}

const std::vector<unsigned char>& Encoder::code() const
{
	return code_;
}

void Encoder::clear()
{
    code_.clear();
}

void Encoder::instruction( int instruction )
{
    REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction < INSTRUCTION_COUNT );
    quad( instruction );
}

void Encoder::instruction( int instruction, ValueType type, ValueStorage storage )
{
    REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction < INSTRUCTION_COUNT );
    byte( instruction );
    byte( dispatch(type, storage) );
    byte( 0 );
    byte( 0 );
}

void Encoder::instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1 )
{
    REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction < INSTRUCTION_COUNT );
    byte( instruction );
    byte( dispatch(type1, storage1) );
    byte( dispatch(type0, storage0) );
    byte( 0 );
}

void Encoder::instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1, ValueType type2, ValueStorage storage2 )
{
    REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction < INSTRUCTION_COUNT );
    byte( instruction );
    byte( dispatch(type0, storage0) );
    byte( dispatch(type1, storage1) );
    byte( dispatch(type2, storage2) );
}

void Encoder::argument( int argument )
{
    quad( argument );
}

void Encoder::byte( int value )
{
    REYES_ASSERT( value >= 0 && value < 256 );
    int size = code_.size();
    code_.resize( code_.size() + sizeof(unsigned char) );
    *reinterpret_cast<unsigned char*>( &code_[size] ) = value;
}

void Encoder::word( int value )
{
    REYES_ASSERT( value >= -32768 && value < 32768 );
    int size = code_.size();
    code_.resize( code_.size() + sizeof(short) );
    *reinterpret_cast<short*>( &code_[size] ) = value;
}

void Encoder::quad( int value )
{
    int size = code_.size();
    code_.resize( code_.size() + sizeof(int) );
    *reinterpret_cast<int*>( &code_[size] ) = value;
}

void Encoder::patch_argument( int address, int distance )
{
    REYES_ASSERT( address >= 0 && address < int(code_.size()) );
    int* argument = reinterpret_cast<int*>( &code_[address] );
    REYES_ASSERT( *argument == 0 );
    *argument = distance;
}

int Encoder::argument_for_patching()
{
    int address = code_.size();
    argument( 0 );
    return address;
}

int Encoder::address()
{
    return code_.size();
}

unsigned int Encoder::dispatch( ValueType type, ValueStorage storage, int shift ) const
{
    unsigned int dispatch_by_type = 0;
    switch ( type )
    {
        case TYPE_INTEGER:
        case TYPE_FLOAT:
            dispatch_by_type = 0;
            break;

        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
            dispatch_by_type = 2;
            break;

        case TYPE_MATRIX:
            dispatch_by_type = 15;
            break;

        case TYPE_STRING:
            dispatch_by_type = 0;
            break;

        default:
            REYES_ASSERT( false );
            dispatch_by_type = 0;
            break;
    }

    unsigned int dispatch_by_storage = storage == STORAGE_VARYING ? DISPATCH_VARYING : DISPATCH_UNIFORM;
    return (dispatch_by_storage | dispatch_by_type) << shift;
}

unsigned int Encoder::dispatch( ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1 ) const
{
    return 
        dispatch( type0, storage0, 8 ) |
        dispatch( type1, storage1, 0 )
    ;
}

unsigned int Encoder::dispatch( ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1, ValueType type2, ValueStorage storage2 ) const
{
    return 
        dispatch( type0, storage0, 16 ) |
        dispatch( type1, storage1, 8 ) |
        dispatch( type2, storage2, 0 )
    ;
}
