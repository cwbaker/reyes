//
// Error.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_ERROR_HPP_INCLUDED
#define SWEET_RENDER_ERROR_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/error/Error.hpp>
#include <sweet/error/ErrorTemplate.hpp>

namespace sweet
{

namespace render
{

/**
// Error codes for exceptions thrown from the %persist library.
*/
enum ErrorCodes
{
    RENDER_ERROR_NONE, ///< No error has occured.
    RENDER_ERROR_OPENING_FILE_FAILED, ///< Opening a file failed.
    RENDER_ERROR_READING_FILE_FAILED, ///< Reading a file failed.
    RENDER_ERROR_UNDEFINED_SYMBOL, ///< An undefined symbol was found when parsing a shader.
    RENDER_ERROR_SYNTAX_ERROR, ///< A syntax error occured while parsing a shader.
    RENDER_ERROR_PARSING_FAILED, ///< Parsing a file failed.    
    RENDER_ERROR_SEMANTIC_ERROR, ///< A semantic error occured while analysing a shader.
    RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED, ///< Semantic analysis failed.
    RENDER_ERROR_CODE_GENERATION_ERROR, ///< An error occured while generating code.
    RENDER_ERROR_CODE_GENERATION_FAILED, ///< Code generation failed.
    RENDER_ERROR_OUT_OF_MEMORY, ///< A memory allocation failed.
    RENDER_ERROR_UNKNOWN_COLOR_SPACE, ///< An unknown color space was passed to ctransform() or used in a typecast expression.
    RENDER_ERROR_INVALID_DISPLAY_MODE, ///< A display mode was requested for a device or file format that doesn't support it.
};

/**
// Errors thrown from the %render library.
*/
class SWEET_RENDER_DECLSPEC Error : public error::Error 
{
    public:
        Error( int error );
        Error( int error, const char* format, va_list& args );
};

/**
// Opening a file failed.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_OPENING_FILE_FAILED, Error> OpeningFileFailedError;

/**
// Reading a file failed.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_READING_FILE_FAILED, Error> ReadingFileFailedError;

/**
// An undefined symbol was found when interpreting a shader.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_UNDEFINED_SYMBOL, Error> UndefinedSymbolError;

/**
// A syntax error occured while parsing a shader.
*/
typedef error::ErrorTemplate<RENDER_ERROR_SYNTAX_ERROR, Error> SyntaxError;

/**
// Parsing a file failed.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_PARSING_FAILED, Error> ParsingFailedError;

/**
// A semantic error occured while analysing a shader.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_SEMANTIC_ERROR, Error> SemanticError;

/**
// Semantic analysis failed.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED, Error> SemanticAnalysisFailedError;

/**
// An error occured while generating code.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_CODE_GENERATION_ERROR, Error> CodeGenerationError;

/**
// Code generation failed.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_CODE_GENERATION_FAILED, Error> CodeGenerationFailedError;

/**
// A memory allocation failed.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_OUT_OF_MEMORY, Error> OutOfMemoryError;

/**
// An unknown color space was passed to ctransform() or used in a typecast expression.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_UNKNOWN_COLOR_SPACE, Error> UnknownColorSpaceError;

/**
// A display mode was requested for a device or file format that doesn't support it.
// @relates Error
*/
typedef error::ErrorTemplate<RENDER_ERROR_INVALID_DISPLAY_MODE, Error> InvalidDisplayModeError;

}

}

#endif