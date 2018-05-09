#ifndef REYES_IMAGEBUFFERFORMAT_HPP_INCLUDED
#define REYES_IMAGEBUFFERFORMAT_HPP_INCLUDED

namespace reyes
{

/**
// The format of elements stored in an ImageBuffer.
*/
enum ImageBufferFormat
{
    FORMAT_U8, ///< Each element is an 8 bit unsigned char.
    FORMAT_F32, ///< Each element is a 32 bit float.
    FORMAT_COUNT
};

}

#endif
