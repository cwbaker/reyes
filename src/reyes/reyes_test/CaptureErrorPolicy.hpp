#ifndef REYES_CAPTUREERRORPOLICY_HPP_INCLUDED
#define REYES_CAPTUREERRORPOLICY_HPP_INCLUDED

#include <reyes/ErrorPolicy.hpp>
#include <vector>
#include <string>

namespace sweet
{

namespace render
{
	
struct CaptureErrorPolicy : public render::ErrorPolicy
{
    std::vector<int> errors;
    std::vector<std::string> messages;
    
    CaptureErrorPolicy();    
    void render_error( int error, const char* format, va_list args );
};

}

}

#endif
