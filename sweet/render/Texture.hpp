//
// Texture.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_TEXTURE_HPP_INCLUDED
#define SWEET_RENDER_TEXTURE_HPP_INCLUDED

#include "declspec.hpp"
#include "TextureType.hpp"
#include <sweet/math/vec4.hpp>
#include <sweet/math/mat4x4.hpp>
#include <string>

namespace sweet
{

namespace render
{

class ImageBuffer;

/**
// A color map, shadow map, or environment map texture.
*/
class SWEET_RENDER_DECLSPEC Texture
{
    TextureType type_; ///< The type of texture.
    math::mat4x4 camera_transform_; ///< The camera transform in effect when a shadow map was created.
    math::mat4x4 screen_transform_; ///< The screen transform in effect when a shadow map was created.
    ImageBuffer* image_buffers_; ///< The image buffers that store texture data for this texture.

public:
    Texture();
    Texture( TextureType type, const math::mat4x4& camera_transform, const math::mat4x4& screen_transform );
    Texture( const std::string& filename, TextureType type = TEXTURE_COLOR );
    ~Texture();
    
    TextureType type() const;
    ImageBuffer* image_buffers() const;
    bool valid() const;
    
    math::vec4 color( float s, float t ) const;
    math::vec4 environment( const math::vec3& direction ) const;
    float shadow( const math::vec4& P, float bias ) const;
    
private:
    void load( const std::string& filename, TextureType type );
};

}

}

#endif