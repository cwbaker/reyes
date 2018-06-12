//
// dt.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "dt.hpp"
#include "VirtualMachine.hpp"

namespace sweet
{
    
namespace fx
{

void dt_u3_u3( VirtualMachine* virtual_machine, float** arguments, int /*length*/ )
{
    float delta_time = virtual_machine->delta_time();
    float* results = arguments[0];
    float* values = arguments[1];
    results[0] = values[0] * delta_time;
    results[1] = values[1] * delta_time;
    results[2] = values[2] * delta_time;
}

void dt_v3_v3( VirtualMachine* virtual_machine, float** arguments, int length )
{
    float delta_time = virtual_machine->delta_time();
    float* results = arguments[0];
    float* values = arguments[1];
    for ( int i = 0; i < length; ++i )
    {
        results[i * 3 + 0] = values[i * 3 + 0] * delta_time;
        results[i * 3 + 1] = values[i * 3 + 1] * delta_time;
        results[i * 3 + 2] = values[i * 3 + 2] * delta_time;
    }
}

void dt_u1( VirtualMachine* virtual_machine, float** arguments, int /*length*/ )
{
    float delta_time = virtual_machine->delta_time();
    float* results = arguments[0];
    results[0] = delta_time;
}
  
}

}
