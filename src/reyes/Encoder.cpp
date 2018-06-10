//
// Encoder.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Encoder.hpp"
#include "Instruction.hpp"
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
    word( instruction );
    word( 0 );
}

void Encoder::instruction( int instruction, int type, int storage )
{
    REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction < INSTRUCTION_COUNT );
    word( instruction );
    word( dispatch_by_type_and_storage(type, storage) );
}

void Encoder::instruction( int instruction, int type, int storage, int other_type, int other_storage )
{
    REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction < INSTRUCTION_COUNT );
    word( instruction );
    word( (dispatch_by_type_and_storage(type, storage) << 8) | dispatch_by_type_and_storage(other_type, other_storage) );
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

unsigned int Encoder::dispatch_by_type_and_storage( int type, int storage ) const
{
    unsigned int dispatch_by_type = 0;
    switch ( type )
    {
        case TYPE_INTEGER:
        case TYPE_FLOAT:
            dispatch_by_type = 1;
            break;

        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
            dispatch_by_type = 3;
            break;

        case TYPE_MATRIX:
            dispatch_by_type = 16;
            break;

        case TYPE_STRING:
            dispatch_by_type = 1;
            break;

        default:
            REYES_ASSERT( false );
            dispatch_by_type = 0;
            break;
    }

    unsigned int dispatch_by_storage = storage == STORAGE_VARYING ? DISPATCH_VARYING : DISPATCH_UNIFORM;
    return dispatch_by_type | dispatch_by_storage;
}
