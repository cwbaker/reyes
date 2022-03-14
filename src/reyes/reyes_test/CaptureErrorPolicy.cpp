//
// CaptureErrorPolicy.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "CaptureErrorPolicy.hpp"
#include <reyes/ErrorCode.hpp>
#include <reyes/assert.hpp>

using namespace reyes;

CaptureErrorPolicy::CaptureErrorPolicy()
: reyes::ErrorPolicy()
, errors()
, messages()
{
}
    
void CaptureErrorPolicy::render_error( int error, const char* format, va_list args )
{
    REYES_ASSERT( error >= RENDER_ERROR_NONE && error < RENDER_ERROR_COUNT );
    REYES_ASSERT( format );
    char message [1024];
    vsnprintf( message, sizeof(message), format, args );
    message[sizeof(message) - 1] = 0;
    errors.push_back( error );
    messages.push_back( message );
}
