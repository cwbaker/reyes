//
// AddSymbolHelper.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "AddSymbolHelper.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "assert.hpp"

using namespace sweet;
using namespace sweet::reyes;

AddSymbolHelper::AddSymbolHelper( SymbolTable* symbol_table )
: symbol_table_( symbol_table ),
  symbol_()
{   
    REYES_ASSERT( symbol_table_ );
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );    
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2, std::shared_ptr<Value> a3), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2, std::shared_ptr<Value> a3, std::shared_ptr<Value> a4), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );

    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2, std::shared_ptr<Value> a3, std::shared_ptr<Value> a4, std::shared_ptr<Value> a5), ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( type );
    symbol_->set_storage( storage );
    symbol_->set_function( reinterpret_cast<void*>(function) );
    return *this;    
}

AddSymbolHelper& AddSymbolHelper::operator()( ValueType type, ValueStorage storage )
{
    REYES_ASSERT( symbol_ );
    symbol_->add_parameter( type, storage );
    return *this;
}

AddSymbolHelper& AddSymbolHelper::operator()( const char* identifier, float value )
{
    REYES_ASSERT( symbol_table_ );
    
    symbol_ = symbol_table_->add_symbol( identifier );
    symbol_->set_type( TYPE_FLOAT );
    symbol_->set_storage( STORAGE_CONSTANT );
    symbol_->set_value( value );
    return *this;
}
