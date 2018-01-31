
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
#include <sweet/math/vec2.ipp>
#include <sweet/math/vec3.ipp>
#include <sweet/assert/assert.hpp>

using std::vector;
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
            SWEET_ASSERT( filename );

            ptr<SyntaxNode> syntax_node = shader_parser.parse( filename );
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
            ptr<Symbol> symbol = code_generator.find_symbol( identifier );
            SWEET_ASSERT( symbol );        
            CHECK( symbol );        
            if ( symbol )
            {
                SWEET_ASSERT( symbol->type() == type );
                CHECK( symbol->type() == type );
                CHECK( symbol->storage() == storage );
            }
        }    
    };

    TEST_FIXTURE( CodeGenerationTest, Constant )
    {
        if ( test("../shaders/constant.sl") )
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
        if ( test("../shaders/matte.sl") )
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
        if ( test("../shaders/plastic.sl") )
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
        if ( test("../shaders/bumpy.sl") )
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
        if ( test("../shaders/paintedplastic.sl") )
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
        if ( test("../shaders/distantlight.sl") )
        {        
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, PointLight )
    {
        if ( test("../shaders/pointlight.sl") )
        {        
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, spotlight )
    {
        if ( test("../shaders/spotlight.sl") )
        {        
        }
    }
    
    TEST_FIXTURE( CodeGenerationTest, ShadowPointLight )
    {
        if ( test("../shaders/shadowpointlight.sl") )
        {
        }
    }
}
