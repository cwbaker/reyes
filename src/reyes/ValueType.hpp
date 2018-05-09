#ifndef REYES_VALUETYPE_HPP_INCLUDED
#define REYES_VALUETYPE_HPP_INCLUDED

namespace sweet
{

namespace reyes
{

/**
// The type of a shader value or geometry attribute.
*/
enum ValueType
{
    TYPE_NULL, ///< Invalid or unspecified type.
    TYPE_INTEGER, ///< Used to store a conditional mask.
    TYPE_FLOAT, ///< 32 bit floating point number.
    TYPE_COLOR, ///< Triple of floats representing an RGB color.
    TYPE_POINT, ///< Triple of floats representing a position.
    TYPE_VECTOR, ///< Triple of floats representing a direction.
    TYPE_NORMAL, ///< Triple of floats representing a unit direction.
    TYPE_MATRIX, ///< Sixteentuple of floats representing a matrix.
    TYPE_STRING, ///< A string.
    TYPE_COUNT
};

}

}

#endif
