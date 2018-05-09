//
// Texture.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Texture.hpp"
#include "ImageBuffer.hpp"
#include "ImageBufferFormat.hpp"
#include "ErrorCode.hpp"
#include "ErrorPolicy.hpp"
#include <math/mat4x4.ipp>
#include <math/scalar.ipp>
#include <jpeg/jpeglib.h>
#include "assert.hpp"
#include <string>
#include <stdio.h>
#include <memory.h>
#define _USE_MATH_DEFINES
#include <math.h>

using std::string;
using namespace math;
using namespace reyes;

#if defined(BUILD_OS_WINDOWS)
#define snprintf _snprintf
#endif

namespace
{

enum CubeMapFaceIndex
{
    CUBE_LEFT_NX,
    CUBE_FRONT_PZ,
    CUBE_RIGHT_PX,
    CUBE_BACK_NZ,
    CUBE_BOTTOM_NY,
    CUBE_TOP_PY
};

}

Texture::Texture()
: type_( TEXTURE_NULL ),
  camera_transform_( identity() ),
  screen_transform_( identity() ),
  image_buffers_( NULL )
{
}

Texture::Texture( TextureType type, const math::mat4x4& camera_transform, const math::mat4x4& screen_transform )
: type_( type ),
  camera_transform_( camera_transform ),
  screen_transform_( screen_transform ),
  image_buffers_( NULL )
{
    REYES_ASSERT( type_ >= TEXTURE_NULL && type_ < TEXTURE_COUNT );
    image_buffers_ = new ImageBuffer [1];
}

Texture::Texture( const std::string& filename, TextureType type, ErrorPolicy* error_policy )
: type_( TEXTURE_NULL ),
  camera_transform_( identity() ),
  screen_transform_( identity() ),
  image_buffers_( NULL )
{
    load( filename, type, error_policy );
}

Texture::~Texture()
{
    delete[] image_buffers_;
    image_buffers_ = NULL;
}

TextureType Texture::type() const
{
    return type_;
}

ImageBuffer* Texture::image_buffers() const
{
    return image_buffers_;
}

bool Texture::valid() const
{
    return image_buffers_ && image_buffers_->width() > 0 && image_buffers_->height() > 0;
}

math::vec4 Texture::color( float s, float t ) const
{
    s = clamp( s, 0.0f, 1.0f );
    t = clamp( t, 0.0f, 1.0f );
    const unsigned char* pixel = image_buffers_->u8_data( s, t );
    return vec4(
        float(*(pixel + 0)) / 255.0f,
        float(*(pixel + 1)) / 255.0f,
        float(*(pixel + 2)) / 255.0f,
        1.0f
    );
}

math::vec4 Texture::environment( const math::vec3& direction ) const
{
    if ( type_ == TEXTURE_CUBIC_ENVIRONMENT )
    {
        int image = 0;
        float s = 0.0f;
        float t = 0.0f;     

        if ( fabsf(direction.x) >= fabsf(direction.y) && fabsf(direction.x) >= fabsf(direction.z) )
        {
            image = direction.x >= 0.0f ? CUBE_RIGHT_PX : CUBE_LEFT_NX;
            s = -sign(direction.x) * direction.z / fabsf(direction.x);
            t = direction.y / fabsf(direction.x);
        }
        else if ( fabsf(direction.y) >= fabsf(direction.x) && fabsf(direction.y) >= fabsf(direction.z) )
        {
            // @todo
            //  Work out why the selection between top (+ive y) and bottom
            //  (-ive y) cube maps works opposite to what you would expect in
            //  that the negative map is choosen when y is positive.
            image = direction.y >= 0.0f ? CUBE_BOTTOM_NY : CUBE_TOP_PY;
            s = direction.x / fabsf(direction.y);
            t = -sign(direction.y) * direction.z / fabsf(direction.y);
        }
        else
        {
            image = direction.z >= 0.0f ? CUBE_FRONT_PZ : CUBE_BACK_NZ;
            s = sign(direction.z) * direction.x / fabsf(direction.z);
            t = direction.y / fabsf(direction.z);
        }

        s = clamp( (s + 1.0f) / 2.0f, 0.0f, 1.0f );
        t = clamp( (t + 1.0f) / 2.0f, 0.0f, 1.0f );
        const unsigned char* pixel = image_buffers_[image].u8_data( s, t );
        return vec4(
            float(*(pixel + 0)) / 255.0f,
            float(*(pixel + 1)) / 255.0f,
            float(*(pixel + 2)) / 255.0f,
            1.0f
        );
    }
    else    
    {
        float longitude = atan2f( direction.y, direction.x );
        float latitude = asinf( direction.z );
        float s = (longitude + float(M_PI)) / (2.0f * float(M_PI));
        float t = (latitude + 0.5f * float(M_PI)) / float(M_PI);
        return color( s, t );
    }
}

float Texture::shadow( const math::vec4& P, float bias ) const
{
    vec4 xx = screen_transform_ * camera_transform_ * vec4( vec3(P), 1.0f );
    float s = clamp( xx.x / (2.0f * xx.w) + 0.5f, 0.0f, 1.0f );
    float t = clamp( 1.0f - (xx.y / (2.0f * xx.w) + 0.5f), 0.0f, 1.0f );
    const float* pixel = reinterpret_cast<float*>( image_buffers_->f32_data(s, t) );
    return xx.w <= *pixel + bias ? 1.0f : 0.0f;
}

void Texture::load( const std::string& filename, TextureType type, ErrorPolicy* error_policy )
{
    REYES_ASSERT( !filename.empty() );
    REYES_ASSERT( error_policy );
    REYES_ASSERT( type > TEXTURE_NULL && type < TEXTURE_COUNT );
    
    type_ = type;
    size_t extension_begin = filename.rfind( '.' );
    if ( extension_begin != string::npos )
    {
        string extension = filename.substr( extension_begin );
        if ( type_ == TEXTURE_CUBIC_ENVIRONMENT )
        {
            const int FACES = 6;
            const char* FILENAME_BY_FACE [FACES] = { "nx", "pz", "px", "nz", "ny", "py" };

            image_buffers_ = new ImageBuffer [FACES];
            for ( int i = 0; i < FACES; ++i )
            {
                static const int MAXIMUM_FILENAME_LENGTH = 1024;
                char buffer [MAXIMUM_FILENAME_LENGTH];
                snprintf( buffer, sizeof(buffer), filename.c_str(), FILENAME_BY_FACE[i] );
                buffer[sizeof(buffer) - 1] = 0;
                                
                if ( extension == ".jpeg" || extension == ".jpg" )
                {
                    image_buffers_[i].load_jpeg( buffer );
                }
                else if ( extension == ".png" )
                {
                    image_buffers_[i].load_png( buffer );
                }
            }
        }
        else
        {
            REYES_ASSERT( !image_buffers_ );
            image_buffers_ = new ImageBuffer [1];
            if ( extension == ".jpeg" || extension == ".jpg" )
            {
                image_buffers_->load_jpeg( filename.c_str() );
            }
            else if ( extension == ".png" )
            {
                image_buffers_->load_png( filename.c_str() );
            }
            else
            {
                if ( error_policy )
                {                    
                    error_policy->error( RENDER_ERROR_OPENING_FILE_FAILED, "Unrecognized file type '%s' when loading a texture from '%s'", extension.c_str(), filename.c_str() );
                }
                return;
            }
        }
    }
}
