
#include <unit/UnitTest.h>
#include <sweet/render/ShaderParser.hpp>
#include <sweet/render/SyntaxNode.hpp>
#include <sweet/render/Symbol.hpp>
#include <sweet/render/SymbolTable.hpp>
#include <sweet/render/Error.hpp>
#include <sweet/assert/assert.hpp>
#include <vector>
#include <string.h>

using std::vector;
using namespace sweet::pointer;
using namespace sweet::render;

SUITE( ShaderParser )
{
    struct BuildSyntaxTree
    {
        std::vector<ptr<SyntaxNode> > nodes_;
        
        BuildSyntaxTree()
        : nodes_()
        {
            ptr<SyntaxNode> node( new SyntaxNode(SHADER_NODE_NULL, 0) );
            nodes_.push_back( node );
        }
        
        BuildSyntaxTree& begin( SyntaxNodeType type, const char* lexeme = NULL )
        {
            SWEET_ASSERT( !nodes_.empty() );
            ptr<SyntaxNode> node( new SyntaxNode(type, 0, lexeme) );
            nodes_.back()->add_node( node );
            nodes_.push_back( node );
            return *this;
        }
        
        BuildSyntaxTree& end()
        {
            SWEET_ASSERT( !nodes_.empty() );
            SWEET_ASSERT( nodes_.size() > 1 );
            nodes_.pop_back();
            return *this;
        }
        
        BuildSyntaxTree& node( SyntaxNodeType type, const char* lexeme = NULL )
        {
            ptr<SyntaxNode> node( new SyntaxNode(type, 0, lexeme) );
            nodes_.back()->add_node( node );
            return *this;
        }
        
        BuildSyntaxTree& null()
        {
            return node( SHADER_NODE_NULL );
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

        BuildSyntaxTree& normal_type( const char* space = NULL )
        {
            return space ? begin( SHADER_NODE_NORMAL_TYPE ).string(space).end() : node( SHADER_NODE_NORMAL_TYPE );
        }
        
        BuildSyntaxTree& point_type( const char* space = NULL )
        {
            return space ? begin( SHADER_NODE_POINT_TYPE ).string(space).end() : node( SHADER_NODE_POINT_TYPE );
        }
        
        BuildSyntaxTree& vector_type( const char* space = NULL )
        {
            return space ? begin( SHADER_NODE_VECTOR_TYPE ).string(space).end() : node( SHADER_NODE_VECTOR_TYPE );
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
        
        BuildSyntaxTree& surface_shader()
        {
            return begin( SHADER_NODE_SURFACE_SHADER );
        }
        
        BuildSyntaxTree& light_shader()
        {
            return begin( SHADER_NODE_LIGHT_SHADER );
        }
        
        BuildSyntaxTree& volume_shader()
        {
            return begin( SHADER_NODE_VOLUME_SHADER );
        }
        
        BuildSyntaxTree& displacement_shader()
        {
            return begin( SHADER_NODE_DISPLACEMENT_SHADER );
        }
        
        BuildSyntaxTree& imager_shader()
        {
            return begin( SHADER_NODE_IMAGER_SHADER );
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
        
        ptr<SyntaxNode> root() const
        {
            SWEET_ASSERT( nodes_.size() == 1 );
            SWEET_ASSERT( nodes_.back()->get_nodes().size() == 1 );
            return nodes_.back()->get_nodes().at( 0 );
        }
    };

    TEST( TestConstant )
    {       
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/constant.sl" );

        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );
    }

    TEST( TestMatte )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/matte.sl" );

        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader()
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                    .end()
                    .variable( "Kd" )
                        .integer( "1" )
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );
    }

    TEST( TestMetal )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/metal.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .begin( SHADER_NODE_SURFACE_SHADER )
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                    .end()
                    .variable( "Ks" )
                        .integer( "1" )
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
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
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );
    }

    /*
    TEST( TestShinyMetal )
    {
        ShaderParser shader_parser;
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/shinymetal.sl" );

        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader()
                .list()
                    .variable( "Ka" )
                            .integer( "1" )
                    .end()
                    .variable( "Ks" )
                        .integer( "1" )
                    .end()
                    .variable( "Kr" )
                        .integer( "1" )
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                    .end()
                    .variable( "texturename" )
                        .string( "" )
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
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
                    .end()
                    .variable( "D" )
                        .call( "reflect" )
                            .identifier( "I" )
                            .call( "normalize" )
                                .identifier( "Nf" )
                            .end()
                        .end()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );
    }
    */

    TEST( TestPlastic )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/plastic.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader()
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                    .end()
                    .variable( "Kd" )
                        .real( ".5" )
                    .end()
                    .variable( "Ks" )
                        .real( ".5" )
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                    .end()
                    .variable( "specularcolor" )
                        .integer( "1" )
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
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );
    }
    
    TEST( TestPaintedPlastic )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/paintedplastic.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .surface_shader()
                .list()
                    .variable( "Ka" )
                        .integer( "1" )
                    .end()
                    .variable( "Kd" )
                        .real( ".5" )
                    .end()
                    .variable( "Ks" )
                        .real( ".5" )
                    .end()
                    .variable( "roughness" )
                        .real( ".1" )
                    .end()
                    .variable( "specularcolor" )
                        .integer( "1" )
                    .end()
                    .variable( "texturename" )
                        .string( "" )
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
                    .end()
                    .variable( "V" )
                        .negate()
                            .call( "normalize" )
                                .identifier( "I" )
                            .end()
                        .end()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestAmbientLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/ambientlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader()
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
                    .end()
                .end()
                .list()
                    .ambient()
                        .identifier( "Cl" )
                        .identifier( "Ol" )
                    .end()
                    .assign( "Cl" )
                        .multiply()
                            .identifier( "intensity" )
                            .identifier( "lightcolor" )
                        .end()
                    .end()
                .end()
            .end()
        .end();
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestDistantLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/distantlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader()
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
      
        CHECK( *shader == *expected_shader );  
    }
    
    TEST( TestPointLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/pointlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader()
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();

        CHECK( *shader == *expected_shader );  
    }

    TEST( TestSpotLight )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/spotlight.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .light_shader()
                .list()
                    .variable( "intensity" )
                        .integer( "1" )
                    .end()
                    .variable( "lightcolor" )
                        .integer( "1" )
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
                    .end()
                    .variable( "coneangle" )
                        .call( "radians" )
                            .integer( "30" )
                        .end()
                    .end()
                    .variable( "conedeltaangle" )
                        .call( "radians" )
                            .integer( "5" )
                        .end()
                    .end()
                    .variable( "beamdistribution" )
                        .integer( "2" )
                    .end()
                .end()
                .list()
                    .variable( "atten" )
                        .null()
                    .end()
                    .variable( "cosangle" )
                        .null()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }
    
    TEST( TestDepthCue )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/depthcue.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .volume_shader()
                .list()
                    .variable( "mindistance" )
                        .integer( "0" )
                    .end()
                    .variable( "maxdistance" )
                        .integer( "1" )
                    .end()
                    .variable( "background" )
                        .integer( "0" )
                    .end()
                .end()
                .list()
                    .variable( "d" )
                        .null()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestFog )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/fog.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .volume_shader()
                .list()
                    .variable( "distance" )
                        .integer( "1" )
                    .end()
                    .variable( "background" )
                        .integer( "0" )
                    .end()
                .end()
                .list()
                    .variable( "d" )
                        .null()
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestBumpy )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/bumpy.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .displacement_shader()
                .list()
                    .variable( "Km" )
                        .integer( "1" )
                    .end()
                    .variable( "texturename" )
                        .string( "" )
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestBackground )
    {
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/background.sl" );
        
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
            .imager_shader()
                .list()
                    .variable( "background" )
                        .integer( "1" )
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
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
    }

    TEST( TestShadowPointLight )
    {           
        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( "../shaders/shadowpointlight.sl" );
        
        /*
        BuildSyntaxTree build_syntax_tree;
        build_syntax_tree.begin( SHADER_NODE_LIST )
        .end();
        ptr<SyntaxNode> expected_shader = build_syntax_tree.root();
        
        CHECK( *shader == *expected_shader );  
        */
    }
    
    TEST( L_is_known_inside_illuminance )
    {
        const char* source =
            "surface L_is_known_inside_illuminance() { \n"
            "   illuminance( P ) { \n"
            "       vector Ln = normalize( L ); \n"
            "   } \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );        
        ptr<SyntaxNode> shader = shader_parser.parse( source, source + strlen(source) );
        CHECK( shader );
    }
    
    TEST( L_is_unknown_outside_illuminance )
    {
        const char* source =
            "surface L_is_unknown_outside_illuminance() { \n"
            "   vector Ln = normalize( L ); \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        CHECK( !shader_parser.parse(source, source + strlen(source)) );
    }
    
    TEST( Cl_is_known_inside_illuminance )
    {
        const char* source =
            "surface Cl_is_known_inside_illuminance() { \n"
            "   illuminance( P ) { \n"
            "       Ci += Cl; \n"
            "   } \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );        
        ptr<SyntaxNode> shader = shader_parser.parse( source, source + strlen(source) );
        CHECK( shader );
    }
    
    TEST( Cl_is_unknown_outside_illuminance )
    {
        const char* source =
            "surface Cl_is_unknown_outside_illuminance() { \n"
            "   Ci += Cl; \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        CHECK( !shader_parser.parse(source, source + strlen(source)) );
    }
    
    TEST( L_is_known_inside_solar )
    {
        const char* source =
            "light L_is_known_inside_solar( point from = point \"shader\" (0, 0, 0); point to = point \"shader\" (0, 0, 0); ) { \n"
            "   solar( from - to, 0 ) { \n"
            "       Cl = L; \n"
            "   } \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( source, source + strlen(source) );
        CHECK( shader );
    }

    TEST( L_is_known_inside_illuminate )
    {
        const char* source =
            "light L_is_known_inside_illuminate( point from = point \"shader\" (0, 0, 0); ) { \n"
            "   illuminate( from ) { \n"
            "       Cl = 1 / L.L; \n"
            "   } \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        ptr<SyntaxNode> shader = shader_parser.parse( source, source + strlen(source) );
        CHECK( shader );
    }

    TEST( L_is_unknown_outside_solar_and_illuminate )
    {
        const char* source =
            "light L_is_unknown_outside_solar_and_illuminate() { \n"
            "   Cl = 1 / L.L; \n"
            "}"
        ;

        SymbolTable symbol_table;
        ShaderParser shader_parser( symbol_table );
        CHECK( !shader_parser.parse(source, source + strlen(source)) );
    }
}
