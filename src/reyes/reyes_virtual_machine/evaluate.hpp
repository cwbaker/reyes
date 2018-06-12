#ifndef SWEET_FX_EVALUATE_HPP_INCLUDED
#define SWEET_FX_EVALUATE_HPP_INCLUDED

namespace sweet
{
    
namespace fx
{

class ProgramCurve;

void evaluate( int dispatch, float* result, const float* parameter, int length, const ProgramCurve* curve );
    
}

}

#endif
