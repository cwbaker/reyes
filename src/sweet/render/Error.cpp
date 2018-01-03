//
// Error.cpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Error.hpp"

using namespace sweet::render;

Error::Error( int error )
: error::Error( error )
{
}

Error::Error( int error, const char* format, va_list& args )
: error::Error( error, format, args )
{
}
