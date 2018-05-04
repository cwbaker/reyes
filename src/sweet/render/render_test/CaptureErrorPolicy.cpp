//
// CaptureErrorPolicy.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "CaptureErrorPolicy.hpp"
#include <sweet/render/ErrorCode.hpp>
#include <sweet/assert/assert.hpp>

using namespace sweet::render;

CaptureErrorPolicy::CaptureErrorPolicy()
: ErrorPolicy(),
  errors(),
  messages()
{
}
    
void CaptureErrorPolicy::render_error( int error, const char* format, va_list args )
{
    SWEET_ASSERT( error >= RENDER_ERROR_NONE && error < RENDER_ERROR_COUNT );
    SWEET_ASSERT( format );
    char message [1024];
    vsnprintf( message, sizeof(message), format, args );
    message[sizeof(message) - 1] = 0;
    errors.push_back( error );
    messages.push_back( message );
}
