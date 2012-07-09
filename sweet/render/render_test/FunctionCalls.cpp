
#include <sweet/unit/UnitTest.h>
#include <sweet/render/ErrorPolicy.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Error.hpp>
#include <sweet/render/ErrorAction.hpp>
#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sweet;
using namespace sweet::render;

static const float TOLERANCE = 0.01f;

SUITE( FunctionCalls )
{
    TEST( function_call_with_color_passed_instead_of_point )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        error_policy.actions( ERROR_ACTION_THROW );
        const char* source =
            "surface function_call_with_color_passed_instead_of_point() { \n"
            "   diffuse( Cs ); \n"
            "}"
        ;
        CHECK_THROW( Shader(source, source + strlen(source), symbol_table, error_policy), SemanticAnalysisFailedError );
    }

    TEST( function_call_with_constant_passed_instead_of_point )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        error_policy.actions( ERROR_ACTION_THROW );
        const char* source =
            "surface function_call_with_constant_passed_instead_of_point() { \n"
            "   diffuse( 1 ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
    }

    TEST( function_call_with_uniform_passed_instead_of_point )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        error_policy.actions( ERROR_ACTION_THROW );
        const char* source =
            "surface function_call_with_uniform_passed_instead_of_point(uniform float a = 1;) { \n"
            "   diffuse( a ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
    }

    TEST( function_call_with_point )
    {
        SymbolTable symbol_table;
        ErrorPolicy error_policy;
        error_policy.actions( ERROR_ACTION_THROW );
        const char* source =
            "surface function_call_with_point() { \n"
            "   diffuse( N ); \n"
            "}"
        ;
        Shader( source, source + strlen(source), symbol_table, error_policy );
    }
}
