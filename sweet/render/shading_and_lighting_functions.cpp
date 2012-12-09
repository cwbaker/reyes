//
// shading_and_lighting_functions.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Grid.hpp"
#include "Texture.hpp"
#include "Value.hpp"
#include "VirtualMachine.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include <sweet/math/scalar.ipp>
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/assert/assert.hpp>
#include <algorithm>

using std::min;
using std::max;
using std::vector;
using namespace sweet;
using namespace sweet::math;

namespace sweet
{

namespace render
{

void ambient( const Renderer& renderer, const Grid& grid, ptr<Value> color )
{
    SWEET_ASSERT( color );
    
    color->reset( TYPE_COLOR, STORAGE_VARYING, grid.size() );
    color->zero();

    const vector<ptr<Light> >& lights = grid.lights();
    for ( vector<ptr<Light> >::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        SWEET_ASSERT( light );
        
        if ( light->type() == LIGHT_AMBIENT )
        {
            const ptr<Value>& light_color = light->color();
            SWEET_ASSERT( light_color );
            
            vec3* color_values = color->vec3_values();
            const vec3* light_color_values = light_color->vec3_values();
            for ( unsigned int i = 0; i < color->size(); ++i )
            {
                color_values[i] += light_color_values[min(i, light_color->size() - 1)];
            }
        }
    }
}

void diffuse( const Renderer& renderer, const Grid& grid, ptr<Value> color, ptr<Value> normal )
{
    SWEET_ASSERT( color );
    SWEET_ASSERT( normal );
    SWEET_ASSERT( normal->size() == grid.size() );
    
    color->reset( TYPE_COLOR, STORAGE_VARYING, grid.size() );
    color->zero();

    ptr<Value> P = grid.find_value( "P" );
    SWEET_ASSERT( P );
    SWEET_ASSERT( P->type() == TYPE_POINT );
    SWEET_ASSERT( P->storage() == STORAGE_VARYING );
    SWEET_ASSERT( P->size() == color->size() );

    const vector<ptr<Light> >& lights = grid.lights();
    for ( vector<ptr<Light> >::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        SWEET_ASSERT( light );
        
        if ( light->type() != LIGHT_AMBIENT )
        {
            const ptr<Value>& light_color = light->color();
            SWEET_ASSERT( light_color );
            
            vec3* colors = color->vec3_values();
            const vec3* positions = P->vec3_values();
            const vec3* normals = normal->vec3_values();
            const vec3* light_colors = light_color->vec3_values();
            const int size = color->size();

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
                            const vec3& N = normals[i];
                            if ( dot(N, L) >= 0.0f )
                            {
                                const vec3& Cl = light_colors[i];
                                colors[i] +=  Cl * dot( N, normalize(L) );
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
                            const vec3& N = normals[i];
                            if ( dot(N, L) >= 0.0f && dot(light_axis, -N) >= light_angle_cosine )
                            {
                                const vec3& Cl = light_colors[i];
                                colors[i] +=  Cl * dot( N, normalize(L) );
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
                        const vec3 L = normalize( light_position - positions[i] );
                        const vec3& N = normals[i];
                        if ( dot(N, L) >= 0.0f )
                        {
                            const vec3& Cl = light_colors[i];
                            colors[i] +=  Cl * dot( N, normalize(L) );
                        }                    
                    }
                    break;
                }
                
                case LIGHT_ILLUMINATE_AXIS_ANGLE:
                {
                    const vec3& light_position = light->position();
                    const vec3& light_axis = light->axis();
                    const float light_angle_cosine = cosf( light->angle() );
                    for ( unsigned int i = 0; i < size; ++i )
                    {
                        const vec3 L = normalize( light_position - positions[i] );
                        const vec3& N = normals[i];
                        if ( dot(N, L) >= 0.0f && dot(light_axis, -L) >= light_angle_cosine )
                        {
                            const vec3& Cl = light_colors[i];
                            colors[i] +=  Cl * dot( N, L );
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

void specular( const Renderer& renderer, const Grid& grid, ptr<Value> color, ptr<Value> normal, ptr<Value> view, ptr<Value> roughness_value )
{
    SWEET_ASSERT( color );
    SWEET_ASSERT( normal );
    SWEET_ASSERT( view );
    SWEET_ASSERT( roughness_value );
    
    color->reset( TYPE_COLOR, STORAGE_VARYING, grid.size() );
    color->zero();
    
    ptr<Value> P = grid.find_value( "P" );
    SWEET_ASSERT( P );
    SWEET_ASSERT( P->type() == TYPE_POINT );
    SWEET_ASSERT( P->storage() == STORAGE_VARYING );
    SWEET_ASSERT( P->size() == color->size() );
    
    const vector<ptr<Light> >& lights = grid.lights();
    for ( vector<ptr<Light> >::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        SWEET_ASSERT( light );
        
        vec3* colors = color->vec3_values();
        const vec3* positions = P->vec3_values();
        const vec3* normals = normal->vec3_values();
        const vec3* views = view->vec3_values();
        const float roughness = roughness_value->float_value();
        const vec3* light_colors = light->color()->vec3_values();
        const int size = color->size();

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
                        const vec3& N = normals[i];
                        if ( dot(N, L) >= 0.0f )
                        {
                            const vec3& Cl = light_colors[i];
                            const vec3& V = views[i];
                            vec3 H = normalize( L + V );
                            colors[i] += Cl * powf( std::max(0.0f, dot(N, H)), 1.0f / roughness );
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
                        const vec3& N = normals[i];
                        if ( dot(N, L) >= 0.0f && dot(light_axis, -N) >= light_angle_cosine )
                        {
                            const vec3& Cl = light_colors[i];
                            const vec3& V = views[i];
                            vec3 H = normalize( L + V );
                            colors[i] += Cl * powf( std::max(0.0f, dot(N, H)), 1.0f / roughness );
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
                    const vec3 L = normalize( light_position - positions[i] );
                    const vec3& N = normals[i];
                    if ( dot(N, L) >= 0.0f )
                    {
                        const vec3& Cl = light_colors[i];
                        const vec3& V = views[i];
                        vec3 H = normalize( L + V );
                        colors[i] += Cl * powf( std::max(0.0f, dot(N, H)), 1.0f / roughness );
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
                    const vec3 L = normalize( light_position - positions[i] );
                    const vec3& N = normals[i];
                    if ( dot(N, L) >= 0.0f && dot(light_axis, -L) >= light_angle_cosine )
                    {
                        const vec3& Cl = light_colors[i];
                        const vec3& V = views[i];
                        vec3 H = normalize( L + V );
                        colors[i] += Cl * powf( std::max(0.0f, dot(N, H)), 1.0f / roughness );
                    }                    
                }
                break;
            }
            
            default:    
                break;
        }        
    }
}

void specularbrdf( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> l, ptr<Value> n, ptr<Value> v, ptr<Value> roughness_value )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( l );
    SWEET_ASSERT( n );
    SWEET_ASSERT( v );
    SWEET_ASSERT( roughness_value );
    SWEET_ASSERT( roughness_value->type() == TYPE_FLOAT );
    SWEET_ASSERT( roughness_value->storage() == STORAGE_CONSTANT || roughness_value->storage() == STORAGE_UNIFORM );
    
    const vec3* lights = l->vec3_values();
    const vec3* normals = n->vec3_values();
    const vec3* views = v->vec3_values();
    const float roughness = roughness_value->float_value();
    const int size = l->size();

    result->reset( TYPE_COLOR, STORAGE_VARYING, size );
    vec3* colors = result->vec3_values();
    
    for ( int i = 0; i < size; ++i )
    {
        float alpha = powf( max(0.0f, dot(normals[i], normalize(lights[i] + views[i]))), 1.0f / roughness );
        colors[i] = vec3( alpha, alpha, alpha );
    }
}

void phong( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> normal, ptr<Value> view, ptr<Value> power_value )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( normal );
    SWEET_ASSERT( view );
    SWEET_ASSERT( power_value );
    
    result->reset( TYPE_COLOR, STORAGE_VARYING, grid.size() );
    result->zero();
    
    ptr<Value> P = grid.find_value( "P" );
    SWEET_ASSERT( P );
    SWEET_ASSERT( P->type() == TYPE_POINT );
    SWEET_ASSERT( P->storage() == STORAGE_VARYING );
    SWEET_ASSERT( P->size() == result->size() );
    
    const vector<ptr<Light> >& lights = grid.lights();
    for ( vector<ptr<Light> >::const_iterator i = lights.begin(); i != lights.end(); ++i )
    {
        Light* light = i->get();
        SWEET_ASSERT( light );
        
        vec3* colors = result->vec3_values();
        const vec3* positions = P->vec3_values();
        const vec3* normals = normal->vec3_values();
        const vec3* views = view->vec3_values();
        const float power = power_value->float_value();
        const vec3* light_colors = light->color()->vec3_values();
        const int size = result->size();

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
                        const vec3 N = normalize( normals[i] );
                        if ( dot(N, L) >= 0.0f )
                        {
                            const vec3& Cl = light_colors[i];
                            const vec3& V = views[i];
                            const vec3 R = -V - 2.0f * dot(-V, N) * N;
                            colors[i] += Cl * powf( max(0.0f, dot(R, L)), power );
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
                        const vec3 N = normalize( normals[i] );
                        if ( dot(N, L) >= 0.0f && dot(light_axis, -N) >= light_angle_cosine )
                        {
                            const vec3& Cl = light_colors[i];
                            const vec3& V = views[i];
                            const vec3 R = -V - 2.0f * dot(-V, N) * N;
                            colors[i] += Cl * powf( max(0.0f, dot(R, L)), power );
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
                    const vec3 L = normalize( light_position - positions[i] );
                    const vec3 N = normalize( normals[i] );
                    if ( dot(N, L) >= 0.0f )
                    {
                        const vec3& Cl = light_colors[i];
                        const vec3& V = views[i];
                        const vec3 R = -V - 2.0f * dot(-V, N) * N;
                        colors[i] += Cl * powf( max(0.0f, dot(R, L)), power );
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
                    const vec3 L = normalize( light_position - positions[i] );
                    const vec3 N = normalize( normals[i] );
                    if ( dot(N, L) >= 0.0f && dot(light_axis, -L) >= light_angle_cosine )
                    {
                        const vec3& Cl = light_colors[i];
                        const vec3& V = views[i];
                        const vec3 R = -V - 2.0f * dot(-V, N) * N;
                        colors[i] += Cl * powf( max(0.0f, dot(R, L)), power );
                    }                    
                }
                break;
            }
            
            default:    
                break;
        }        
    }
}

void trace( const Renderer& renderer, const Grid& grid, ptr<Value> result, ptr<Value> point, ptr<Value> reflection )
{
    SWEET_ASSERT( result );
    result->reset( TYPE_COLOR, STORAGE_VARYING, grid.size() );
    result->zero();
}

}

}
