#ifndef SWEET_RENDER_ERRORPOLICY_HPP_INCLUDED
#define SWEET_RENDER_ERRORPOLICY_HPP_INCLUDED

#include "declspec.hpp"
#include <stdarg.h>

namespace sweet
{

namespace render
{

class Error;

/**
// An interface to be implemented by objects that wish to respond to or 
// process errors encountered while rendering.
*/
class SWEET_RENDER_DECLSPEC ErrorPolicy
{
    int total_errors_; ///< The total number of errors that have occured.
    int errors_; ///< The number of errors that have occured since the last clear.

public:
    ErrorPolicy();
    int total_errors() const;
    int errors() const;
    void clear();
    void error( int error, const char* format, ... );

private:
    virtual void render_error( int error, const char* format, va_list args );
};

}

}

#endif
