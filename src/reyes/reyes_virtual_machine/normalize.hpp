#ifndef SWEET_FX_NORMALIZE_HPP_INCLUDED
#define SWEET_FX_NORMALIZE_HPP_INCLUDED

namespace sweet
{
    
namespace fx
{

class VirtualMachine;

void normalize_u3_u3( VirtualMachine* virtual_machine, float** arguments, int length );
void normalize_v3_v3( VirtualMachine* virtual_machine, float** arguments, int length );
    
}

}

#endif
