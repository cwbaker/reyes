#ifndef SWEET_FX_LERP_HPP_INCLUDED
#define SWEET_FX_LERP_HPP_INCLUDED

namespace sweet
{
    
namespace fx
{

class VirtualMachine;

void lerp_v4_u4u4v1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_v3v3v1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_v3u3v1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_u3v3v1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_u3u3v1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_v3u3u1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_u3v3u1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v3_u3u3u1( VirtualMachine* virtual_machine, float** arguments, int length );
void lerp_v1_u1u1v1( VirtualMachine* virtual_machine, float** arguments, int length );
    
}

}

#endif
