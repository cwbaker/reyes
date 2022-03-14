
#include <UnitTest++/UnitTest++.h>
#include <reyes/Shader.hpp>
#include <reyes/Symbol.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/SyntaxNode.hpp>
#include <reyes/ShaderParser.hpp>
#include <reyes/SemanticAnalyzer.hpp>
#include <reyes/CodeGenerator.hpp>
#include <reyes/Value.hpp>
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <reyes/assert.hpp>

using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

SUITE( CodeGeneration )
{
    struct CodeGenerationTest
    {
        ErrorPolicy error_policy;
        SymbolTable symbol_table;
        ShaderParser shader_parser;
        SemanticAnalyzer semantic_analyzer;
        CodeGenerator code_generator;
        
        CodeGenerationTest()
        : symbol_table(),
          shader_parser( &symbol_table, &error_policy ),
          semantic_analyzer( &symbol_table, &error_policy ),
          code_generator( &error_policy )
        {
        }
        
        bool test( const char* filename )
        {
            REYES_ASSERT( filename );

            shared_ptr<SyntaxNode> syntax_node = shader_parser.parse( filename );
            CHECK( syntax_node );            
            if ( syntax_node )
            {        
                semantic_analyzer.analyze( syntax_node.get(), filename );
                code_generator.generate( syntax_node.get(), filename );
            }
            return syntax_node != NULL;
        }
        
        void check_symbol( const char* identifier, ValueType type, ValueStorage storage )
        {
            shared_ptr<Symbol> symbol = code_generator.find_symbol( identifier );
            REYES_ASSERT( symbol );        
            CHECK( symbol );        
            if ( symbol )
            {
                REYES_ASSERT( symbol->type() == type );
                CHECK( symbol->type() == type );
                CHECK( symbol->storage() == storage );
            }
        }

        void check_light_symbols()
        {
            check_symbol( "Ps", TYPE_POINT, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "Cl", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ol", TYPE_COLOR, STORAGE_VARYING );
        }

        void check_surface_symbols()
        {
            check_symbol( "Cs", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Os", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "P", TYPE_POINT, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "I", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "s", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "t", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "Ci", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Oi", TYPE_COLOR, STORAGE_VARYING );
        }

        void check_displacement_symbols()
        {
            check_symbol( "P", TYPE_POINT, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING);
            check_symbol( "I", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "s", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "t", TYPE_FLOAT, STORAGE_VARYING );
        }
    };

    TEST_FIXTURE( CodeGenerationTest, Constant )
    {
        if ( test(SHADERS_PATH "constant.sl") )
        {        
            CHECK( code_generator.symbols().size() == 9 );
            check_surface_symbols();
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, Matte )
    {
        if ( test(SHADERS_PATH "matte.sl") )
        {        
            CHECK( code_generator.symbols().size() == 11 );
            check_surface_symbols();            
            check_symbol( "Ka", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Kd", TYPE_FLOAT, STORAGE_UNIFORM );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, Plastic )
    {
        if ( test(SHADERS_PATH "plastic.sl") )
        {
            CHECK( code_generator.symbols().size() == 14 );
            check_surface_symbols();            
            check_symbol( "Ka", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Kd", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Ks", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "roughness", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "specularcolor", TYPE_COLOR, STORAGE_UNIFORM );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, Bumpy )
    {
        if ( test(SHADERS_PATH "bumpy.sl") )
        {
            CHECK( code_generator.symbols().size() == 7 );
            check_displacement_symbols();
            check_symbol( "Km", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "texturename", TYPE_STRING, STORAGE_UNIFORM );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, PaintedPlastic )
    {
        if ( test(SHADERS_PATH "paintedplastic.sl") )
        {
            CHECK( code_generator.symbols().size() == 15 );
            check_surface_symbols();
            check_symbol( "Ka", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Kd", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Ks", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "roughness", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "specularcolor", TYPE_COLOR, STORAGE_UNIFORM );
            check_symbol( "texturename", TYPE_STRING, STORAGE_UNIFORM );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, DistantLight )
    {
        if ( test(SHADERS_PATH "distantlight.sl") )
        {
            CHECK( code_generator.symbols().size() == 8 );
            check_light_symbols();
            check_symbol( "intensity", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "lightcolor", TYPE_COLOR, STORAGE_UNIFORM );
            check_symbol( "from", TYPE_POINT, STORAGE_UNIFORM );
            check_symbol( "to", TYPE_POINT, STORAGE_UNIFORM );
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, PointLight )
    {
        if ( test(SHADERS_PATH "pointlight.sl") )
        {        
            CHECK( code_generator.symbols().size() == 7 );
            check_light_symbols();
            check_symbol( "intensity", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "lightcolor", TYPE_COLOR, STORAGE_UNIFORM );
            check_symbol( "from", TYPE_POINT, STORAGE_UNIFORM );
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, spotlight )
    {
        if ( test(SHADERS_PATH "spotlight.sl") )
        {        
            CHECK( code_generator.symbols().size() == 11 );
            check_light_symbols();
            check_symbol( "intensity", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "lightcolor", TYPE_COLOR, STORAGE_UNIFORM );
            check_symbol( "from", TYPE_POINT, STORAGE_UNIFORM );
            check_symbol( "to", TYPE_POINT, STORAGE_UNIFORM );
            check_symbol( "coneangle", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "conedeltaangle", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "beamdistribution", TYPE_FLOAT, STORAGE_UNIFORM );
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, ShadowPointLight )
    {
        if ( test(SHADERS_PATH "shadowpointlight.sl") )
        {
            CHECK( code_generator.symbols().size() == 9 );
            check_light_symbols();
            check_symbol( "intensity", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "lightcolor", TYPE_COLOR, STORAGE_UNIFORM );
            check_symbol( "from", TYPE_POINT, STORAGE_UNIFORM );
            check_symbol( "shadowname", TYPE_STRING, STORAGE_UNIFORM );
            check_symbol( "bias", TYPE_FLOAT, STORAGE_UNIFORM );
        }
    }
}
