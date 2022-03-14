#ifndef REYES_SYMBOL_HPP_INCLUDED
#define REYES_SYMBOL_HPP_INCLUDED

#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include "Address.hpp"
#include "SymbolParameter.hpp"
#include <string>
#include <vector>

namespace reyes
{

class Scope;

/**
// A symbol recognized by the shader parser.
*/
class Symbol
{
    std::string identifier_; ///< The identifier of the symbol.
    ValueType type_; ///< The type of the symbol.
    ValueStorage storage_; ///< The storage of the symbol.
    int elements_; ///< The number of elements in the symbol.
    int index_; ///< The index of the symbol in its scope.
    Address address_; ///< The address of this symbol.
    float value_; ///< The floating point value of this symbol.
    void* function_; ///< The function value of this symbol (or null if this symbol isn't a function).
    std::vector<SymbolParameter> parameters_; ///< The parameter types of this symbol.
    
public:
    Symbol();
    Symbol( const std::string& identifier, int index );

    const std::string& identifier() const;
    ValueType type() const;
    ValueStorage storage() const;
    int elements() const;
    int index() const;
    Address address() const;
    Segment segment() const;
    int offset() const;
    float value() const;
    void* function() const;
    bool operator<( const Symbol& symbol ) const;
    bool matches_return( ValueType type, ValueStorage storage ) const;
    int size_by_type_and_storage( int maximum_varying_length ) const;

    void set_type( ValueType type );
    void set_storage( ValueStorage storage );
    void set_elements( int elements );
    void set_address( Address address );
    void set_segment( Segment segment );
    void set_value( float value );
    void set_function( void* function );
    
    void add_parameter( ValueType type, ValueStorage storage );
    const std::vector<SymbolParameter>& parameters() const;
};

}

#endif
