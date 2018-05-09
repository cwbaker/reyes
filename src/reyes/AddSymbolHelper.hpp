#ifndef REYES_ADDSYMBOLHELPER_HPP_INCLUDED
#define REYES_ADDSYMBOLHELPER_HPP_INCLUDED

#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <vector>
#include <memory>

namespace reyes
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
class AddSymbolHelper
{
    SymbolTable* symbol_table_; ///< The SymbolTable to add Symbols to.
    std::shared_ptr<Symbol> symbol_; ///< The most recently added Symbol.

public:
    AddSymbolHelper( SymbolTable* symbol_table );
    AddSymbolHelper& operator()( const char* identifier, ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2, std::shared_ptr<Value> a3), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2, std::shared_ptr<Value> a3, std::shared_ptr<Value> a4), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, std::shared_ptr<Value> a0, std::shared_ptr<Value> a1, std::shared_ptr<Value> a2, std::shared_ptr<Value> a3, std::shared_ptr<Value> a4, std::shared_ptr<Value> a5), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, float value );
};

}

#endif
