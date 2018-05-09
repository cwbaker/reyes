#ifndef REYES_SYMBOL_HPP_INCLUDED
#define REYES_SYMBOL_HPP_INCLUDED

#include "declspec.hpp"
#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include "SymbolParameter.hpp"
#include <string>
#include <vector>

namespace sweet
{

namespace render
{

/**
// A symbol recognized by the shader parser.
*/
class REYES_DECLSPEC Symbol
{
    std::string identifier_; ///< The identifier of the symbol.
    ValueType type_; ///< The type of the symbol.
    ValueStorage storage_; ///< The storage of the symbol.
    int elements_; ///< The number of elements in the symbol.
    int index_; ///< The index of the symbol in the symbols assigned to a shader.
    int register_index_; ///< The register index of the symbol in the registers assigned to a shader.
    float value_; ///< The floating point value of this symbol.
    void* function_; ///< The function value of this symbol (or null if this symbol isn't a function).
    std::vector<SymbolParameter> parameters_; ///< The parameter types of this symbol.
    
public:
    Symbol();
    Symbol( const std::string& identifier );

    const std::string& identifier() const;
    ValueType type() const;
    ValueStorage storage() const;
    int elements() const;
    int index() const;
    int register_index() const;
    float value() const;
    void* function() const;
    bool operator<( const Symbol& symbol ) const;
    bool matches_return( ValueType type, ValueStorage storage ) const;

    void set_type( ValueType type );
    void set_storage( ValueStorage storage );
    void set_elements( int elements );
    void set_index( int index );
    void set_register_index( int register_index );
    void set_value( float value );
    void set_function( void* function );
    
    void add_parameter( ValueType type, ValueStorage storage );
    const std::vector<SymbolParameter>& parameters() const;
};

}

}

#endif
