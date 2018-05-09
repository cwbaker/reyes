#ifndef REYES_VALUESTORAGE_HPP_INCLUDED
#define REYES_VALUESTORAGE_HPP_INCLUDED

namespace reyes
{

/**
// Whether a value is constant, uniform, or varying over a surface.
//
// Greater values take precedence over lesser values when two different values
// are applied in a binary operation.  That is the result of a binary 
// operation on values with different storage values is the highest storage 
// value.
//
// For example if a constant value is multiplied by a uniform value then the 
// result will be a uniform value.  If a uniform value is added to a varying 
// value then the result will be a varying value.
//
// Make sure that this enumeration stays in an order that expresses that 
// precedence or fix the code that depends on it!
*/
enum ValueStorage
{
    STORAGE_NULL, ///< Invalid or unspecified storage.
    STORAGE_CONSTANT, ///< Constant storage has one value per grid.
    STORAGE_UNIFORM, ///< Uniform storage has one value per grid.
    STORAGE_VARYING, ///< Varying storage has one value per grid vertex.
    STORAGE_COUNT
};

}

#endif
