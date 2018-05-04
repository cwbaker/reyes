#ifndef SWEET_RENDER_ERRORCODE_HPP_INCLUDED
#define SWEET_RENDER_ERRORCODE_HPP_INCLUDED

namespace sweet
{

namespace render
{

/**
// Error codes for the toy REYES/RenderMan library.
*/
enum ErrorCode
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
    RENDER_ERROR_COUNT
};

}

}

#endif
