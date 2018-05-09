#ifndef REYES_DISPLAYMODE_HPP_INCLUDED
#define REYES_DISPLAYMODE_HPP_INCLUDED

namespace reyes
{

/**
// A bitmask that specifies which of "rgb", "a", and "z" is to be saved out
// when a render is written to an image file.
*/
enum DisplayMode
{
    DISPLAY_MODE_NULL = 0x00,
    DISPLAY_MODE_RGB = 0x01,
    DISPLAY_MODE_A = 0x02,
    DISPLAY_MODE_Z = 0x04
};

}

#endif
