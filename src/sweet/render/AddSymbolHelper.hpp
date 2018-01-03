//
// AddSymbolHelper.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_ADDSYMBOLHELPER_HPP_INCLUDED
#define SWEET_RENDER_ADDSYMBOLHELPER_HPP_INCLUDED

#include "declspec.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <sweet/pointer/ptr.hpp>
#include <vector>

namespace sweet
{

namespace render
{

class Grid;
class Symbol;
class Value;
class Renderer;
class SymbolTable;

/**
// Syntax helper to provide a convenient syntax for constructing hierarchical
// symbol tables.
*/
class SWEET_RENDER_DECLSPEC AddSymbolHelper
{
    SymbolTable* symbol_table_; ///< The SymbolTable to add Symbols to.
    ptr<Symbol> symbol_; ///< The most recently added Symbol.

public:
    AddSymbolHelper( SymbolTable* symbol_table );
    AddSymbolHelper& operator()( const char* identifier, ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2, ptr<Value> a3), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2, ptr<Value> a3, ptr<Value> a4), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, ptr<Value> a0, ptr<Value> a1, ptr<Value> a2, ptr<Value> a3, ptr<Value> a4, ptr<Value> a5), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, float value );
};

}

}

#endif