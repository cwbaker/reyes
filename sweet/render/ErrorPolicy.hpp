//
// ErrorPolicy.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_ERRORPOLICY_HPP_INCLUDED
#define SWEET_RENDER_ERRORPOLICY_HPP_INCLUDED

#include "declspec.hpp"

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
    void* stream_; ///< The stream to report errors to (or null to report to stderr).
    int actions_; ///< The actions to take when an error is reported.
    int errors_; ///< The number of errors that have occured since the last clear.
    int total_errors_; ///< The total number of errors that have occured.

public:
    ErrorPolicy();
    ErrorPolicy( int actions, void* stream );
    ~ErrorPolicy();

    int errors() const;
    int total_errors() const;

    void clear();
    void stream( void* stream );
    void actions( int actions );
    void error( int error, const char* format, ... );
};

}

}

#endif
