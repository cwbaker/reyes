
#include <UnitTest++/UnitTest++.h>
#include <reyes/VirtualMachine.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Shader.hpp>
#include <reyes/Grid.hpp>
#include <reyes/Value.hpp>
#include <reyes/ErrorCode.hpp>
#include <reyes/ErrorAction.hpp>
#include <reyes/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::reyes;

static const float TOLERANCE = 0.01f;

SUITE( AssignExpressions )
{
    struct CheckErrorPolicy : public reyes::ErrorPolicy
    {
        int error;

        void render_error( int eerror, const char* /*format*/, va_list /*args*/ )
        {
            error = eerror;
        }
    };

    struct AssignExpressionTest
    {
        Grid grid;
        float* x;
        float* y;
     
        AssignExpressionTest()
        : grid(),
          x( NULL ),
          y( NULL )
        {
            grid.resize( 2, 2 );
            
            shared_ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            shared_ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();
        }
        
        int test( const char* source )
        {
            CheckErrorPolicy error_policy;      
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
            ;            
            Shader shader( source, source + strlen(source), symbol_table, error_policy );
            VirtualMachine virtual_machine;
            virtual_machine.initialize( grid, shader );
            virtual_machine.shade( grid, grid, shader );
            return error_policy.error;
        }
    };

    TEST_FIXTURE( AssignExpressionTest, assign_uniform_from_constant )
    {
        test(
            "surface assign_uniform_from_constant(float z = 0;) { \n"
            "   z = 1; \n"
            "   x = z; \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
    }

    TEST_FIXTURE( AssignExpressionTest, assign_varying_from_constant )
    {
        test(
            "surface assign_varying_from_constant() { \n"
            "   x = 1; \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
    }

    TEST_FIXTURE( AssignExpressionTest, assign_varying_from_uniform )
    {
        test(
            "surface assign_varying_from_uniform(float z = 1;) { \n"
            "   x = z; \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
    }

    TEST_FIXTURE( AssignExpressionTest, assign_result_is_the_assigned_value )
    {
        test(
            "surface assign_result_is_the_assigned_value() { \n"
            "   x = y = 1; \n"
            "}"
        );
        CHECK_CLOSE( 1.0f, x[0], TOLERANCE );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_constant_from_constant_fails )
    {
        const char* shader = 
            "surface assign_constant_from_constant_fails() { \n"
            "   PI = 2; \n"
            "}"
        ;    
        CHECK( test(shader) == RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_constant_from_uniform_fails )
    {
        const char* shader = 
            "surface assign_constant_from_uniform_fails(float z = 0;) { \n"
            "   PI = z; \n"
            "}"
        ;
        CHECK( test(shader) == RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_constant_from_varying_fails )
    {
        const char* shader = 
            "surface assign_constant_from_varying_fails() { \n"
            "   PI = x; \n"
            "}"
        ;    
        CHECK( test(shader) == RENDER_ERROR_SEMANTIC_ANALYSIS_FAILED );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_uniform_from_identified_constant )
    {
        test(
            "surface assign_to_constant_fails(float z = 0;) { \n"
            "   z = PI; \n"
            "   x = z; \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI), x[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI), x[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI), x[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI), x[3], TOLERANCE );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_varying_from_identified_constant )
    {
        test(
            "surface assign_to_constant_fails() { \n"
            "   x = PI; \n"
            "}"
        );
        CHECK_CLOSE( float(M_PI), x[0], TOLERANCE );
        CHECK_CLOSE( float(M_PI), x[1], TOLERANCE );
        CHECK_CLOSE( float(M_PI), x[2], TOLERANCE );
        CHECK_CLOSE( float(M_PI), x[3], TOLERANCE );
    }
}
