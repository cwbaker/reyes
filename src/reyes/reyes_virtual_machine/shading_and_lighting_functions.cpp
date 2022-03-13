//
// shading_and_lighting_functions.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <reyes/Grid.hpp>
#include <reyes/Texture.hpp>
#include <reyes/Value.hpp>
#include <reyes/VirtualMachine.hpp>
#include <reyes/Light.hpp>
#include <reyes/Renderer.hpp>
#include <reyes/assert.hpp>
#include <math/scalar.ipp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <algorithm>
#include <string.h>

using std::min;
using std::max;
using std::vector;
using std::shared_ptr;
using namespace math;

namespace reyes
{

void ambient( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    math::vec3* color = reinterpret_cast<math::vec3*>( arguments[0] );

    memset( color, 0, sizeof(vec3) * grid.size() );
    const vector<shared_ptr<Light>>& lights = grid.lights();
    for ( vector<shared_ptr<Light>>::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        REYES_ASSERT( light );        
        if ( light->type() == LIGHT_AMBIENT )
        {
            const math::vec3* light_color = light->color();
            REYES_ASSERT( light_color );
            for ( int i = 0; i < grid.size(); ++i )
            {
                color[i] += light_color[i];
            }
        }
    }
}

void diffuse( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    math::vec3* color = reinterpret_cast<math::vec3*>( arguments[0] );
    const math::vec3* position = grid.vec3_value( "P" );
    const math::vec3* normal = reinterpret_cast<const math::vec3*>( arguments[1] );

    memset( color, 0, sizeof(vec3) * grid.size() );
    const vector<std::shared_ptr<Light>>& lights = grid.lights();
    for ( vector<std::shared_ptr<Light>>::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        REYES_ASSERT( light );
        
        if ( light->type() != LIGHT_AMBIENT )
        {
            const math::vec3* light_color = light->color();
            REYES_ASSERT( light_color );
            
            const int size = grid.size();

            switch ( light->type() )
            {
                case LIGHT_SOLAR_AXIS:
                case LIGHT_SOLAR_AXIS_ANGLE:
                {
                    const vec3& light_direction = -light->position();
                    if ( light->angle() == 0.0f )
                    {
                        for ( int i = 0; i < size; ++i )
                        {
                            const vec3& L = light_direction;
                            const vec3& N = normal[i];
                            if ( dot(N, L) >= 0.0f )
                            {
                                const vec3& Cl = light_color[i];
                                color[i] +=  Cl * dot( N, normalize(L) );
                            }                    
                        }
                    }
                    else
                    {
                        const vec3& light_axis = light->axis();
                        const float light_angle_cosine = cosf( light->angle() );                    
                        for ( int i = 0; i < size; ++i )
                        {
                            const vec3& L = light_direction;
                            const vec3& N = normal[i];
                            if ( dot(N, L) >= 0.0f && dot(light_axis, -N) >= light_angle_cosine )
                            {
                                const vec3& Cl = light_color[i];
                                color[i] +=  Cl * dot( N, normalize(L) );
                            }                    
                        }
                    }
                    break;
                }
                    
                case LIGHT_ILLUMINATE:
                {
                    const vec3& light_position = light->position();
                    for ( int i = 0; i < size; ++i )
                    {
                        const vec3 L = normalize( light_position - position[i] );
                        const vec3& N = normal[i];
                        if ( dot(N, L) >= 0.0f )
                        {
                            const vec3& Cl = light_color[i];
                            color[i] +=  Cl * dot( N, normalize(L) );
                        }                    
                    }
                    break;
                }
                
                case LIGHT_ILLUMINATE_AXIS_ANGLE:
                {
                    const vec3& light_position = light->position();
                    const vec3& light_axis = light->axis();
                    const float light_angle_cosine = cosf( light->angle() );
                    for ( int i = 0; i < size; ++i )
                    {
                        const vec3 L = normalize( light_position - position[i] );
                        const vec3& N = normal[i];
                        if ( dot(N, L) >= 0.0f && dot(light_axis, -L) >= light_angle_cosine )
                        {
                            const vec3& Cl = light_color[i];
                            color[i] +=  Cl * dot( N, L );
                        }                    
                    }
                    break;
                }
                
                default:
                    break;
            }
        }
    }    
}

void specular( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    vec3* color = reinterpret_cast<vec3*>( arguments[0] );
    const vec3* position = grid.vec3_value( "P" );
    const vec3* normal = reinterpret_cast<const vec3*>( arguments[1] );
    const vec3* view = reinterpret_cast<const vec3*>( arguments[2] );
    const float* roughness = reinterpret_cast<const float*>( arguments[3] );

    memset( color, 0, sizeof(vec3) * grid.size() );    
    const vector<std::shared_ptr<Light>>& lights = grid.lights();
    for ( vector<std::shared_ptr<Light>>::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        REYES_ASSERT( light );
        
        const vec3* light_color = light->color();
        const int size = grid.size();
        const float gloss = 1.0f / roughness[0];

        switch ( light->type() )
        {
            case LIGHT_SOLAR_AXIS:
            case LIGHT_SOLAR_AXIS_ANGLE:
            {
                const vec3& light_direction = -light->position();
                if ( light->angle() == 0.0f )
                {
                    for ( int i = 0; i < size; ++i )
                    {
                        const vec3& L = light_direction;
                        const vec3& N = normal[i];
                        if ( dot(N, L) >= 0.0f )
                        {
                            const vec3& Cl = light_color[i];
                            const vec3& V = view[i];
                            vec3 H = normalize( L + V );
                            color[i] += Cl * powf( std::max(0.0f, dot(N, H)), gloss );
                        }                    
                    }
                }
                else
                {
                    const vec3& light_axis = light->axis();
                    const float light_angle_cosine = cosf( light->angle() );                    
                    for ( int i = 0; i < size; ++i )
                    {
                        const vec3& L = light_direction;
                        const vec3& N = normal[i];
                        if ( dot(N, L) >= 0.0f && dot(light_axis, -N) >= light_angle_cosine )
                        {
                            const vec3& Cl = light_color[i];
                            const vec3& V = view[i];
                            vec3 H = normalize( L + V );
                            color[i] += Cl * powf( std::max(0.0f, dot(N, H)), gloss );
                        }                    
                    }
                }
                break;
            }
            
            case LIGHT_ILLUMINATE:  
            {
                const vec3& light_position = light->position();
                for ( int i = 0; i < size; ++i )
                {
                    const vec3 L = normalize( light_position - position[i] );
                    const vec3& N = normal[i];
                    if ( dot(N, L) >= 0.0f )
                    {
                        const vec3& Cl = light_color[i];
                        const vec3& V = view[i];
                        vec3 H = normalize( L + V );
                        color[i] += Cl * powf( std::max(0.0f, dot(N, H)), gloss );
                    }                    
                }
                break;
            }
                
            case LIGHT_ILLUMINATE_AXIS_ANGLE:
            {
                const vec3& light_position = light->position();
                const vec3& light_axis = light->axis();
                const float light_angle_cosine = cosf( light->angle() );
                for ( int i = 0; i < size; ++i )
                {
                    const vec3 L = normalize( light_position - position[i] );
                    const vec3& N = normal[i];
                    if ( dot(N, L) >= 0.0f && dot(light_axis, -L) >= light_angle_cosine )
                    {
                        const vec3& Cl = light_color[i];
                        const vec3& V = view[i];
                        vec3 H = normalize( L + V );
                        color[i] += Cl * powf( std::max(0.0f, dot(N, H)), gloss );
                    }                    
                }
                break;
            }
            
            default:    
                break;
        }        
    }
}

void specularbrdf( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    math::vec3* color = reinterpret_cast<math::vec3*>( arguments[0] );
    const math::vec3* light_direction = reinterpret_cast<const math::vec3*>( arguments[1] );
    const math::vec3* normal = reinterpret_cast<const math::vec3*>( arguments[2] );
    const math::vec3* view = reinterpret_cast<const math::vec3*>( arguments[3] );
    const float* roughness = reinterpret_cast<const float*>( arguments[4] );
    const float gloss = 1.0f / roughness[5];
    const int size = grid.size();

    memset( color, 0, sizeof(vec3) * grid.size() );    
    for ( int i = 0; i < size; ++i )
    {
        float alpha = powf( max(0.0f, dot(normal[i], normalize(light_direction[i] + view[i]))), gloss );
        color[i] = vec3( alpha, alpha, alpha );
    }
}

void phong( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    math::vec3* color = reinterpret_cast<math::vec3*>( arguments[0] );
    const math::vec3* position = grid.vec3_value( "P" );
    const math::vec3* normal = reinterpret_cast<const math::vec3*>( arguments[1] );
    const math::vec3* view = reinterpret_cast<const math::vec3*>( arguments[2] );
    const float* power = reinterpret_cast<const float*>( arguments[3] );

    memset( color, 0, sizeof(vec3) * grid.size() );
    const vector<std::shared_ptr<Light>>& lights = grid.lights();
    for ( vector<std::shared_ptr<Light>>::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        REYES_ASSERT( light );
        
        const vec3* light_color = light->color();
        const float power_ = power[0];
        const int size = grid.size();

        switch ( light->type() )
        {
            case LIGHT_SOLAR_AXIS:
            case LIGHT_SOLAR_AXIS_ANGLE:
            {
                const vec3& light_direction = -light->position();
                if ( light->angle() == 0.0f )
                {
                    const vec3 L = normalize( light_direction );
                    for ( int i = 0; i < size; ++i )
                    {
                        const vec3 N = normalize( normal[i] );
                        if ( dot(N, L) >= 0.0f )
                        {
                            const vec3& Cl = light_color[i];
                            const vec3& V = view[i];
                            const vec3 R = -V - 2.0f * dot(-V, N) * N;
                            color[i] += Cl * powf( max(0.0f, dot(R, L)), power_ );
                        }                    
                    }
                }
                else
                {
                    const vec3 L = normalize( light_direction );
                    const vec3& light_axis = light->axis();
                    const float light_angle_cosine = cosf( light->angle() );                    
                    for ( int i = 0; i < size; ++i )
                    {
                        const vec3 N = normalize( normal[i] );
                        if ( dot(N, L) >= 0.0f && dot(light_axis, -N) >= light_angle_cosine )
                        {
                            const vec3& Cl = light_color[i];
                            const vec3& V = view[i];
                            const vec3 R = -V - 2.0f * dot(-V, N) * N;
                            color[i] += Cl * powf( max(0.0f, dot(R, L)), power_ );
                        }                    
                    }
                }
                break;
            }
            
            case LIGHT_ILLUMINATE:  
            {
                const vec3& light_position = light->position();
                for ( int i = 0; i < size; ++i )
                {
                    const vec3 L = normalize( light_position - position[i] );
                    const vec3 N = normalize( normal[i] );
                    if ( dot(N, L) >= 0.0f )
                    {
                        const vec3& Cl = light_color[i];
                        const vec3& V = view[i];
                        const vec3 R = -V - 2.0f * dot(-V, N) * N;
                        color[i] += Cl * powf( max(0.0f, dot(R, L)), power_ );
                    }                    
                }
                break;
            }
                
            case LIGHT_ILLUMINATE_AXIS_ANGLE:
            {
                const vec3& light_position = light->position();
                const vec3& light_axis = light->axis();
                const float light_angle_cosine = cosf( light->angle() );
                for ( int i = 0; i < size; ++i )
                {
                    const vec3 L = normalize( light_position - position[i] );
                    const vec3 N = normalize( normal[i] );
                    if ( dot(N, L) >= 0.0f && dot(light_axis, -L) >= light_angle_cosine )
                    {
                        const vec3& Cl = light_color[i];
                        const vec3& V = view[i];
                        const vec3 R = -V - 2.0f * dot(-V, N) * N;
                        color[i] += Cl * powf( max(0.0f, dot(R, L)), power_ );
                    }                    
                }
                break;
            }
            
            default:    
                break;
        }        
    }
}

void trace( const Renderer& /*renderer*/, const Grid& grid, int /*dispatch*/, void** arguments )
{
    math::vec3* color = reinterpret_cast<math::vec3*>( arguments[0] );
    // const math::vec3* point = reinterpret_cast<const math::vec3*>( arguments[1] );
    // const math::vec3* reflection = reinterpret_cast<const math::vec3*>( arguments[2] );
    memset( color, 0, sizeof(vec3) * grid.size() );
}

}
