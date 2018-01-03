//
// ErrorPolicy.cpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "ErrorPolicy.hpp"
#include "Error.hpp"
#include "ErrorAction.hpp"
#include <sweet/assert/assert.hpp>
#include <stdio.h>

using namespace sweet::render;

ErrorPolicy::ErrorPolicy()
: stream_( stderr ),
  actions_( ERROR_ACTION_IGNORE ),
  errors_( 0 ),
  total_errors_( 0 )
{
    ErrorPolicy::stream( stderr );
    ErrorPolicy::actions( ERROR_ACTION_THROW | ERROR_ACTION_PRINT );
}

ErrorPolicy::ErrorPolicy( int actions, void* stream )
: stream_( stderr ),
  actions_( ERROR_ACTION_IGNORE ),
  errors_( 0 ),
  total_errors_( 0 )
{
    ErrorPolicy::stream( stream );
    ErrorPolicy::actions( actions );
}

ErrorPolicy::~ErrorPolicy()
{
}

int ErrorPolicy::errors() const
{
    return errors_;
}

int ErrorPolicy::total_errors() const
{
    return total_errors_;
}

void ErrorPolicy::clear()
{
    errors_ = 0;
}

void ErrorPolicy::stream( void* stream )
{
    stream_ = stream ? stream : stderr;
}

void ErrorPolicy::actions( int actions )
{
    actions_ = actions;
}

void ErrorPolicy::error( int error, const char* format, ... )
{
    SWEET_ASSERT( format );

    ++errors_;
    ++total_errors_;

    if ( actions_ )
    {
        va_list args;
        va_start( args, format );
        char message [1024];
        vsnprintf( message, sizeof(message), format, args );
        message[sizeof(message) - 1] = 0;
        va_end( args );
        
        if ( actions_ & ERROR_ACTION_PRINT )
        {
            fprintf( reinterpret_cast<FILE*>(stream_), "render: %s.\n", message );
        }
        
        if ( actions_ & ERROR_ACTION_THROW )
        {
            switch ( error )
            {
                case RENDER_ERROR_OPENING_FILE_FAILED:
                    SWEET_ERROR( OpeningFileFailedError(message) );
                    break;
                    
                case RENDER_ERROR_READING_FILE_FAILED:
                    SWEET_ERROR( ReadingFileFailedError(message) );
                    break;
                    
                case RENDER_ERROR_UNDEFINED_SYMBOL:
                    SWEET_ERROR( UndefinedSymbolError(message) );
                    break;
                    
                case RENDER_ERROR_PARSING_FAILED:
                    SWEET_ERROR( ParsingFailedError(message) );
                    break;
                    
                case RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED:
                    SWEET_ERROR( SemanticAnalysisFailedError(message) );
                    break;
                    
                case RENDER_ERROR_CODE_GENERATION_FAILED:
                    SWEET_ERROR( CodeGenerationFailedError(message) );
                    break;
                    
                case RENDER_ERROR_OUT_OF_MEMORY:
                    SWEET_ERROR( OutOfMemoryError(message) );
                    break;
                    
                case RENDER_ERROR_UNKNOWN_COLOR_SPACE:
                    SWEET_ERROR( UnknownColorSpaceError(message) );
                    break;
                    
                case RENDER_ERROR_INVALID_DISPLAY_MODE:
                    SWEET_ERROR( InvalidDisplayModeError(message) );
                    break;

                case RENDER_ERROR_SYNTAX_ERROR:
                case RENDER_ERROR_SEMANTIC_ERROR:
                case RENDER_ERROR_CODE_GENERATION_ERROR:
                    break;

                case RENDER_ERROR_NONE:
                default:
                    SWEET_ASSERT( false );
                    break;
            }
        }
    }
}
