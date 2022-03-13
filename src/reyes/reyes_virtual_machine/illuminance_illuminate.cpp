
#include "illuminance_illuminate.hpp"
#include "Dispatch.hpp"
#include <math/vec3.ipp>
#include <reyes/assert.hpp>

namespace reyes
{

void illuminance_illuminate_u3u3u1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int /*length*/ )
{
    mask[0] = dot( axis[0], normalize(light_position[0] - position[0]) ) >= cosf( angle[0] );
}

void illuminance_illuminate_u3u3v1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[0], normalize(light_position[0] - position[0]) ) >= cosf( angle[i] );
    }
}

void illuminance_illuminate_u3v3u1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(light_position[0] - position[0]) ) >= cosf( angle[0] );
    }
}

void illuminance_illuminate_u3v3v1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(light_position[0] - position[0]) ) >= cosf( angle[i] );
    }
}

void illuminance_illuminate_v3u3u1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[0], normalize(light_position[0] - position[i]) ) >= cosf( angle[0] );
    }
}

void illuminance_illuminate_v3u3v1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[0], normalize(light_position[0] - position[i]) ) >= cosf( angle[i] );
    }
}

void illuminance_illuminate_v3v3u1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(light_position[0] - position[i]) ) >= cosf( angle[0] );
    }
}

void illuminance_illuminate_v3v3v1( int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(light_position[0] - position[i]) ) >= cosf( angle[i] );
    }
}

void illuminance_illuminate( int dispatch, int* mask, const math::vec3* light_position, const math::vec3* position, const math::vec3* axis, const float* angle, int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U3U3U1:
            illuminance_illuminate_u3u3u1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_U3U3V1:
            illuminance_illuminate_u3u3v1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_U3V3U1:
            illuminance_illuminate_u3v3u1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_U3V3V1:
            illuminance_illuminate_u3v3v1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_V3U3U1:
            illuminance_illuminate_v3u3u1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_V3U3V1:
            illuminance_illuminate_v3u3v1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_V3V3U1:
            illuminance_illuminate_v3v3u1( mask, light_position, position, axis, angle, length );
            break;

        case DISPATCH_V3V3V1:
            illuminance_illuminate_v3v3v1( mask, light_position, position, axis, angle, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
