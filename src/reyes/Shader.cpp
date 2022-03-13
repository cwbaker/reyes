//
// Shader.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "stdafx.hpp"
#include "Shader.hpp"
#include "SyntaxNode.hpp"
#include "ShaderParser.hpp"
#include "SemanticAnalyzer.hpp"
#include "CodeGenerator.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "assert.hpp"

using std::map;
using std::string;
using std::vector;
using std::shared_ptr;
using namespace reyes;

Shader::Shader()
: symbols_()
, values_()
, constants_()
, code_()
, initialize_address_( 0 )
, shade_address_( 0 )
, parameters_( 0 )
, variables_( 0 )
, constant_memory_size_( 0 )
, grid_memory_size_( 0 )
, temporary_memory_size_( 0 )
{
}

Shader::Shader( const char* filename, ErrorPolicy& error_policy )
: symbols_()
, values_()
, constants_()
, code_()
, initialize_address_( 0 )
, shade_address_( 0 )
, parameters_( 0 )
, variables_( 0 )
, constant_memory_size_( 0 )
, grid_memory_size_( 0 )
, temporary_memory_size_( 0 )
{
    load_file( filename, error_policy );
}

Shader::Shader( const char* filename, SymbolTable& symbol_table, ErrorPolicy& error_policy )
: symbols_()
, values_()
, constants_()
, code_()
, initialize_address_( 0 )
, shade_address_( 0 )
, parameters_( 0 )
, variables_( 0 )
, constant_memory_size_( 0 )
, grid_memory_size_( 0 )
, temporary_memory_size_( 0 )
{
    load_file( filename, symbol_table, error_policy );
}

Shader::Shader( const char* start, const char* finish, ErrorPolicy& error_policy )
: symbols_()
, values_()
, code_()
, initialize_address_( 0 )
, shade_address_( 0 )
, parameters_( 0 )
, variables_( 0 )
, constant_memory_size_( 0 )
, grid_memory_size_( 0 )
, temporary_memory_size_( 0 )
{
    load_memory( start, finish, error_policy );
}

Shader::Shader( const char* start, const char* finish, SymbolTable& symbol_table, ErrorPolicy& error_policy )
: symbols_()
, values_()
, code_()
, initialize_address_( 0 )
, shade_address_( 0 )
, parameters_( 0 )
, variables_( 0 )
, constant_memory_size_( 0 )
, grid_memory_size_( 0 )
, temporary_memory_size_( 0 )
{
    load_memory( start, finish, symbol_table, error_policy );
}

const Symbol* Shader::symbol( int index ) const
{
    REYES_ASSERT( index >= 0 && index < int(symbols_.size()) );
    return index >= 0 && index < int(symbols_.size()) ? symbols_[index].get() : nullptr;
}

const std::vector<std::shared_ptr<Symbol>>& Shader::symbols() const
{
    return symbols_;
}

const std::vector<std::shared_ptr<Value>>& Shader::values() const
{
    return values_;
}

const unsigned char* Shader::constant( int offset ) const
{
    REYES_ASSERT( offset >= 0 && offset < int(constants_.size()) );
    return &constants_[offset];
}

const std::vector<unsigned char>& Shader::code() const
{
    return code_;
}

int Shader::initialize_address() const
{
    return initialize_address_;
}

int Shader::shade_address() const
{
    return shade_address_;
}

int Shader::end_address() const
{
    return int(code_.size());
}

int Shader::parameters() const
{
    return parameters_;
}

int Shader::variables() const
{
    return variables_;
}

int Shader::constant_memory_size() const
{
    return constant_memory_size_;
}

int Shader::grid_memory_size() const
{
    return grid_memory_size_;
}

int Shader::temporary_memory_size() const
{
    return temporary_memory_size_;
}

std::shared_ptr<Symbol> Shader::find_symbol( const std::string& identifier ) const
{
    vector<shared_ptr<Symbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    return i != symbols_.end() ? *i : shared_ptr<Symbol>();
}

void Shader::load_file( const char* filename, ErrorPolicy& error_policy )
{
    SymbolTable symbol_table;
    load_file( filename, symbol_table, error_policy );
}

void Shader::load_file( const char* filename, SymbolTable& symbol_table, ErrorPolicy& error_policy )
{
    REYES_ASSERT( filename );
    REYES_ASSERT( symbol_table );
    REYES_ASSERT( error_policy );
    
    ShaderParser shader_parser( &symbol_table, &error_policy );
    shared_ptr<SyntaxNode> syntax_node = shader_parser.parse( filename );

    SemanticAnalyzer semantic_analyzer( &symbol_table, &error_policy );
    semantic_analyzer.analyze( syntax_node.get(), filename );

    CodeGenerator code_generator( &error_policy );
    code_generator.generate( syntax_node.get(), filename );
    
    constants_ = code_generator.constant_data();
    symbols_.swap( code_generator.symbols() );
    values_.swap( code_generator.values() );
    constants_ = code_generator.constant_data();
    code_ = code_generator.code();

    initialize_address_ = code_generator.initialize_address();
    shade_address_ = code_generator.shade_address();
    parameters_ = code_generator.parameters();
    variables_ = code_generator.variables();
    constant_memory_size_ = code_generator.constant_memory_size();
    grid_memory_size_ = code_generator.grid_memory_size();
    temporary_memory_size_ = code_generator.temporary_memory_size();
}

void Shader::load_memory( const char* start, const char* finish, ErrorPolicy& error_policy )
{
    SymbolTable symbol_table;
    load_memory( start, finish, symbol_table, error_policy );
}

void Shader::load_memory( const char* start, const char* finish, SymbolTable& symbol_table, ErrorPolicy& error_policy )
{
    REYES_ASSERT( start );
    REYES_ASSERT( finish );
    REYES_ASSERT( start <= finish );
    
    ShaderParser shader_parser( &symbol_table, &error_policy );
    shared_ptr<SyntaxNode> syntax_node = shader_parser.parse( start, finish );

    SemanticAnalyzer semantic_analyzer( &symbol_table, &error_policy );
    semantic_analyzer.analyze( syntax_node.get(), "from memory" );

    CodeGenerator code_generator( &error_policy );
    code_generator.generate( syntax_node.get(), "from memory" );
    
    constants_ = code_generator.constant_data();
    symbols_.swap( code_generator.symbols() );
    values_.swap( code_generator.values() );
    code_ = code_generator.code();

    initialize_address_ = code_generator.initialize_address();
    shade_address_ = code_generator.shade_address();
    parameters_ = code_generator.parameters();
    variables_ = code_generator.variables();
    constant_memory_size_ = code_generator.constant_memory_size();
    grid_memory_size_ = code_generator.grid_memory_size();
    temporary_memory_size_ = code_generator.temporary_memory_size();
}
