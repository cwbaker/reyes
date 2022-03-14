#pragma once

#include "ValueType.hpp"
#include "ValueStorage.hpp"
#include <vector>
#include <stddef.h>

namespace reyes
{

class Encoder
{
    std::vector<unsigned char> code_; ///< The byte code generated for the shader.

public:
	Encoder();
    size_t size() const;
	const std::vector<unsigned char>& code() const;
    void clear();
    void instruction( int instruction );
    void instruction( int instruction, ValueType type, ValueStorage storage );
    void instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1 );
    void instruction( int instruction, ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1, ValueType type2, ValueStorage storage2 );
    void argument( int argument );
    void byte( int value );
    void word( int value );
    void quad( int value );
    void patch_argument( int address, int distance );
    int argument_for_patching();
    int address();

private:
    unsigned int dispatch( ValueType type, ValueStorage storage, int shift = 0 ) const;
    unsigned int dispatch( ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1 ) const;
    unsigned int dispatch( ValueType type0, ValueStorage storage0, ValueType type1, ValueStorage storage1, ValueType type2, ValueStorage storage2 ) const;
};

}
