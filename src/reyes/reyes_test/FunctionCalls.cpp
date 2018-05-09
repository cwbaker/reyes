
#include <UnitTest++/UnitTest++.h>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Shader.hpp>
#include <reyes/ErrorCode.hpp>
#include <reyes/ErrorAction.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using namespace sweet;
using namespace sweet::reyes;

SUITE( FunctionCalls )
{
    struct CheckErrorPolicy : public reyes::ErrorPolicy
    {
        int error;
        
        CheckErrorPolicy()
        : ErrorPolicy(),
          error( RENDER_ERROR_NONE )
        {
        }
        
        void render_error( int eerror, const char* /*format*/, va_list /*args*/ )
        {            
            error = eerror;
        }
    };

    TEST( function_call_with_color_passed_instead_of_point )
    {
        CheckErrorPolicy error_policy;
        SymbolTable symbol_table;
        const char* source =
            "surface function_call_with_color_passed_instead_of_point() { \n"
            "   diffuse( Cs ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
        CHECK_EQUAL( error_policy.error, RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED );
    }

    TEST( function_call_with_constant_passed_instead_of_point )
    {
        CheckErrorPolicy error_policy;
        SymbolTable symbol_table;
        const char* source =
            "surface function_call_with_constant_passed_instead_of_point() { \n"
            "   diffuse( 1 ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
        CHECK_EQUAL( error_policy.error, RENDER_ERROR_NONE );
    }

    TEST( function_call_with_uniform_passed_instead_of_point )
    {
        CheckErrorPolicy error_policy;
        SymbolTable symbol_table;
        const char* source =
            "surface function_call_with_uniform_passed_instead_of_point(uniform float a = 1;) { \n"
            "   diffuse( a ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
    }

    TEST( function_call_with_point )
    {
        CheckErrorPolicy error_policy;
        SymbolTable symbol_table;
        const char* source =
            "surface function_call_with_point() { \n"
            "   diffuse( N ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
    }
}
