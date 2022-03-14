#pragma once

#include "TextureType.hpp"
#include <math/vec4.hpp>
#include <math/mat4x4.hpp>
#include <string>

namespace reyes
{

class ErrorPolicy;
class ImageBuffer;

/**
// A color map, shadow map, or environment map texture.
*/
class Texture
{
    TextureType type_; ///< The type of texture.
    math::mat4x4 camera_transform_; ///< The camera transform in effect when a shadow map was created.
    math::mat4x4 screen_transform_; ///< The screen transform in effect when a shadow map was created.
    ImageBuffer* image_buffers_; ///< The image buffers that store texture data for this texture.

public:
    Texture();
    Texture( TextureType type, const math::mat4x4& camera_transform, const math::mat4x4& screen_transform );
    Texture( const std::string& filename, TextureType type, ErrorPolicy* error_policy );
    ~Texture();
    
    TextureType type() const;
    ImageBuffer* image_buffers() const;
    bool valid() const;
    
    math::vec4 color( float s, float t ) const;
    math::vec4 environment( const math::vec3& direction ) const;
    float shadow( const math::vec4& P, float bias ) const;
    
private:
    void load( const std::string& filename, TextureType type, ErrorPolicy* error_policy );
};

}
