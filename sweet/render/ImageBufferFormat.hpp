//
// ImageBufferFormat.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_IMAGEBUFFERFORMAT_HPP_INCLUDED
#define SWEET_RENDER_IMAGEBUFFERFORMAT_HPP_INCLUDED

namespace sweet
{

namespace render
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

}

#endif
