//
// normalize.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "normalize.hpp"
#include <sweet/math/vec3.ipp>

using namespace sweet::math;

namespace sweet
{
    
namespace fx
{
    
void normalize_u3_u3( VirtualMachine* /*virtual_machine*/, float** arguments, int /*length*/ )
{
    vec3* results = (vec3*) arguments[0];
    const vec3* xx = (const vec3*) arguments[1];
    results[0] = normalize( xx[0] );
}

void normalize_v3_v3( VirtualMachine* /*virtual_machine*/, float** arguments, int length )
{
    vec3* results = (vec3*) arguments[0];
    const vec3* xx = (const vec3*) arguments[1];
    for ( int i = 0; i < length; ++i )
    {
        results[i] = normalize( xx[i] );
    }
}

}

}
