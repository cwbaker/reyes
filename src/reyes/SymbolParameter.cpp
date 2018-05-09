//
// SymbolParameter.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "SymbolParameter.hpp"

using namespace sweet::reyes;

SymbolParameter::SymbolParameter( ValueType type, ValueStorage storage )
: type_( type ),
  storage_( storage )
{
}

ValueType SymbolParameter::type() const
{
    return type_;
}

ValueStorage SymbolParameter::storage() const
{
    return storage_;
}

/**
// Can this SymbolParameter be passed a value of \e type and \e storage?
//
// Checks to see whether a value of \e type and \e storage could be validly
// passed as a parameter matching this SymbolParameter.  This allows for both
// type conversion and storage promotion so that floats can be converted to 
// color, point, vector, normal, or matrix; point, vector, and normal types 
// are considered equivalent; and lesser storages are promoted to greater 
// ones (constant to uniform or uniform to varying).
//
// The main use of this is to provide overloaded functions to the shader
// that can accept uniform or varying parameters and have their matched symbol
// providing the correct return type and storage for use further up the
// syntax tree.  For example the sin() function is equally able to operate on
// uniform and varying data but it is important to capture the fact that the
// storage of its return value will match the storage of its input parameter.
//
// @param type
//  The type of a value to check for matching with this SymbolParameter.
//
// @param storage
//  The storage of a value to check for matching with this SymbolParameter.
//
// @return
//  True if this SymbolParameter could be passed a value of \e type and 
//  \e storage otherwise false.
*/
bool SymbolParameter::matches( ValueType type, ValueStorage storage ) const
{
    bool type_matches = type == type_
        || (type >= TYPE_POINT && type <= TYPE_NORMAL && type_ >= TYPE_POINT && type_ <= TYPE_NORMAL)
        || (type == TYPE_FLOAT && type_ >= TYPE_FLOAT && type_ <= TYPE_MATRIX)
    ;
    
    bool storage_matches = storage <= storage_ 
        && storage != STORAGE_NULL
    ;

    return type_matches && storage_matches;
}
