
#include <unit/UnitTest.h>
#include <sweet/render/VirtualMachine.hpp>
#include <sweet/render/ErrorPolicy.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Grid.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/Error.hpp>
#include <sweet/render/ErrorAction.hpp>
#include <sweet/assert/assert.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

using std::vector;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

static const float TOLERANCE = 0.01f;

SUITE( AssignExpressions )
{
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
            
            ptr<Value> x_value = grid.add_value( "x", TYPE_FLOAT );
            x_value->zero();
            x = x_value->float_values();
            
            ptr<Value> y_value = grid.add_value( "y", TYPE_FLOAT );
            y_value->zero();
            y = y_value->float_values();
        }
        
        void test( const char* source )
        {
            ErrorPolicy error_policy;
            error_policy.actions( ERROR_ACTION_THROW );
            
            SymbolTable symbol_table;
            symbol_table.add_symbols()
                ( "x", TYPE_FLOAT )
                ( "y", TYPE_FLOAT )
            ;            
            Shader shader( source, source + strlen(source), symbol_table, error_policy );
            VirtualMachine virtual_machine;
            virtual_machine.initialize( grid, shader );
            virtual_machine.shade( grid, grid, shader );
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
        CHECK_THROW( test(shader), SemanticAnalysisFailedError );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_constant_from_uniform_fails )
    {
        const char* shader = 
            "surface assign_constant_from_uniform_fails(float z = 0;) { \n"
            "   PI = z; \n"
            "}"
        ;    
        CHECK_THROW( test(shader), SemanticAnalysisFailedError );
    }
    
    TEST_FIXTURE( AssignExpressionTest, assign_constant_from_varying_fails )
    {
        const char* shader = 
            "surface assign_constant_from_varying_fails() { \n"
            "   PI = x; \n"
            "}"
        ;    
        CHECK_THROW( test(shader), SemanticAnalysisFailedError );
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
