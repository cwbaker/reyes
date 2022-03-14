#ifndef REYES_ADDSYMBOLHELPER_HPP_INCLUDED
#define REYES_ADDSYMBOLHELPER_HPP_INCLUDED

#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include "Segment.hpp"
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
    Segment segment_; ///< The Segment to add symbols to.
    std::shared_ptr<Symbol> symbol_; ///< The most recently added Symbol.

public:
    AddSymbolHelper( SymbolTable* symbol_table, Segment segment );
    AddSymbolHelper& operator()( const char* identifier, ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, void (*function)(const Renderer&, const Grid&, int, void**), ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( ValueType type, ValueStorage storage = STORAGE_VARYING );
    AddSymbolHelper& operator()( const char* identifier, float value );
    AddSymbolHelper& add_symbols( Segment segment = SEGMENT_GRID );
    AddSymbolHelper& add_default_symbols();
    AddSymbolHelper& add_surface_symbols();
    AddSymbolHelper& add_light_symbols();
    AddSymbolHelper& add_volume_symbols();
    AddSymbolHelper& add_displacement_symbols();
    AddSymbolHelper& add_imager_symbols();
    AddSymbolHelper& add_illuminance_symbols();
    AddSymbolHelper& add_lighting_symbols();
};

}

#endif
