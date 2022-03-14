#pragma once

namespace reyes
{

/**
// The type of a Light (defined by whether it was created through assignment 
// to 'Cl' and/or 'Ol', a "solar" statement, or an "illuminate" statement.
*/
enum LightType
{
    LIGHT_NULL,
    LIGHT_AMBIENT,
    LIGHT_SOLAR_AXIS,
    LIGHT_SOLAR_AXIS_ANGLE,
    LIGHT_ILLUMINATE,
    LIGHT_ILLUMINATE_AXIS_ANGLE,
    LIGHT_COUNT
};

}
