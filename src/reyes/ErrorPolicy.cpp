//
// ErrorPolicy.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ErrorPolicy.hpp"
#include "assert.hpp"
#include <stdio.h>
#include <stdarg.h>

using namespace reyes;

ErrorPolicy::ErrorPolicy()
: total_errors_( 0 )
, errors_( 0 )
{
}

int ErrorPolicy::total_errors() const
{
    return total_errors_;
}

int ErrorPolicy::errors() const
{
    return errors_;
}

void ErrorPolicy::clear()
{
    errors_ = 0;
}

void ErrorPolicy::error( int error, const char* format, ... )
{
    ++total_errors_;
    ++errors_;
    va_list args;
    va_start( args, format );
    render_error( error, format, args );
    va_end( args );
}

void ErrorPolicy::render_error( int /*error*/, const char* format, va_list args )
{
    REYES_ASSERT( format );
    fprintf( stderr, "render: ERROR: " );
    vfprintf( stderr, format, args );
    fprintf( stderr, "\n" );
}
