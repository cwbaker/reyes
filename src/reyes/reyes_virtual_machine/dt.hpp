#ifndef SWEET_FX_DT_HPP_INCLUDED
#define SWEET_FX_DT_HPP_INCLUDED

namespace sweet
{

namespace fx
{

class VirtualMachine;

void dt_u3_u3( VirtualMachine* virtual_machine, float** arguments, int length );
void dt_v3_v3( VirtualMachine* virtual_machine, float** arguments, int length );
void dt_u1( VirtualMachine* virtual_machine, float** arguments, int length );

}

}

#endif
