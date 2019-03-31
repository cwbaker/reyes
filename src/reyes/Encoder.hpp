#ifndef _REYES_ENCODER_HPP_
#define _REYES_ENCODER_HPP_

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
    void instruction( int instruction, int type, int storage );
    void instruction( int instruction, int type, int storage, int other_type, int other_storage );
    void argument( int argument );
    void byte( int value );
    void word( int value );
    void quad( int value );
    void patch_argument( int address, int distance );
    int argument_for_patching();
    int address();

private:
    unsigned int dispatch_by_type_and_storage( int type, int storage ) const;
};

}

#endif
