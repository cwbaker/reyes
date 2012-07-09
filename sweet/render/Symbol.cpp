//
// Symbol.cpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Symbol.hpp"
#include <sweet/assert/assert.hpp>

using std::string;
using namespace sweet;
using namespace sweet::render;

Symbol::Symbol()
: identifier_(),
  type_( TYPE_NULL ),
  storage_( STORAGE_NULL ),
  elements_( 0 ),
  index_( 0 ),
  register_index_( 0 ),
  value_( 0.0f ),
  function_( NULL ),
  parameters_()
{
}

Symbol::Symbol( const std::string& identifier )
: identifier_( identifier ),
  type_( TYPE_NULL ),
  storage_( STORAGE_NULL ),
  elements_( 0 ),
  index_( 0 ),
  register_index_( 0 ),
  value_( 0.0f ),
  function_( NULL ),
  parameters_()
{
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

int Symbol::register_index() const
{
    return register_index_;
}

float Symbol::value() const
{
    return value_;
}

const void* Symbol::function() const
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

void Symbol::set_type( ValueType type )
{
    SWEET_ASSERT( type >= TYPE_NULL && type < TYPE_COUNT );
    type_ = type;
}

void Symbol::set_storage( ValueStorage storage )
{
    SWEET_ASSERT( storage >= STORAGE_NULL && storage < STORAGE_COUNT );
    storage_ = storage;
}

void Symbol::set_elements( int elements )
{
    SWEET_ASSERT( elements > 0 );
    elements_ = elements;
}

void Symbol::set_index( int index )
{
    index_ = index;
}

void Symbol::set_register_index( int register_index )
{
    register_index_ = register_index;
}

void Symbol::set_value( float value )
{
    value_ = value;
}

void Symbol::set_function( const void* function )
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
