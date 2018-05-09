
#include <UnitTest++/UnitTest++.h>
#include <sweet/render/Shader.hpp>
#include <sweet/render/Symbol.hpp>
#include <sweet/render/ErrorPolicy.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/SyntaxNode.hpp>
#include <sweet/render/ShaderParser.hpp>
#include <sweet/render/SemanticAnalyzer.hpp>
#include <sweet/render/CodeGenerator.hpp>
#include <sweet/render/Value.hpp>
#include <sweet/render/Instruction.hpp>
#include <math/vec2.ipp>
#include <math/vec3.ipp>
#include <sweet/render/assert.hpp>

using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::math;
using namespace sweet::render;

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
          shader_parser( symbol_table, &error_policy ),
          semantic_analyzer( symbol_table, &error_policy ),
          code_generator( symbol_table, &error_policy )
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
    };

    TEST_FIXTURE( CodeGenerationTest, Constant )
    {
        if ( test(SHADERS_PATH "constant.sl") )
        {        
            CHECK( code_generator.symbols().size() == 4 );
            check_symbol( "Oi", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Os", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ci", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Cs", TYPE_COLOR, STORAGE_VARYING );
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, Matte )
    {
        if ( test(SHADERS_PATH "matte.sl") )
        {        
            CHECK( code_generator.symbols().size() == 13 );
            check_symbol( "Nf", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "faceforward", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "normalize", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "I", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "Oi", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Os", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ci", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Cs", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ka", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "ambient", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Kd", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "diffuse", TYPE_COLOR, STORAGE_VARYING );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, Plastic )
    {
        if ( test(SHADERS_PATH "plastic.sl") )
        {
            CHECK( code_generator.symbols().size() == 18 );
            check_symbol( "Nf", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "faceforward", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "normalize", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "I", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "Oi", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Os", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ci", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Cs", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ka", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "ambient", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Kd", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "diffuse", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "specular", TYPE_COLOR, STORAGE_VARYING );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, Bumpy )
    {
        if ( test(SHADERS_PATH "bumpy.sl") )
        {
            CHECK( code_generator.symbols().size() == 9 );
            check_symbol( "Km", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "texturename", TYPE_STRING, STORAGE_UNIFORM );
            check_symbol( "amp", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "s", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "t", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "P", TYPE_POINT, STORAGE_VARYING );
            check_symbol( "normalize", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "calculatenormal", TYPE_POINT, STORAGE_VARYING );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, PaintedPlastic )
    {
        if ( test(SHADERS_PATH "paintedplastic.sl") )
        {
            CHECK( code_generator.symbols().size() == 21 );
            check_symbol( "Ka", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Kd", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "Ks", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "roughness", TYPE_FLOAT, STORAGE_UNIFORM );
            check_symbol( "specularcolor", TYPE_COLOR, STORAGE_UNIFORM );
            check_symbol( "texturename", TYPE_STRING, STORAGE_UNIFORM );
            check_symbol( "Nf", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "N", TYPE_NORMAL, STORAGE_VARYING );
            check_symbol( "I", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "V", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "Oi", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Os", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Ci", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "Cs", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "s", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "t", TYPE_FLOAT, STORAGE_VARYING );
            check_symbol( "normalize", TYPE_VECTOR, STORAGE_VARYING );
            check_symbol( "ambient", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "diffuse", TYPE_COLOR, STORAGE_VARYING );
            check_symbol( "specular", TYPE_COLOR, STORAGE_VARYING );
        }
    }

    TEST_FIXTURE( CodeGenerationTest, DistantLight )
    {
        if ( test(SHADERS_PATH "distantlight.sl") )
        {        
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, PointLight )
    {
        if ( test(SHADERS_PATH "pointlight.sl") )
        {        
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, spotlight )
    {
        if ( test(SHADERS_PATH "spotlight.sl") )
        {        
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, ShadowPointLight )
    {
        if ( test(SHADERS_PATH "shadowpointlight.sl") )
        {
        }
    }
}
