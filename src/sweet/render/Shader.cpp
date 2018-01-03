//
// Shader.cpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Shader.hpp"
#include "SyntaxNode.hpp"
#include "ShaderParser.hpp"
#include "SemanticAnalyzer.hpp"
#include "CodeGenerator.hpp"
#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include <sweet/assert/assert.hpp>

using std::map;
using std::string;
using std::vector;
using namespace sweet;
using namespace sweet::render;

Shader::Shader()
: symbols_(),
  values_(),
  code_(),
  initialize_address_( 0 ),
  shade_address_( 0 ),
  parameters_( 0 ),
  variables_( 0 ),
  constants_( 0 ),
  permanent_registers_( 0 ),
  registers_( 0 )
{
}

Shader::Shader( const char* filename, SymbolTable& symbol_table, ErrorPolicy& error_policy )
: symbols_(),
  values_(),
  code_(),
  initialize_address_( 0 ),
  shade_address_( 0 ),
  parameters_( 0 ),
  variables_( 0 ),
  constants_( 0 ),
  permanent_registers_( 0 ),
  registers_( 0 )
{
    SWEET_ASSERT( filename );
    
    ShaderParser shader_parser( symbol_table, &error_policy );
    ptr<SyntaxNode> syntax_node = shader_parser.parse( filename );

    SemanticAnalyzer semantic_analyzer( symbol_table, &error_policy );
    semantic_analyzer.analyze( syntax_node.get(), filename );

    CodeGenerator code_generator( symbol_table, &error_policy );
    code_generator.generate( syntax_node.get(), filename );
    
    symbols_.swap( code_generator.symbols() );
    values_.swap( code_generator.values() );
    code_.swap( code_generator.code() );

    initialize_address_ = code_generator.initialize_address();
    shade_address_ = code_generator.shade_address();
    parameters_ = code_generator.parameters();
    variables_ = code_generator.variables();
    constants_ = code_generator.constants();
    permanent_registers_ = code_generator.permanent_registers();
    registers_ = code_generator.registers();
}

Shader::Shader( const char* start, const char* finish, SymbolTable& symbol_table, ErrorPolicy& error_policy )
: symbols_(),
  values_(),
  code_(),
  initialize_address_( 0 ),
  shade_address_( 0 ),
  parameters_( 0 ),
  variables_( 0 ),
  constants_( 0 ),
  permanent_registers_( 0 ),
  registers_( 0 )
{
    SWEET_ASSERT( start );
    SWEET_ASSERT( finish );
    SWEET_ASSERT( start <= finish );
    
    ShaderParser shader_parser( symbol_table, &error_policy );
    ptr<SyntaxNode> syntax_node = shader_parser.parse( start, finish );

    SemanticAnalyzer semantic_analyzer( symbol_table, &error_policy );
    semantic_analyzer.analyze( syntax_node.get(), "from memory" );

    CodeGenerator code_generator( symbol_table, &error_policy );
    code_generator.generate( syntax_node.get(), "from memory" );
    
    symbols_.swap( code_generator.symbols() );
    values_.swap( code_generator.values() );
    code_.swap( code_generator.code() );

    initialize_address_ = code_generator.initialize_address();
    shade_address_ = code_generator.shade_address();
    parameters_ = code_generator.parameters();
    variables_ = code_generator.variables();
    constants_ = code_generator.constants();
    permanent_registers_ = code_generator.permanent_registers();
    registers_ = code_generator.registers();
}

const std::vector<ptr<Symbol> >& Shader::symbols() const
{
    return symbols_;
}

const std::vector<ptr<Value> >& Shader::values() const
{
    return values_;
}

const std::vector<short>& Shader::code() const
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

int Shader::constants() const
{
    return constants_;
}

int Shader::permanent_registers() const
{
    return permanent_registers_;
}

int Shader::registers() const
{
    return registers_;
}

ptr<Symbol> Shader::find_symbol( const std::string& identifier ) const
{
    vector<ptr<Symbol> >::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    return i != symbols_.end() ? *i : ptr<Symbol>();
}
