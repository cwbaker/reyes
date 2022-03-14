//
// Symbol.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Symbol.hpp"
#include "Scope.hpp"
#include "assert.hpp"
#include <assert.h>

using std::string;
using namespace reyes;

Symbol::Symbol()
: identifier_()
, type_( TYPE_NULL )
, storage_( STORAGE_NULL )
, elements_( 0 )
, index_( 0 )
, address_()
, value_( 0.0f )
, function_( nullptr )
, parameters_()
{
}

Symbol::Symbol( const std::string& identifier, int index )
: identifier_( identifier )
, type_( TYPE_NULL )
, storage_( STORAGE_NULL )
, elements_( 0 )
, index_( index )
, address_()
, value_( 0.0f )
, function_( nullptr )
, parameters_()
{
    REYES_ASSERT( index_ >= 0 );
}

const std::string& Symbol::identifier() const
{
    return identifier_;
}

ValueType Symbol::type() const
{
    return type_;
}

ValueStorage Symbol::storage() const
{
    return storage_;
}

int Symbol::elements() const
{
    return elements_;
}

int Symbol::index() const
{
    return index_;
}

Address Symbol::address() const
{
    return address_;
}

Segment Symbol::segment() const
{
    return address_.segment();
}

int Symbol::offset() const
{
    return address_.offset();
}

float Symbol::value() const
{
    return value_;
}

void* Symbol::function() const
{
    return function_;
}

bool Symbol::operator<( const Symbol& symbol ) const
{
    return identifier_ < symbol.identifier_;
}

/**
// Does the type and storage of this Symbol match \e type and \e storage?
//
// This is used to help use typecast return types as an extra lookup when
// resolving overloaded functions.  For example 'float random()' is a 
// different function from 'point random()' and the only way to distinguish 
// between the two is to overload on return type specified by a typecast of 
// the function call expression.
//
// The expected storage is only used if a valid type is passed also otherwise
// it is assumed that the function only ever returns a uniform (or varying) 
// value or that the storage is implied by the storage of the parameters.
//
// @param type
//  The type to check this Symbol against (TYPE_NULL matches all other types).
//
// @param storage
//  The storage to check this Symbol against (STORAGE_NULL matches all other
//  storages).
//
// @return
//  Returns true if this Symbol matches \e type and \e storage otherwise 
//  false.
*/
bool Symbol::matches_return( ValueType type, ValueStorage storage ) const
{
    bool type_matches = type == TYPE_NULL
        || type_ == type
    ;
    
    bool storage_matches = storage == STORAGE_NULL
        || storage_ == storage
        || type == TYPE_NULL
    ;
    
    return type_matches && storage_matches;
}

int Symbol::size_by_type_and_storage( int maximum_varying_length ) const
{
    assert( maximum_varying_length > 0 );

    int size_by_type = 0;
    switch ( type_ )
    {
        case TYPE_NULL:
        case TYPE_STRING:
            size_by_type = 0;
            break;

        case TYPE_INTEGER:
            size_by_type = sizeof(int);
            break;

        case TYPE_FLOAT:
            size_by_type = sizeof(float);
            break;

        case TYPE_COLOR:
        case TYPE_POINT:
        case TYPE_VECTOR:
        case TYPE_NORMAL:
            size_by_type = 3 * sizeof(float);
            break;

        case TYPE_MATRIX:
            size_by_type = 16 * sizeof(float);
            break;

        default:
            assert( false );
            size_by_type = 0;
            break;
    }

    int size_by_storage = storage_ == STORAGE_VARYING ? maximum_varying_length : 1;
    return size_by_type * size_by_storage;
}

void Symbol::set_type( ValueType type )
{
    REYES_ASSERT( type >= TYPE_NULL && type < TYPE_COUNT );
    type_ = type;
}

void Symbol::set_storage( ValueStorage storage )
{
    REYES_ASSERT( storage >= STORAGE_NULL && storage < STORAGE_COUNT );
    storage_ = storage;
}

void Symbol::set_elements( int elements )
{
    REYES_ASSERT( elements > 0 );
    elements_ = elements;
}

void Symbol::set_address( Address address )
{
    address_ = address;
}

void Symbol::set_segment( Segment segment )
{
    address_.set_segment( segment );
}

void Symbol::set_value( float value )
{
    value_ = value;
}

void Symbol::set_function( void* function )
{
    function_ = function;
}

void Symbol::add_parameter( ValueType type, ValueStorage storage )
{
    parameters_.push_back( SymbolParameter(type, storage) );
}

const std::vector<SymbolParameter>& Symbol::parameters() const
{
    return parameters_;
}
