//
// ShaderParser.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "ShaderParser.hpp"
#include "ErrorCode.hpp"
#include "SyntaxNode.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "ErrorPolicy.hpp"
#include <lalr/Parser.hpp>
#include <lalr/PositionIterator.hpp>
#include "assert.hpp"
#include <fstream>
#include <iterator>
#include <functional>
#include <map>
#include <string>

using std::map;
using std::find;
using std::string;
using std::vector;
using std::istream_iterator;
using std::bind;
using std::shared_ptr;
using namespace std::placeholders;
using namespace reyes;

template <class Iterator>
class ShaderParserContext : public lalr::ErrorPolicy
{
    SymbolTable* symbol_table_;
    reyes::ErrorPolicy* error_policy_;
    const lalr::Parser<lalr::PositionIterator<Iterator>, shared_ptr<SyntaxNode>, char>* parser_;
    int solar_and_illuminate_statements_;
    int errors_;

public:
    ShaderParserContext( SymbolTable* symbol_table, reyes::ErrorPolicy* error_policy )
    : lalr::ErrorPolicy()
    , symbol_table_( symbol_table )
    , error_policy_( error_policy )
    , parser_( nullptr )
    , solar_and_illuminate_statements_( 0 )
    , errors_( 0 )
    {    
    }

    void error( int line, const char* format, va_list args )
    {       
        ++errors_;
        if ( error_policy_ )
        {
            char message [1024];
            vsnprintf( message, sizeof(message), format, args );
            error_policy_->error( RENDER_ERROR_SYNTAX_ERROR, "(%d): %s", line, message );
        }
    }

    void error( int line, const char* format, ... )
    {       
        va_list args;
        va_start( args, format );
        error( line, format, args );
        va_end( args );
    }

    void lalr_error( int line, int /*column*/, int /*error*/, const char* format, va_list args )
    {
        error( line, format, args );
        printf( "(%d): error: ", line );
        vprintf( format, args );
    }
    
    void lalr_vprintf( const char* format, va_list args )
    {
        vprintf( format, args );
    }

    static void string_( lalr::PositionIterator<Iterator> begin, lalr::PositionIterator<Iterator> end, std::string* lexeme, const void** /*symbol*/, lalr::PositionIterator<Iterator>* position, int* lines )
    {
        REYES_ASSERT( lexeme );
        REYES_ASSERT( lexeme->length() == 1 );
        REYES_ASSERT( position );
        REYES_ASSERT( lines );

        lalr::PositionIterator<Iterator> i = begin;
        int terminator = lexeme->at( 0 );
        REYES_ASSERT( terminator == '"' );
        lexeme->clear();
        
        while ( i != end && *i != terminator )
        {
            *lexeme += *i;
            ++i;
        }
        
        if ( i != end )
        {
            ++i;
        }
        
        *position = i;
        *lines = 0;
    }

    shared_ptr<SyntaxNode> shader_definition_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const string& identifier = nodes[1].lexeme();
        const shared_ptr<SyntaxNode>& shader = start[0];
        const shared_ptr<SyntaxNode>& formals = start[3];
        const shared_ptr<SyntaxNode>& statements = start[6];
        shader->set_lexeme( identifier );
        shader->add_node( formals );
        shader->add_node( statements );
        return shader;
    }

    static shared_ptr<SyntaxNode> shader_definition( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length, ShaderParserContext* context )
    {
        ShaderParserContext* shader_parser_context = reinterpret_cast<ShaderParserContext*>( context );
        return shader_parser_context->shader_definition_( start, start + length );
    }
    
    shared_ptr<SyntaxNode> function_definition_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const string& identifier = nodes[1].lexeme();
        const shared_ptr<SyntaxNode>& type_node = start[0];
        const shared_ptr<SyntaxNode>& formals = start[3];
        const shared_ptr<SyntaxNode>& statements = start[6];
        shared_ptr<SyntaxNode> function( new SyntaxNode(SHADER_NODE_FUNCTION, nodes[1].line(), identifier) );
        function->add_node( formals );
        function->add_node( statements );
        function->add_node( type_node );
        return function;
    }
    
    static shared_ptr<SyntaxNode> add_to_list( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> list = start[0];
        shared_ptr<SyntaxNode> back = start[length - 1];
        if ( back->node_type() != SHADER_NODE_LIST )
        {
            list->add_node( back );
        }
        else
        {
            list->add_nodes_at_end( back->nodes().begin(), back->nodes().end() );
        }
        return list;
    }

    static shared_ptr<SyntaxNode> create_list( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> list;
        if ( start[0]->node_type() != SHADER_NODE_LIST )
        {
            list.reset( new SyntaxNode(SHADER_NODE_LIST, start[0]->line()) );
            list->add_node( start[0] );
        }
        else
        {
            list = start[0];
        }
        return list;
    }
    
    static shared_ptr<SyntaxNode> empty_list( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> list( new SyntaxNode(SHADER_NODE_LIST, 0) );
        return list;
    }
    
    shared_ptr<SyntaxNode> formal_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& storage_node = start[1];
        const shared_ptr<SyntaxNode>& type_node = start[2];
        
        const vector<shared_ptr<SyntaxNode>>& syntax_nodes = start[3]->nodes();
        for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = syntax_nodes.begin(); i != syntax_nodes.end(); ++i )
        {
            SyntaxNode* variable_node = i->get();
            REYES_ASSERT( variable_node );
            REYES_ASSERT( variable_node->node_type() == SHADER_NODE_VARIABLE );
            const string& identifier = variable_node->lexeme();
            variable_node->set_lexeme( identifier );
            variable_node->add_node( storage_node );
            variable_node->add_node( type_node );
        }
        
        shared_ptr<SyntaxNode> variable( new SyntaxNode(SHADER_NODE_LIST, start[2]->line()) );
        variable->add_nodes_at_end( syntax_nodes.begin(), syntax_nodes.end() );
        return variable;
    }

    shared_ptr<SyntaxNode> variable_definition_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* /*nodes*/, size_t length )
    {
        const shared_ptr<SyntaxNode>& storage_node = start[1];
        const shared_ptr<SyntaxNode>& type_node = start[2];
        
        const vector<shared_ptr<SyntaxNode>>& nodes = start[3]->nodes();
        for ( vector<shared_ptr<SyntaxNode>>::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
        {
            SyntaxNode* variable_node = i->get();
            REYES_ASSERT( variable_node );
            REYES_ASSERT( variable_node->node_type() == SHADER_NODE_VARIABLE );
            const string& identifier = variable_node->lexeme();
            variable_node->set_lexeme( identifier );
            variable_node->add_node( storage_node );
            variable_node->add_node( type_node );
        }
        
        shared_ptr<SyntaxNode> variable( new SyntaxNode(SHADER_NODE_LIST, start[2]->line()) );
        variable->add_nodes_at_end( nodes.begin(), nodes.end() );
        return variable;
    }

    shared_ptr<SyntaxNode> definition_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> variable( new SyntaxNode(SHADER_NODE_VARIABLE, nodes[0].line(), nodes[0].lexeme()) );
        shared_ptr<SyntaxNode> null( new SyntaxNode(SHADER_NODE_NULL, nodes[0].line()) );
        variable->add_node( null );
        return variable;
    }
    
    shared_ptr<SyntaxNode> definition_expression_with_assignment( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {        
        shared_ptr<SyntaxNode> variable( new SyntaxNode(SHADER_NODE_VARIABLE, nodes[0].line(), nodes[0].lexeme()) );
        const shared_ptr<SyntaxNode>& expression = start[2];
        variable->add_node( expression );
        return variable;
    }
    
    shared_ptr<SyntaxNode> light_shader( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> light_shader( new SyntaxNode(SHADER_NODE_LIGHT_SHADER, nodes[0].line()) );
        return light_shader;
    }

    shared_ptr<SyntaxNode> surface_shader( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> surface_shader( new SyntaxNode(SHADER_NODE_SURFACE_SHADER, nodes[0].line()) );
        return surface_shader;
    }

    shared_ptr<SyntaxNode> volume_shader( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> volume_shader( new SyntaxNode(SHADER_NODE_VOLUME_SHADER, nodes[0].line()) );
        return volume_shader;
    }

    shared_ptr<SyntaxNode> displacement_shader( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> displacement_shader( new SyntaxNode(SHADER_NODE_DISPLACEMENT_SHADER, nodes[0].line()) );
        return displacement_shader;
    }

    shared_ptr<SyntaxNode> imager_shader( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> imager_shader( new SyntaxNode(SHADER_NODE_IMAGER_SHADER, nodes[0].line()) );
        return imager_shader;
    }
    
    static shared_ptr<SyntaxNode> float_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> float_type( new SyntaxNode(SHADER_NODE_FLOAT_TYPE, nodes[0].line()) );
        return float_type;
    }

    static shared_ptr<SyntaxNode> string_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> string_type( new SyntaxNode(SHADER_NODE_STRING_TYPE, nodes[0].line()) );
        return string_type;
    }

    static shared_ptr<SyntaxNode> color_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> color_type( new SyntaxNode(SHADER_NODE_COLOR_TYPE, nodes[0].line()) );
        return color_type;
    }

    static shared_ptr<SyntaxNode> point_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> point_type( new SyntaxNode(SHADER_NODE_POINT_TYPE, nodes[0].line()) );
        return point_type;
    }

    static shared_ptr<SyntaxNode> vector_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> vector_type( new SyntaxNode(SHADER_NODE_VECTOR_TYPE, nodes[0].line()) );
        return vector_type;
    }

    static shared_ptr<SyntaxNode> normal_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> normal_type( new SyntaxNode(SHADER_NODE_NORMAL_TYPE, nodes[0].line()) );
        return normal_type;
    }

    static shared_ptr<SyntaxNode> matrix_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> matrix_type( new SyntaxNode(SHADER_NODE_MATRIX_TYPE, nodes[0].line()) );
        return matrix_type;
    }

    static shared_ptr<SyntaxNode> void_type( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> void_type( new SyntaxNode(SHADER_NODE_VOID_TYPE, nodes[0].line()) );
        return void_type;
    }

    static shared_ptr<SyntaxNode> varying( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> varying( new SyntaxNode(SHADER_NODE_VARYING, nodes[0].line()) );
        return varying;
    }

    static shared_ptr<SyntaxNode> uniform( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> uniform( new SyntaxNode(SHADER_NODE_UNIFORM, nodes[0].line()) );
        return uniform;
    }

    static shared_ptr<SyntaxNode> output( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> output( new SyntaxNode(SHADER_NODE_OUTPUT, nodes[0].line()) );
        return output;
    }

    static shared_ptr<SyntaxNode> extern_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> extern_( new SyntaxNode(SHADER_NODE_EXTERN, nodes[0].line()) );
        return extern_;
    }

    static shared_ptr<SyntaxNode> null( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> null( new SyntaxNode(SHADER_NODE_NULL, 0) );
        return null;
    }

    static shared_ptr<SyntaxNode> block_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& statements = start[1];
        return statements;
    }
    
    static shared_ptr<SyntaxNode> return_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> return_( new SyntaxNode(SHADER_NODE_RETURN, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& expression = start[1];
        REYES_ASSERT( expression );
        return_->add_node( expression );
        return return_;
    }
    
    static shared_ptr<SyntaxNode> break_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> break_( new SyntaxNode(SHADER_NODE_BREAK, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& level = start[1];
        if ( level )
        {
            break_->add_node( level );
        }
        return break_;
    }
    
    static shared_ptr<SyntaxNode> continue_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> continue_( new SyntaxNode(SHADER_NODE_CONTINUE, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& level = start[1];
        if ( level )
        {
            continue_->add_node( level );
        }
        return continue_;
    }
    
    static shared_ptr<SyntaxNode> if_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& expression = start[2];
        REYES_ASSERT( expression );

        const shared_ptr<SyntaxNode>& statement = start[4];
        REYES_ASSERT( statement );
        REYES_ASSERT( statement->node_type() == SHADER_NODE_STATEMENT || statement->node_type() == SHADER_NODE_LIST );

        shared_ptr<SyntaxNode> if_( new SyntaxNode(SHADER_NODE_IF, nodes[0].line()) );
        if_->add_node( expression );
        if_->add_node( statement );
        return if_;
    }
    
    static shared_ptr<SyntaxNode> if_else_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& expression = start[2];
        REYES_ASSERT( expression );

        const shared_ptr<SyntaxNode>& statement = start[4];
        REYES_ASSERT( statement );
        REYES_ASSERT( statement->node_type() == SHADER_NODE_STATEMENT || statement->node_type() == SHADER_NODE_LIST );

        const shared_ptr<SyntaxNode>& else_statement = start[6];
        REYES_ASSERT( else_statement );
        REYES_ASSERT( else_statement->node_type() == SHADER_NODE_STATEMENT || statement->node_type() == SHADER_NODE_LIST );

        shared_ptr<SyntaxNode> if_else( new SyntaxNode(SHADER_NODE_IF_ELSE, nodes[0].line()) );
        if_else->add_node( expression );
        if_else->add_node( statement );
        if_else->add_node( else_statement );
        return if_else;
    }
    
    static shared_ptr<SyntaxNode> while_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& expression = start[2];
        REYES_ASSERT( expression );
        
        const shared_ptr<SyntaxNode>& statement = start[4];
        REYES_ASSERT( statement );
        REYES_ASSERT( statement->node_type() == SHADER_NODE_STATEMENT || statement->node_type() == SHADER_NODE_LIST );

        shared_ptr<SyntaxNode> while_( new SyntaxNode(SHADER_NODE_WHILE, nodes[0].line()) );
        while_->add_node( expression );
        while_->add_node( statement );
        return while_;
    }
    
    static shared_ptr<SyntaxNode> for_statement( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& initial_expression = start[2];
        REYES_ASSERT( initial_expression );
        
        const shared_ptr<SyntaxNode>& condition_expression = start[4];
        REYES_ASSERT( condition_expression );

        const shared_ptr<SyntaxNode>& increment_expression = start[6];
        REYES_ASSERT( increment_expression );

        const shared_ptr<SyntaxNode>& statement = start[8];
        REYES_ASSERT( statement );
        REYES_ASSERT( statement->node_type() == SHADER_NODE_STATEMENT || statement->node_type() == SHADER_NODE_LIST );

        shared_ptr<SyntaxNode> for_( new SyntaxNode(SHADER_NODE_FOR, nodes[0].line()) );
        for_->add_node( initial_expression );
        for_->add_node( condition_expression );
        for_->add_node( increment_expression );
        for_->add_node( statement );
        return for_;
    }
    
    shared_ptr<SyntaxNode> solar_statement_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const char* LIGHT_COLOR = "Cl";
        const char* LIGHT_OPACITY = "Ol";

        const shared_ptr<SyntaxNode>& expressions = start[2];    
        const shared_ptr<SyntaxNode>& statement = start[4];
        shared_ptr<SyntaxNode> light_color( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_COLOR) );
        shared_ptr<SyntaxNode> light_opacity( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_OPACITY) );

        shared_ptr<SyntaxNode> solar( new SyntaxNode(SHADER_NODE_SOLAR, nodes[0].line()) );        
        solar->add_node( expressions );
        solar->add_node( statement );
        solar->add_node( light_color );
        solar->add_node( light_opacity );
        return solar;
    }    
    
    shared_ptr<SyntaxNode> illuminate_statement_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const char* SURFACE_POSITION = "Ps";
        const char* LIGHT_DIRECTION = "L";
        const char* LIGHT_COLOR = "Cl";
        const char* LIGHT_OPACITY = "Ol";

        const shared_ptr<SyntaxNode>& expressions = start[2];
        const shared_ptr<SyntaxNode>& statement = start[4];
        shared_ptr<SyntaxNode> surface_position( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, SURFACE_POSITION) );
        shared_ptr<SyntaxNode> light_direction( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_DIRECTION) );
        shared_ptr<SyntaxNode> light_color( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_COLOR) );
        shared_ptr<SyntaxNode> light_opacity( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_OPACITY) );

        shared_ptr<SyntaxNode> illuminate( new SyntaxNode(SHADER_NODE_ILLUMINATE, nodes[0].line()) );
        illuminate->add_node( expressions );
        illuminate->add_node( statement );
        illuminate->add_node( surface_position );
        illuminate->add_node( light_direction );
        illuminate->add_node( light_color );
        illuminate->add_node( light_opacity );
        return illuminate;
    }
    
    shared_ptr<SyntaxNode> illuminance_statement_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const char* LIGHT_DIRECTION = "L";
        const char* LIGHT_COLOR = "Cl";
        const char* LIGHT_OPACITY = "Ol";

        const shared_ptr<SyntaxNode>& parameters = start[2];        
        const shared_ptr<SyntaxNode>& statement = start[4];
        shared_ptr<SyntaxNode> light_direction( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_DIRECTION) );
        shared_ptr<SyntaxNode> light_color( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_COLOR) );
        shared_ptr<SyntaxNode> light_opacity( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, LIGHT_OPACITY) );

        shared_ptr<SyntaxNode> illuminance( new SyntaxNode(SHADER_NODE_ILLUMINANCE, nodes[0].line()) );
        illuminance->add_node( parameters );
        illuminance->add_node( statement );
        illuminance->add_node( light_direction );
        illuminance->add_node( light_color );
        illuminance->add_node( light_opacity );

        return illuminance;
    }
    
    static shared_ptr<SyntaxNode> statement_error( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return shared_ptr<SyntaxNode>();
    }
    
    static shared_ptr<SyntaxNode> binary_operator( SyntaxNodeType type, const shared_ptr<SyntaxNode>* start, int line )
    {
        const shared_ptr<SyntaxNode>& lhs = start[0];
        REYES_ASSERT( lhs );
        
        const shared_ptr<SyntaxNode>& rhs = start[2];
        REYES_ASSERT( rhs );
        
        shared_ptr<SyntaxNode> binary_operator( new SyntaxNode(type, line) );
        binary_operator->add_node( lhs );
        binary_operator->add_node( rhs );
        return binary_operator;
    }

    static shared_ptr<SyntaxNode> dot_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_DOT, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> cross_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_CROSS, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> multiply_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_MULTIPLY, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> divide_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_DIVIDE, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> add_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_ADD, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> subtract_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_SUBTRACT, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> greater_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_GREATER, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> greater_equal_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_GREATER_EQUAL, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> less_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_LESS, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> less_equal_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_LESS_EQUAL, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> equal_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_EQUAL, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> not_equal_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_NOT_EQUAL, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> and_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_AND, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> or_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return binary_operator( SHADER_NODE_OR, start, nodes[1].line() );
    }
    
    static shared_ptr<SyntaxNode> negate_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& expression = start[1];
        REYES_ASSERT( expression );
        
        shared_ptr<SyntaxNode> negate( new SyntaxNode(SHADER_NODE_NEGATE, nodes[0].line()) );
        negate->add_node( expression );
        return negate;
    }
    
    static shared_ptr<SyntaxNode> ternary_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& condition_expression = start[0];
        REYES_ASSERT( condition_expression );
    
        const shared_ptr<SyntaxNode>& expression = start[0];
        REYES_ASSERT( expression );

        const shared_ptr<SyntaxNode>& else_expression = start[0];
        REYES_ASSERT( else_expression );

        shared_ptr<SyntaxNode> ternary_expression( new SyntaxNode(SHADER_NODE_TERNARY, nodes[1].line()) );
        ternary_expression->add_node( condition_expression );
        ternary_expression->add_node( expression );
        ternary_expression->add_node( else_expression );
        return ternary_expression;
    }
    
    static shared_ptr<SyntaxNode> typecast_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> typecast( new SyntaxNode(SHADER_NODE_TYPECAST, start[0]->line()) );
        typecast->add_node( start[0] );
        typecast->add_node( start[1] );
        return typecast;
    }
    
    static shared_ptr<SyntaxNode> compound_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& expression = start[1];
        REYES_ASSERT( expression );
        return expression;
    }
    
    static shared_ptr<SyntaxNode> integer_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> integer( new SyntaxNode(SHADER_NODE_INTEGER, nodes[0].line(), nodes[0].lexeme()) );
        return integer;
    }
    
    static shared_ptr<SyntaxNode> real_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> real( new SyntaxNode(SHADER_NODE_REAL, nodes[0].line(), nodes[0].lexeme()) );
        return real;
    }
    
    static shared_ptr<SyntaxNode> string_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> string( new SyntaxNode(SHADER_NODE_STRING, nodes[0].line(), nodes[0].lexeme()) );
        return string;
    }
    
    shared_ptr<SyntaxNode> identifier_expression_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const string& identifier = nodes[0].lexeme();
        shared_ptr<SyntaxNode> expression( new SyntaxNode(SHADER_NODE_IDENTIFIER, nodes[0].line(), identifier) );
        return expression;
    }

    static shared_ptr<SyntaxNode> index_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        REYES_ASSERT( false );
        return shared_ptr<SyntaxNode>();
    }
    
    static shared_ptr<SyntaxNode> pass( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& node = start[0];
        REYES_ASSERT( node );
        return node;
    }
    
    static shared_ptr<SyntaxNode> triple_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& first_expression = start[1];
        REYES_ASSERT( first_expression );

        const shared_ptr<SyntaxNode>& second_expression = start[3];
        REYES_ASSERT( second_expression );

        const shared_ptr<SyntaxNode>& third_expression = start[5];
        REYES_ASSERT( third_expression );
    
        shared_ptr<SyntaxNode> triple( new SyntaxNode(SHADER_NODE_TRIPLE, nodes[0].line()) );
        triple->add_node( first_expression );
        triple->add_node( second_expression );
        triple->add_node( third_expression );
        return triple;
    }
    
    static shared_ptr<SyntaxNode> sixteentuple_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> sixteentuple( new SyntaxNode(SHADER_NODE_SIXTEENTUPLE, nodes[0].line()) );
        for ( int i = 0; i < 16; ++i )
        {
            const shared_ptr<SyntaxNode>& expression = start[1 + i * 2];
            REYES_ASSERT( expression );
            sixteentuple->add_node( expression );
        }        
        return sixteentuple;
    }
    
    static shared_ptr<SyntaxNode> color_typecast( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> color_type( new SyntaxNode(SHADER_NODE_COLOR_TYPE, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& space = start[1];
        if ( space )
        {
            color_type->add_node( space );
        }
        return color_type;
    }
    
    static shared_ptr<SyntaxNode> point_typecast( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> point_type( new SyntaxNode(SHADER_NODE_POINT_TYPE, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& space = start[1];
        if ( space )
        {
            point_type->add_node( space );
        }
        return point_type;
    }

    static shared_ptr<SyntaxNode> vector_typecast( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> vector_type( new SyntaxNode(SHADER_NODE_VECTOR_TYPE, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& space = start[1];
        if ( space )
        {
            vector_type->add_node( space );
        }
        return vector_type;
    }

    static shared_ptr<SyntaxNode> normal_typecast( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> normal_type( new SyntaxNode(SHADER_NODE_NORMAL_TYPE, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& space = start[1];
        if ( space )
        {
            normal_type->add_node( space );
        }
        return normal_type;
    }

    static shared_ptr<SyntaxNode> matrix_typecast( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> matrix_type( new SyntaxNode(SHADER_NODE_MATRIX_TYPE, nodes[0].line()) );
        const shared_ptr<SyntaxNode>& space = start[1];
        if ( space )
        {
            matrix_type->add_node( space );
        }
        return matrix_type;
    }

    shared_ptr<SyntaxNode> assign_operator( SyntaxNodeType type, const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes )
    {
        const shared_ptr<SyntaxNode>& expression = start[2];
        REYES_ASSERT( expression );
    
        const string& identifier = nodes[0].lexeme();
        shared_ptr<SyntaxNode> assign_operator( new SyntaxNode(type, nodes[0].line(), identifier) );
        assign_operator->add_node( expression );
        assign_operator->set_lexeme( identifier );
        return assign_operator;
    }
    
    shared_ptr<SyntaxNode> assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {        
        return assign_operator( SHADER_NODE_ASSIGN, start, nodes );
    }
    
    shared_ptr<SyntaxNode> add_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return assign_operator( SHADER_NODE_ADD_ASSIGN, start, nodes );
    }
    
    shared_ptr<SyntaxNode> subtract_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return assign_operator( SHADER_NODE_SUBTRACT_ASSIGN, start, nodes );
    }
    
    shared_ptr<SyntaxNode> multiply_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return assign_operator( SHADER_NODE_MULTIPLY_ASSIGN, start, nodes );
    }
    
    shared_ptr<SyntaxNode> divide_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        return assign_operator( SHADER_NODE_DIVIDE_ASSIGN, start, nodes );
    }
    
    shared_ptr<SyntaxNode> index_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        REYES_ASSERT( false );
        return shared_ptr<SyntaxNode>();
    }
    
    shared_ptr<SyntaxNode> index_add_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        REYES_ASSERT( false );
        return shared_ptr<SyntaxNode>();
    }
    
    shared_ptr<SyntaxNode> index_subtract_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        REYES_ASSERT( false );
        return shared_ptr<SyntaxNode>();
    }
    
    shared_ptr<SyntaxNode> index_multiply_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        REYES_ASSERT( false );
        return shared_ptr<SyntaxNode>();
    }
    
    shared_ptr<SyntaxNode> index_divide_assign_expression( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        REYES_ASSERT( false );
        return shared_ptr<SyntaxNode>();
    }
    
    shared_ptr<SyntaxNode> call_expression_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        const shared_ptr<SyntaxNode>& expressions = start[2];
        REYES_ASSERT( expressions );
        REYES_ASSERT( expressions->node_type() == SHADER_NODE_LIST );
    
        shared_ptr<SyntaxNode> call( new SyntaxNode(SHADER_NODE_CALL, nodes[0].line(), nodes[0].lexeme()) );
        call->add_nodes_at_end( expressions->nodes().begin(), expressions->nodes().end() );
        return call;
    }

    shared_ptr<SyntaxNode> texture_expression_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> texture( new SyntaxNode(SHADER_NODE_TEXTURE, nodes[0].line()) );
        const vector<shared_ptr<SyntaxNode> >& parameters = start[2]->nodes();
        texture->add_nodes_at_end( parameters.begin(), parameters.end() );
        if ( parameters.size() == 1 )
        {
            shared_ptr<SyntaxNode> s( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, "s") );
            s->set_symbol( symbol_table_->find_symbol("s") );
            texture->add_node( s );
            shared_ptr<SyntaxNode> t( new SyntaxNode(SHADER_NODE_IDENTIFIER, 0, "t") );
            t->set_symbol( symbol_table_->find_symbol("t") );
            texture->add_node( t );
        }
        return texture;
    }
    
    shared_ptr<SyntaxNode> shadow_expression_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> shadow( new SyntaxNode(SHADER_NODE_SHADOW, nodes[0].line()) );
        const vector<shared_ptr<SyntaxNode> >& parameters = start[2]->nodes();
        shadow->add_nodes_at_end( parameters.begin(), parameters.end() );
        return shadow;
    }
    
    shared_ptr<SyntaxNode> environment_expression_( const shared_ptr<SyntaxNode>* start, const lalr::ParserNode<>* nodes, size_t length )
    {
        shared_ptr<SyntaxNode> environment( new SyntaxNode(SHADER_NODE_ENVIRONMENT, nodes[0].line()) );
        const vector<shared_ptr<SyntaxNode> >& parameters = start[2]->nodes();
        environment->add_nodes_at_end( parameters.begin(), parameters.end() );
        return environment;
    }
    
    shared_ptr<SyntaxNode> parse( Iterator start, Iterator finish, const char* name )
    {
        REYES_ASSERT( name );

        extern const lalr::ParserStateMachine* shader_parser_state_machine;
        lalr::Parser<lalr::PositionIterator<Iterator>, shared_ptr<SyntaxNode>, char> parser( shader_parser_state_machine, this );
        parser.lexer_action_handlers()
            ( "string", &string_ )
        ;
        parser.parser_action_handlers()
            ( "shader_definition", bind(&ShaderParserContext::shader_definition_, this, _1, _2, _3) )
            ( "function_definition", bind(&ShaderParserContext::function_definition_, this, _1, _2, _3) )
            ( "add_to_list", &add_to_list )
            ( "create_list", &create_list )
            ( "empty_list", &empty_list )
            ( "formal", bind(&ShaderParserContext::formal_, this, _1, _2, _3) )
            ( "variable_definition", bind(&ShaderParserContext::variable_definition_, this, _1, _2, _3) )
            ( "definition_expression", bind(&ShaderParserContext::definition_expression, this, _1, _2, _3) )
            ( "definition_expression_with_assignment", bind(&ShaderParserContext::definition_expression_with_assignment, this, _1, _2, _3) )
            ( "light_shader", bind(&ShaderParserContext::light_shader, this, _1, _2, _3) )
            ( "surface_shader", bind(&ShaderParserContext::surface_shader, this, _1, _2, _3) )
            ( "volume_shader", bind(&ShaderParserContext::volume_shader, this, _1, _2, _3) )
            ( "displacement_shader", bind(&ShaderParserContext::displacement_shader, this, _1, _2, _3) )
            ( "imager_shader", bind(&ShaderParserContext::imager_shader, this, _1, _2, _3) )
            ( "float_type", &float_type )
            ( "string_type", &string_type )
            ( "color_type", &color_type )
            ( "point_type", &point_type )
            ( "vector_type", &vector_type )
            ( "normal_type", &normal_type )
            ( "normal_type", &normal_type )
            ( "matrix_type", &matrix_type )
            ( "void_type", &void_type )
            ( "varying", &varying )
            ( "uniform", &uniform )
            ( "output", &output )
            ( "extern", &extern_ )
            ( "null", &null )
            ( "block_statement", &block_statement )
            ( "return_statement", &return_statement )
            ( "break_statement", &break_statement )
            ( "continue_statement", &continue_statement )
            ( "if_else_statement", &if_else_statement )
            ( "if_statement", &if_statement )
            ( "while_statement", &while_statement )
            ( "for_statement", &for_statement )
            ( "solar_statement", bind(&ShaderParserContext::solar_statement_, this, _1, _2, _3) )
            ( "illuminate_statement", bind(&ShaderParserContext::illuminate_statement_, this, _1, _2, _3) )
            ( "illuminance_statement", bind(&ShaderParserContext::illuminance_statement_, this, _1, _2, _3) )
            ( "dot_expression", &dot_expression )
            ( "cross_expression", &cross_expression )
            ( "multiply_expression", &multiply_expression )
            ( "divide_expression", &divide_expression )
            ( "add_expression", &add_expression )
            ( "subtract_expression", &subtract_expression )
            ( "greater_expression", &greater_expression )
            ( "greater_equal_expression", &greater_equal_expression )
            ( "less_expression", &less_expression )
            ( "less_equal_expression", &less_equal_expression )
            ( "equal_expression", &equal_expression )
            ( "not_equal_expression", &not_equal_expression )
            ( "and_expression", &and_expression )
            ( "or_expression", &or_expression )
            ( "negate_expression", &negate_expression )
            ( "ternary_expression", &ternary_expression )
            ( "typecast_expression", &typecast_expression )
            ( "compound_expression", &compound_expression )
            ( "integer_expression", &integer_expression )
            ( "real_expression", &real_expression )
            ( "string_expression", &string_expression )
            ( "identifier_expression", bind(&ShaderParserContext::identifier_expression_, this, _1, _2, _3) )
            ( "index_expression", &index_expression )
            ( "pass", &pass )
            ( "triple_expression", &triple_expression )
            ( "sixteentuple_expression", &sixteentuple_expression )
            ( "color_typecast", &color_typecast )
            ( "point_typecast", &point_typecast )
            ( "vector_typecast", &vector_typecast )
            ( "normal_typecast", &normal_typecast )
            ( "matrix_typecast", &matrix_typecast )
            ( "assign_expression", bind(&ShaderParserContext::assign_expression, this, _1, _2, _3) )
            ( "add_assign_expression", bind(&ShaderParserContext::add_assign_expression, this, _1, _2, _3) )
            ( "subtract_assign_expression", bind(&ShaderParserContext::subtract_assign_expression, this, _1, _2, _3) )
            ( "multiply_assign_expression", bind(&ShaderParserContext::multiply_assign_expression, this, _1, _2, _3) )
            ( "divide_assign_expression", bind(&ShaderParserContext::divide_assign_expression, this, _1, _2, _3) )
            ( "index_assign_expression", bind(&ShaderParserContext::index_assign_expression, this, _1, _2, _3) )
            ( "index_add_assign_expression", bind(&ShaderParserContext::index_add_assign_expression, this, _1, _2, _3) )
            ( "index_subtract_assign_expression", bind(&ShaderParserContext::index_subtract_assign_expression, this, _1, _2, _3) )
            ( "index_multiply_assign_expression", bind(&ShaderParserContext::index_multiply_assign_expression, this, _1, _2, _3) )
            ( "index_divide_assign_expression", bind(&ShaderParserContext::index_divide_assign_expression, this, _1, _2, _3) )
            ( "call_expression", bind(&ShaderParserContext::call_expression_, this, _1, _2, _3) )
            ( "texture_expression", bind(&ShaderParserContext::texture_expression_, this, _1, _2, _3) )
            ( "environment_expression", bind(&ShaderParserContext::environment_expression_, this, _1, _2, _3) )
            ( "shadow_expression", bind(&ShaderParserContext::shadow_expression_, this, _1, _2, _3) )
        ;

        errors_ = 0;
        parser_ = &parser;
        parser.parse( lalr::PositionIterator<Iterator>(start, finish), lalr::PositionIterator<Iterator>() );
        shared_ptr<SyntaxNode> syntax_node;
        if ( parser.accepted() && parser.full() && errors_ == 0 )
        {
            syntax_node = parser.user_data();
            syntax_node->set_scope( symbol_table_->global_scope() );
        }
        else if ( error_policy_ )
        {
            error_policy_->error( RENDER_ERROR_PARSING_FAILED, "Parsing shader '%s' failed", name );
        }
        return syntax_node;
    }
};

ShaderParser::ShaderParser( SymbolTable* symbol_table, ErrorPolicy* error_policy )
: symbol_table_( symbol_table )
, error_policy_( error_policy )
{
    REYES_ASSERT( symbol_table_ );
}

shared_ptr<SyntaxNode> ShaderParser::parse( const char* filename )
{    
    REYES_ASSERT( filename );

    shared_ptr<SyntaxNode> syntax_node;    
    std::ifstream stream( filename, std::ios::binary );
    if ( stream.is_open() )
    {
        stream.unsetf( std::iostream::skipws );
        stream.exceptions( std::iostream::badbit );        
        ShaderParserContext<istream_iterator<char>> shader_parser_context( symbol_table_, error_policy_ );
        syntax_node = shader_parser_context.parse( istream_iterator<char>(stream), istream_iterator<char>(), filename );
    }
    else if ( error_policy_ )
    {
        error_policy_->error( RENDER_ERROR_OPENING_FILE_FAILED, "Opening shader '%s' failed", filename );
    }
    return syntax_node;
}

shared_ptr<SyntaxNode> ShaderParser::parse( const char* start, const char* finish )
{    
    REYES_ASSERT( start );
    REYES_ASSERT( finish );
    REYES_ASSERT( start <= finish );

    ShaderParserContext<const char*> shader_parser_context( symbol_table_, error_policy_ );
    return shader_parser_context.parse( start, finish, "from memory" );
}
