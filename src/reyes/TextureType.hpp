#pragma once

namespace reyes
{

/**
// The type of a texture.
*/
enum TextureType
{
    TEXTURE_NULL, ///< Null texture type.
    TEXTURE_COLOR, ///< A standard 2d color texture.
    TEXTURE_SHADOW, ///< A shadow map created from a depth buffer.
    TEXTURE_LATLONG_ENVIRONMENT, ///< A lat-long environment map.
    TEXTURE_CUBIC_ENVIRONMENT, ///< A cube environment map.
    TEXTURE_COUNT
};

}
