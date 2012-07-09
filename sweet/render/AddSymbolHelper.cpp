//
// AddSymbolHelper.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "AddSymbolHelper.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"

using namespace sweet;
using namespace sweet::render;

AddSymbolHelper::AddSymbolHelper( SymbolTable* symbol_table )
: symbol_table_( symbol_table ),
  symbol_()
{   
    SWEET_ASSERT( symbol_table_ );
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );    
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2, ptr<Value> a3), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2, ptr<Value> a3, ptr<Value> a4), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2, ptr<Value> a3, ptr<Value> a4, ptr<Value> a5), ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<const void*>(function) );
    return *this;    
}

AddSymbolHelper& AddSymbolHelper::operator()( ValueType type, ValueStorage storage )
{
    SWEET_ASSERT( symbol_ );
    symbol_->add_parameter( type, storage );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, float value )
{
    SWEET_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( TYPE_FLOAT );
    symbol_->set_storage( STORAGE_CONSTANT );
    symbol_->set_value( value );
    return *this;
}
