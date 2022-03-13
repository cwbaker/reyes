
#include <UnitTest++/UnitTest++.h>
#include <reyes/ShaderParser.hpp>
#include <reyes/SyntaxNode.hpp>
#include <reyes/Symbol.hpp>
#include <reyes/SymbolTable.hpp>
#include <reyes/Shader.hpp>
#include <reyes/ErrorPolicy.hpp>
#include <reyes/assert.hpp>
#include <vector>
#include <string.h>

using std::vector;
using std::shared_ptr;
using namespace reyes;

SUITE( ShaderParser )
{
    struct BuildSyntaxTree
    {
        std::vector<shared_ptr<SyntaxNode>> nodes_;
        
        BuildSyntaxTree()
        : nodes_()
        {
            shared_ptr<SyntaxNode> node( new SyntaxNode(SHADER_NODE_NULL, 0) );
            nodes_.push_back( node );
        }
        
        BuildSyntaxTree& begin( SyntaxNodeType type, const char* lexeme = nullptr )
        {
            REYES_ASSERT( !nodes_.empty() );
            shared_ptr<SyntaxNode> node( new SyntaxNode(type, 0, lexeme) );
            nodes_.back()->add_node( node );
            nodes_.push_back( node );
            return *this;
        }
        
        BuildSyntaxTree& end()
        {
            REYES_ASSERT( !nodes_.empty() );
            REYES_ASSERT( nodes_.size() > 1 );
            nodes_.pop_back();
            return *this;
        }
        
        BuildSyntaxTree& node( SyntaxNodeType type, const char* lexeme = nullptr )
        {
            shared_ptr<SyntaxNode> node( new SyntaxNode(type, 0, lexeme) );
            nodes_.back()->add_node( node );
            return *this;
        }
        
        BuildSyntaxTree& null()
        {
            return node( SHADER_NODE_NULL );
        }
        
        BuildSyntaxTree& uniform_storage()
        {
            return node( SHADER_NODE_UNIFORM );
        }

        BuildSyntaxTree& varying_storage()
        {
            return node( SHADER_NODE_VARYING );
        }

        BuildSyntaxTree& float_type()
        {
            return node( SHADER_NODE_FLOAT_TYPE );
        }

        BuildSyntaxTree& string_type()
        {
            return node( SHADER_NODE_STRING_TYPE );
        }

        BuildSyntaxTree& color_type()
        {        
            return node( SHADER_NODE_COLOR_TYPE );
        }

        BuildSyntaxTree& normal_type( const char* space = nullptr )
        {
            return space ? begin( SHADER_NODE_NORMAL_TYPE ).string( space ).end() : node( SHADER_NODE_NORMAL_TYPE );
        }
        
        BuildSyntaxTree& point_type( const char* space = nullptr )
        {
            return space ? begin( SHADER_NODE_POINT_TYPE ).string( space ).end() : node( SHADER_NODE_POINT_TYPE );
        }
        
        BuildSyntaxTree& vector_type( const char* space = nullptr )
        {
            return space ? begin( SHADER_NODE_VECTOR_TYPE ).string( space ).end() : node( SHADER_NODE_VECTOR_TYPE );
        }
        
        BuildSyntaxTree& uniform()
        {
            return node( SHADER_NODE_UNIFORM );
        }
        
        BuildSyntaxTree& identifier( const char* lexeme )
        {
            return node( SHADER_NODE_IDENTIFIER, lexeme );
        }
        
        BuildSyntaxTree& integer( const char* lexeme )
        {
            return node( SHADER_NODE_INTEGER, lexeme );
        }
        
        BuildSyntaxTree& real( const char* lexeme )
        {
            return node( SHADER_NODE_REAL, lexeme );
        }
        
        BuildSyntaxTree& string( const char* lexeme )
        {
            return node( SHADER_NODE_STRING, lexeme );
        }
        
        BuildSyntaxTree& triple()
        {
            return begin( SHADER_NODE_TRIPLE );
        }
        
        BuildSyntaxTree& surface_shader( const char* lexeme )
        {
            return begin( SHADER_NODE_SURFACE_SHADER, lexeme );
        }
        
        BuildSyntaxTree& light_shader( const char* lexeme )
        {
            return begin( SHADER_NODE_LIGHT_SHADER, lexeme );
        }
        
        BuildSyntaxTree& volume_shader( const char* lexeme )
        {
            return begin( SHADER_NODE_VOLUME_SHADER, lexeme );
        }
        
        BuildSyntaxTree& displacement_shader( const char* lexeme )
        {
            return begin( SHADER_NODE_DISPLACEMENT_SHADER, lexeme );
        }
        
        BuildSyntaxTree& imager_shader( const char* lexeme )
        {
            return begin( SHADER_NODE_IMAGER_SHADER, lexeme );
        }
        
        BuildSyntaxTree& list()
        {
            return begin( SHADER_NODE_LIST ); 
        }
        
        BuildSyntaxTree& variable( const char* lexeme )
        {
            return begin( SHADER_NODE_VARIABLE, lexeme );
        }
        
        BuildSyntaxTree& ambient()
        {
            return begin( SHADER_NODE_AMBIENT );
        }
        
        BuildSyntaxTree& solar()
        {
            return begin( SHADER_NODE_SOLAR );
        }
        
        BuildSyntaxTree& illuminate()
        {
            return begin( SHADER_NODE_ILLUMINATE );
        }
        
        BuildSyntaxTree& call( const char* lexeme )
        {
            return begin( SHADER_NODE_CALL, lexeme );
        }
        
        BuildSyntaxTree& negate()
        {
            return begin( SHADER_NODE_NEGATE );
        }
        
        BuildSyntaxTree& assign( const char* lexeme )
        {
            return begin( SHADER_NODE_ASSIGN, lexeme );
        }
        
        BuildSyntaxTree& add_assign( const char* lexeme )
        {
            return begin( SHADER_NODE_ADD_ASSIGN, lexeme );
        }
        
        BuildSyntaxTree& multiply_assign( const char* lexeme )
        {
            return begin( SHADER_NODE_MULTIPLY_ASSIGN, lexeme );
        }
        
        BuildSyntaxTree& add()
        {
            return begin( SHADER_NODE_ADD );
        }
        
        BuildSyntaxTree& subtract()
        {
            return begin( SHADER_NODE_SUBTRACT );
        }
        
        BuildSyntaxTree& multiply()
        {
            return begin( SHADER_NODE_MULTIPLY );
        }
        
        BuildSyntaxTree& divide()
        {
            return begin( SHADER_NODE_DIVIDE );
        }
        
        BuildSyntaxTree& dot()
        {
            return begin( SHADER_NODE_DOT );
        }
        
        BuildSyntaxTree& typecast()
        {
            return begin( SHADER_NODE_TYPECAST );
        }
        
        BuildSyntaxTree& texture()
        {
            return begin( SHADER_NODE_TEXTURE );
        }
        
        shared_ptr<SyntaxNode> root() const
        {
            REYES_ASSERT( nodes_.size() == 1 );
            REYES_ASSERT( nodes_.back()->nodes().size() == 1 );
            return nodes_.back()->nodes().at( 0 );
        }
    };

    struct QuietErrorPolicy : public reyes::ErrorPolicy
    {
        void render_error( int /*eerror*/, const char* /*format*/, va_list /*args*/ ) override
        {
        }
    };

    struct SymbolScope
    {
        QuietErrorPolicy error_policy;

        void test( const char* source )
        {
            SymbolTable symbol_table;
            Shader shader( source, source + strlen(source), symbol_table, error_policy );
        }
    };

    TEST( TestConstant )
    {       
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "constant.sl" );

        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader( "constant" )
                .list()
                .end()
                .list()
                    .assign( "Oi" )
                        .identifier( "Os" )
                    .end()
                    .assign( "Ci" )
                        .multiply()
                            .identifier( "Os" )
                            .identifier( "Cs" )
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );
    }

    TEST( TestMatte )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "matte.sl" );

        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader( "matte" )
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Kd" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                .end()
                .list()
                    .variable( "Nf" )
                        .call( "faceforward" )
                            .call( "normalize" )
                                .identifier( "N" )
                            .end()
                            .identifier( "I" )
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .assign( "Oi" )
                        .identifier( "Os" )
                    .end()
                    .assign( "Ci" )
                        .multiply()
                            .identifier( "Os" )
                            .multiply()
                                .identifier( "Cs" )
                                .add()
                                    .multiply()
                                        .identifier( "Ka" )
                                        .call( "ambient" )
                                        .end()
                                    .end()
                                    .multiply()
                                        .identifier( "Kd" )
                                        .call( "diffuse" )
                                            .identifier( "Nf" )
                                        .end()
                                    .end()
                                .end()
                            .end()
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );
    }

    TEST( TestMetal )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "metal.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader( "metal" )
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Ks" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                        .null()
                        .float_type()
                    .end()
                .end()
                .list()
                    .variable( "Nf" )
                        .call( "faceforward" )
                            .call( "normalize" )
                                .identifier( "N" )
                            .end()
                            .identifier( "I" )
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
                        .null()
                        .vector_type()
                    .end()
                    .assign( "Oi" )
                        .identifier( "Os" )
                    .end()
                    .assign( "Ci" )
                        .multiply()
                            .identifier( "Os" )
                            .multiply()
                                .identifier( "Cs" )
                                .add()
                                    .multiply()
                                        .identifier( "Ka" )
                                        .call( "ambient" )
                                        .end()
                                    .end()
                                    .multiply()
                                        .identifier( "Ks" )
                                        .call( "specular" )
                                            .identifier( "Nf" )
                                            .identifier( "V" )
                                            .identifier( "roughness" )
                                        .end()
                                    .end()
                                .end()
                            .end()
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );
    }

    /*
    TEST( TestShinyMetal )
    {
        ShaderParser shader_parser;
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "shinymetal.sl" );

        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader()
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Ks" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Kr" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "texturename" )
                        .string( "" )
                        .null()
                        .string_type()
                    .end()
                .end()
                .list()
                    .variable( "Nf" )
                        .call( "faceforward" )
                            .call( "normalize" )
                                .identifier( "N" )
                            .end()
                            .identifier( "I" )
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .variable( "D" )
                        .call( "reflect" )
                            .identifier( "I" )
                            .call( "normalize" )
                                .identifier( "Nf" )
                            .end()
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .assign( "D" )
                        .call( "vtransform" )
                            .string( "current" )
                            .string( "world" )
                            .identifier( "D" )
                        .end()
                    .end()
                    .assign( "Oi" )
                        .identifier( "Os" )
                    .end()
                    .assign( "Ci" )
                        .multiply()
                            .identifier( "Os" )
                            .multiply()
                                .identifier( "Cs" )
                                .add()
                                    .multiply()
                                        .identifier( "Ka" )
                                        .call( "ambient" )
                                        .end()
                                    .end()
                                    .add()
                                        .multiply()
                                            .identifier( "Ks" )
                                            .call( "specular" )
                                                .identifier( "Nf" )
                                                .identifier( "V" )
                                                .identifier( "roughness" )
                                            .end()
                                        .end()
                                        .multiply()
                                            .identifier( "Kr" )
                                            .typecast()
                                                .color_type()
                                                .texture()
                                                    .identifier( "texturename" )
                                                    .identifier( "D" )
                                                .end()
                                            .end()
                                        .end()
                                    .end()
                                .end()
                            .end()
                        .end()
                    .end()
                .end()             
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );
    }
    */

    TEST( TestPlastic )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "plastic.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader( "plastic" )
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Kd" )
                        .real( ".5" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Ks" )
                        .real( ".5" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "specularcolor" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                .end()
                .list()
                    .variable( "Nf" )
                        .call( "faceforward" )
                            .call( "normalize" )
                                .identifier( "N" )
                            .end()
                            .identifier( "I" )
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
                        .null()
                        .vector_type()
                    .end()
                    .assign( "Oi" )
                        .identifier( "Os" )
                    .end()
                    .assign( "Ci" )
                        .multiply()
                            .identifier( "Os" )
                            .add()
                                .multiply()
                                    .identifier( "Cs" )
                                    .add()
                                        .multiply()
                                            .identifier( "Ka" )
                                            .call( "ambient" )
                                            .end()
                                        .end()
                                        .multiply()
                                            .identifier( "Kd" )
                                            .call( "diffuse" )
                                                .identifier( "Nf" )
                                            .end()
                                        .end()
                                    .end()
                                .end()
                                .multiply()
                                    .identifier( "specularcolor" )
                                    .multiply()
                                        .identifier( "Ks" )
                                        .call( "specular" )
                                            .identifier( "Nf" )
                                            .identifier( "V" )
                                            .identifier( "roughness" )
                                        .end()
                                    .end()
                                .end()
                            .end()
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );
    }
    
    TEST( TestPaintedPlastic )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "paintedplastic.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader( "paintedplastic" )
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Kd" )
                        .real( ".5" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "Ks" )
                        .real( ".5" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "specularcolor" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                    .variable( "texturename" )
                        .string( "" )
                        .null()
                        .string_type()
                    .end()
                .end()
                .list()
                    .variable( "Nf" )
                        .call( "faceforward" )
                            .call( "normalize" )
                                .identifier( "N" )
                            .end()
                            .identifier( "I" )
                        .end()
                        .null()
                        .normal_type()
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
                        .null()
                        .vector_type()
                    .end()
                    .assign( "Oi" )
                        .identifier( "Os" )
                    .end()
                    .assign( "Ci" )
                        .multiply()
                            .identifier( "Os" )
                            .add()
                                .multiply()
                                    .identifier( "Cs" )
                                    .multiply()
                                        .typecast()
                                            .color_type()
                                            .texture()
                                                .identifier( "texturename" )
                                                .identifier( "s" )
                                                .identifier( "t" )
                                            .end()
                                        .end()
                                        .add()
                                            .multiply()
                                                .identifier( "Ka" )
                                                .call( "ambient" )
                                                .end()
                                            .end()
                                            .multiply()
                                                .identifier( "Kd" )
                                                .call( "diffuse" )
                                                    .identifier( "Nf" )
                                                .end()
                                            .end()
                                        .end()
                                    .end()
                                .end()
                                .multiply()
                                    .identifier( "specularcolor" )
                                    .multiply()
                                        .identifier( "Ks" )
                                        .call( "specular" )
                                            .identifier( "Nf" )
                                            .identifier( "V" )
                                            .identifier( "roughness" )
                                        .end()
                                    .end()
                                .end()
                            .end()
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestAmbientLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "ambientlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader( "ambientlight" )
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                .end()
                .list()
                    .assign( "Cl" )
                        .multiply()
                            .identifier( "intensity" )
                            .identifier( "lightcolor" )
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestDistantLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "distantlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader( "distantlight" )
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                    .variable( "from" )
                        .typecast()
                            .point_type( "shader" )
                            .triple()
                                .integer( "0" )
                                .integer( "0" )
                                .integer( "0" )
                            .end()
                        .end()
                        .null()
                        .point_type()
                    .end()
                    .variable( "to" )
                        .typecast()
                            .point_type( "shader" )
                            .triple()
                                .integer( "0" )
                                .integer( "0" )
                                .integer( "1" )
                            .end()
                        .end()
                        .null()
                        .point_type()
                    .end()
                .end()
                .list()
                    .solar()
                        .list()
                            .subtract()
                                .identifier( "to" )
                                .identifier( "from" )
                            .end()
                            .real( "0.0" )
                        .end()
                        .assign( "Cl" )
                            .multiply()
                                .identifier( "intensity" )
                                .identifier( "lightcolor" )
                            .end()
                        .end()
                        .identifier( "Cl" )
                        .identifier( "Ol" )
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
      
        CHECK( *shader == *expected_shader );  
    }
    
    TEST( TestPointLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "pointlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader( "pointlight" )
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                    .variable( "from" )
                        .typecast()
                            .point_type( "shader" )
                            .triple()
                                .integer( "0" )
                                .integer( "0" )
                                .integer( "0" )
                            .end()
                        .end()
                        .null()
                        .point_type()
                    .end()
                .end()
                .list()
                    .illuminate()
                        .list()
                            .identifier( "from" )
                        .end()
                        .assign( "Cl" )
                            .multiply()
                                .identifier( "intensity" )
                                .divide()
                                    .identifier( "lightcolor" )
                                    .dot()
                                        .identifier( "L" )
                                        .identifier( "L" )
                                    .end()
                                .end()
                            .end()
                        .end()
                        .identifier( "Ps" )
                        .identifier( "L" )
                        .identifier( "Cl" )
                        .identifier( "Ol" )
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );  
    }

    TEST( TestSpotLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "spotlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader( "spotlight" )
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                    .variable( "from" )
                        .typecast()
                            .point_type( "shader" )
                            .triple()
                                .integer( "0" )
                                .integer( "0" )
                                .integer( "0" )
                            .end()
                        .end()
                        .null()
                        .point_type()
                    .end()
                    .variable( "to" )
                        .typecast()
                            .point_type( "shader" )
                            .triple()
                                .integer( "0" )
                                .integer( "0" )
                                .integer( "1" )
                            .end()
                        .end()
                        .null()
                        .point_type()
                    .end()
                    .variable( "coneangle" )
                        .call( "radians" )
                            .integer( "30" )
                        .end()
                        .null()
                        .float_type()
                    .end()
                    .variable( "conedeltaangle" )
                        .call( "radians" )
                            .integer( "5" )
                        .end()
                        .null()
                        .float_type()
                    .end()
                    .variable( "beamdistribution" )
                        .integer( "2" )
                        .null()
                        .float_type()
                    .end()
                .end()
                .list()
                    .variable( "atten" )
                        .null()
                        .null()
                        .float_type()
                    .end()
                    .variable( "cosangle" )
                        .null()
                        .null()
                        .float_type()
                    .end()
                    .variable( "A" )
                        .divide()
                            .subtract()
                                .identifier( "to" )
                                .identifier( "from" )
                            .end()
                            .call( "length" )
                                .subtract()
                                    .identifier( "to" )
                                    .identifier( "from" )
                                .end()
                            .end()
                        .end()
                        .uniform_storage()
                        .vector_type()
                    .end()
                    .illuminate()
                        .list()
                            .identifier( "from" )
                            .identifier( "A" )
                            .identifier( "coneangle" )
                        .end()
                        .list()
                            .assign( "cosangle" )
                                .dot()
                                    .identifier( "L" )
                                    .divide()
                                        .identifier( "A" )
                                        .call( "length" )
                                            .identifier( "L" )
                                        .end()
                                    .end()
                                .end()
                            .end()
                            .assign( "atten" )
                                .divide()
                                    .call( "pow" )
                                        .identifier( "cosangle" )
                                        .identifier( "beamdistribution" )
                                    .end()
                                    .dot()
                                        .identifier( "L" )
                                        .identifier( "L" )
                                    .end()
                                .end()
                            .end()
                            .multiply_assign( "atten" )
                                .call( "smoothstep" )
                                    .call( "cos" )
                                        .identifier( "coneangle" )
                                    .end()
                                    .call( "cos" )
                                        .subtract()
                                            .identifier( "coneangle" )
                                            .identifier( "conedeltaangle" )
                                        .end()
                                    .end()
                                    .identifier( "cosangle" )
                                .end()
                            .end()
                            .assign( "Cl" )
                                .multiply()
                                    .identifier( "atten" )
                                    .multiply()
                                        .identifier( "intensity" )
                                        .identifier( "lightcolor" )
                                    .end()
                                .end()
                            .end()
                        .end()
                        .identifier( "Ps" )
                        .identifier( "L" )
                        .identifier( "Cl" )
                        .identifier( "Ol" )                        
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }
    
    TEST( TestDepthCue )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "depthcue.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .volume_shader( "depthcue" )
                .list()
                    .variable( "mindistance" )
                        .integer( "0" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "maxdistance" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "background" )
                        .integer( "0" )
                        .null()
                        .color_type()
                    .end()
                .end()
                .list()
                    .variable( "d" )
                        .null()
                        .null()
                        .float_type()
                    .end()
                    .assign( "d" )
                        .call( "clamp" )
                            .divide()
                                .subtract()
                                    .call( "depth" )
                                        .identifier( "P" )
                                    .end()
                                    .identifier( "mindistance" )
                                .end()
                                .subtract()
                                    .identifier( "maxdistance" )
                                    .identifier( "mindistance" )
                                .end()
                            .end()
                            .real( "0.0" )
                            .real( "1.0" )
                        .end()
                    .end()
                    .assign( "Ci" )
                        .call( "mix" )
                            .identifier( "Ci" )
                            .identifier( "background" )
                            .identifier( "d" )
                        .end()
                    .end()
                    .assign( "Oi" )
                        .call( "mix" )
                            .identifier( "Oi" )
                            .typecast()
                                .color_type()
                                .triple()
                                    .integer( "1" )
                                    .integer( "1" )
                                    .integer( "1" )
                                .end()
                            .end()
                            .identifier( "d" )
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestFog )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "fog.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .volume_shader( "fog" )
                .list()
                    .variable( "distance" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "background" )
                        .integer( "0" )
                        .null()
                        .color_type()
                    .end()
                .end()
                .list()
                    .variable( "d" )
                        .null()
                        .null()
                        .float_type()
                    .end()
                    .assign( "d" )
                        .subtract()
                            .integer( "1" )
                            .call( "exp" )
                                .negate()
                                    .divide()
                                        .call( "length" )
                                            .identifier( "I" )
                                        .end()
                                        .identifier( "distance" )
                                    .end()
                                .end()
                            .end()
                        .end()
                    .end()
                    .assign( "Ci" )
                        .call( "mix" )
                            .identifier( "Ci" )
                            .identifier( "background" )
                            .identifier( "d" )
                        .end()
                    .end()
                    .assign( "Oi" )
                        .call( "mix" )
                            .identifier( "Oi" )
                            .typecast()
                                .color_type()
                                .triple()
                                    .integer( "1" )
                                    .integer( "1" )
                                    .integer( "1" )
                                .end()
                            .end()
                            .identifier( "d" )
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestBumpy )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "bumpy.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .displacement_shader( "bumpy" )
                .list()
                    .variable( "Km" )
                        .integer( "1" )
                        .null()
                        .float_type()
                    .end()
                    .variable( "texturename" )
                        .string( "" )
                        .null()
                        .string_type()
                    .end()
                .end()
                .list()
                    .variable( "amp" )
                        .multiply()
                            .identifier( "Km" )
                            .typecast()
                                .float_type()
                                .texture()
                                    .identifier( "texturename" )
                                    .identifier( "s" )
                                    .identifier( "t" )
                                .end()
                            .end()
                        .end()
                        .null()
                        .float_type()
                    .end()
                    .add_assign( "P" )
                        .multiply()
                            .identifier( "amp" )
                            .call( "normalize" )
                                .identifier( "N" )
                            .end()
                        .end()
                    .end()
                    .assign( "N" )
                        .call( "calculatenormal" )
                            .identifier( "P" )
                        .end()
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestBackground )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "background.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .imager_shader( "background" )
                .list()
                    .variable( "background" )
                        .integer( "1" )
                        .null()
                        .color_type()
                    .end()
                .end()
                .list()
                    .add_assign( "Ci" )
                        .multiply()
                            .subtract()
                                .integer( "1" )
                                .identifier( "alpha" )
                            .end()
                            .identifier( "background" )
                        .end()
                    .end()
                    .assign( "Oi" )
                        .integer( "1" )
                    .end()
                    .assign( "alpha" )
                        .integer( "1" )
                    .end()
                .end()
            .end()
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestShadowPointLight )
    {           
        SymbolTable symbol_table;
        ShaderParser shader_parser( &symbol_table );
        shared_ptr<SyntaxNode> shader = shader_parser.parse( SHADERS_PATH "shadowpointlight.sl" );
        
        /*
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
        .end();
        shared_ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
        */
    }

    TEST_FIXTURE( SymbolScope, L_is_known_inside_illuminance )
    {
        test(
            "surface L_is_known_inside_illuminance() { \n"
            "   vector axis = (0, 0, 1); \n"
            "   float angle = PI; \n"
            "   illuminance( P, axis, angle ) { \n"
            "       vector Ln = normalize( L ); \n"
            "   } \n"
            "}"
        );
        CHECK( error_policy.errors() == 0 );
    }
    
    TEST_FIXTURE( SymbolScope, L_is_unknown_outside_illuminance )
    {
        test(
            "surface L_is_unknown_outside_illuminance() { \n"
            "   vector Ln = normalize( L ); \n"
            "}"
        );
        CHECK( error_policy.errors() > 0 );
    }
    
    TEST_FIXTURE( SymbolScope, Cl_is_known_inside_illuminance )
    {
        test(
            "surface Cl_is_known_inside_illuminance() { \n"
            "   vector axis = (0, 0, 1); \n"
            "   float angle = PI; \n"
            "   illuminance( P, axis, angle ) { \n"
            "       Ci += Cl; \n"
            "   } \n"
            "}"
        );
        CHECK( error_policy.errors() == 0 );
    }
    
    TEST_FIXTURE( SymbolScope, Cl_is_unknown_outside_illuminance )
    {
        test(
            "surface Cl_is_unknown_outside_illuminance() { \n"
            "   Ci += Cl; \n"
            "}"
        );
        CHECK( error_policy.errors() > 0 );
    }
    
    TEST_FIXTURE( SymbolScope, L_is_known_inside_solar )
    {
        test(
            "light L_is_known_inside_solar( point from = point \"shader\" (0, 0, 0); point to = point \"shader\" (0, 0, 0); ) { \n"
            "   solar( from - to, 0 ) { \n"
            "       Cl = 1 / L.L; \n"
            "   } \n"
            "}"
        );
        CHECK( error_policy.errors() == 0 );
    }

    TEST_FIXTURE( SymbolScope, L_is_known_inside_illuminate )
    {
        test(
            "light L_is_known_inside_illuminate( point from = point \"shader\" (0, 0, 0); ) { \n"
            "   illuminate( from ) { \n"
            "       Cl = 1 / L.L; \n"
            "   } \n"
            "}"
        );
        CHECK( error_policy.errors() == 0 );
    }

    TEST_FIXTURE( SymbolScope, L_is_unknown_outside_solar_and_illuminate )
    {
        test(
            "light L_is_unknown_outside_solar_and_illuminate() { \n"
            "   Cl = 1 / L.L; \n"
            "}"
        );
        CHECK( error_policy.errors() > 0 );
    }
}
