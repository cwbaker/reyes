
#include "illuminance_solar.hpp"
#include "Dispatch.hpp"
#include <math/vec3.ipp>
#include <reyes/assert.hpp>

namespace reyes
{

void illuminance_solar_u3u3u1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int /*length*/ )
{
    mask[0] = dot( axis[0], normalize(-light_direction[0]) ) >= cosf( angle[0] );
}

void illuminance_solar_u3u3v1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[0], normalize(-light_direction[0]) ) >= cosf( angle[i] );
    }
}

void illuminance_solar_u3v3u1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(-light_direction[0]) ) >= cosf( angle[0] );
    }
}

void illuminance_solar_u3v3v1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(-light_direction[0]) ) >= cosf( angle[i] );
    }
}

void illuminance_solar_v3u3u1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[0], normalize(-light_direction[0]) ) >= cosf( angle[0] );
    }
}

void illuminance_solar_v3u3v1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[0], normalize(-light_direction[0]) ) >= cosf( angle[i] );
    }
}

void illuminance_solar_v3v3u1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(-light_direction[0]) ) >= cosf( angle[0] );
    }
}

void illuminance_solar_v3v3v1( int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    for ( int i = 0; i < length; ++i )
    {
        mask[i] = dot( axis[i], normalize(-light_direction[0]) ) >= cosf( angle[i] );
    }
}

void illuminance_solar( int dispatch, int* mask, const math::vec3* light_direction, const math::vec3* axis, const float* angle, int length )
{
    switch ( dispatch )
    {
        case DISPATCH_U3U3U1:
            illuminance_solar_u3u3u1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_U3U3V1:
            illuminance_solar_u3u3v1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_U3V3U1:
            illuminance_solar_u3v3u1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_U3V3V1:
            illuminance_solar_u3v3v1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_V3U3U1:
            illuminance_solar_v3u3u1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_V3U3V1:
            illuminance_solar_v3u3v1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_V3V3U1:
            illuminance_solar_v3v3u1( mask, light_direction, axis, angle, length );
            break;

        case DISPATCH_V3V3V1:
            illuminance_solar_v3v3v1( mask, light_direction, axis, angle, length );
            break;

        default:
            REYES_ASSERT( false );
            break;
    }
}

}
