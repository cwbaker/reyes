//
// Debugger.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "Debugger.hpp"
#include "Symbol.hpp"
#include "Grid.hpp"
#include "SampleBuffer.hpp"
#include "SyntaxNode.hpp"
#include "Shader.hpp"
#include "ValueStorage.hpp"
#include <reyes/reyes_virtual_machine/Instruction.hpp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"
#include <stdio.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES
#include <math.h>

using std::string;
using std::vector;
using std::shared_ptr;
using namespace math;
using namespace reyes;

void Debugger::dump_syntax_tree( const SyntaxNode* node, int level ) const
{
    REYES_ASSERT( node );
    
    if ( node )
    {
        for ( int i = 0; i < level; ++i )
        {
            printf( " " );
        }
        
        const char* NODE_TYPES[] =
        {
            "NULL",
            "LIST",
            "LIGHT_SHADER",
            "SURFACE_SHADER",
            "VOLUME_SHADER",
            "DISPLACEMENT_SHADER",
            "IMAGER_SHADER",
            "FLOAT_TYPE",
            "STRING_TYPE",
            "COLOR_TYPE",
            "POINT_TYPE",
            "VECTOR_TYPE",
            "NORMAL_TYPE",
            "MATRIX_TYPE",
            "VOID_TYPE",
            "VARYING",
            "UNIFORM",
            "OUTPUT",
            "EXTERN",
            "FUNCTION",
            "VARIABLE",
            "TYPE",
            "CALL",
            "RETURN",
            "BREAK",
            "CONTINUE",
            "IF",
            "IF_ELSE",
            "WHILE",
            "FOR",
            "AMBIENT",
            "SOLAR",
            "ILLUMINATE",
            "ILLUMINANCE",
            "STATEMENT",
            "DOT",
            "CROSS",
            "MULTIPLY",
            "DIVIDE",
            "ADD",
            "SUBTRACT",
            "GREATER",
            "GREATER_EQUAL",
            "LESS",
            "LESS_EQUAL",
            "EQUAL",
            "NOT_EQUAL",
            "AND",
            "OR",
            "NEGATE",
            "TERNARY",
            "TYPECAST",
            "ASSIGN",    
            "ADD_ASSIGN",
            "SUBTRACT_ASSIGN",
            "MULTIPLY_ASSIGN",
            "DIVIDE_ASSIGN",
            "INTEGER",
            "REAL",
            "STRING",
            "TEXTURE",
            "SHADOW",
            "ENVIRONMENT",
            "TRIPLE",
            "SIXTEENTUPLE",
            "IDENTIFIER",
            "??"
        };
        
        const char* STORAGES [STORAGE_COUNT] =
        {
            "",
            "constant ",
            "uniform ",
            "varying ",
        };
    
        const char* TYPES [TYPE_COUNT] =
        {
            "",
            "int ",
            "float ",
            "color ",
            "point ",
            "vector ",
            "normal ",
            "matrix ",
            "string "
        };
        
        printf( "%s %s ", NODE_TYPES[node->node_type()], STORAGES[node->storage()] );
        if ( !node->lexeme().empty() )
        {
            printf( "'%s' ", node->lexeme().c_str() );            
        }
        shared_ptr<Symbol> symbol = node->symbol();
        if ( symbol )
        {
            printf( "'%s' %s%s", symbol->identifier().c_str(), STORAGES[symbol->storage()], TYPES[symbol->type()] );
        }        
        if ( node->expected_storage() != STORAGE_NULL || node->expected_type() != TYPE_NULL )
        {
            printf( "(%s%s)", STORAGES[node->expected_storage()], TYPES[node->expected_type()] );
        }        
        printf( "\n" );
        
        const vector<shared_ptr<SyntaxNode> >& nodes = node->nodes();
        for ( vector<shared_ptr<SyntaxNode> >::const_iterator i = nodes.begin(); i != nodes.end(); ++i )
        {
            const SyntaxNode* node = i->get();
            REYES_ASSERT( node );
            dump_syntax_tree( node, level + 1 );
        }
    }
}

void Debugger::dump_shader( Shader& shader ) const
{
    printf( "initialize=%d, shade=%d, parameters=%d, variables=%d, constant_memory_size=%d, grid_memory_size=%d, temporary_memory_size=%d\n", 
        shader.initialize_address(),
        shader.shade_address(),
        shader.parameters(),
        shader.variables(),
        shader.constant_memory_size(),
        shader.grid_memory_size(),
        shader.temporary_memory_size()
    );    
    dump_symbols( shader.symbols() );
    dump_code( shader.code() );
}

void Debugger::dump_symbols( const std::vector<std::shared_ptr<Symbol> >& symbols ) const
{
    static const char* SEGMENT[] = 
    {
        "CONSTANT", // SEGMENT_CONSTANT,
        "TEMPORARY", // SEGMENT_TEMPORARY,
        "LIGHT", // SEGMENT_LIGHT,
        "GRID", // SEGMENT_GRID,
        "STRING", // SEGMENT_STRING,
        "NULL", // SEGMENT_NULL
        "??"
    };

    for ( vector<shared_ptr<Symbol> >::const_iterator i = symbols.begin(); i != symbols.end(); ++i )
    {
        const Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        const Address& address = symbol->address();
        printf( "%d, %s, %s, %+d\n", symbol->index(), symbol->identifier().c_str(), SEGMENT[address.segment()], address.offset() );
    }

    printf( "\n\n" );
}

void Debugger::dump_code( const std::vector<unsigned char>& code ) const
{
    struct InstructionMetadata
    {
        const char* name;
        int size;
    };
    
    const InstructionMetadata INSTRUCTIONS [INSTRUCTION_COUNT] = 
    {
        { "null", 0 },
        { "halt", 0 },
        { "reset", 1 },
        { "clear_mask", 0 },
        { "generate_mask", 1 },
        { "invert_mask", 0 },
        { "jump_empty", 0 },
        { "jump_not_empty", 0 },
        { "jump_illuminance", 0 },
        { "jump", 0 },
        { "transform_point", 3 },
        { "transform_vector", 3 },
        { "transform_normal", 3 },
        { "transform_color", 3 },
        { "transform_matrix", 3 },
        { "dot", 3 },
        { "multiply", 3 },
        { "divide", 3 },
        { "add", 3 },
        { "subtract", 3 },
        { "greater", 3 },
        { "greater_equal", 3 },
        { "less", 3 },
        { "less_equal", 3 },
        { "and", 3 },
        { "or", 3 },
        { "equal", 3 },
        { "not_equal", 3 },
        { "negate", 2 },
        { "convert", 2 },
        { "promote", 2 },
        { "assign", 2 },
        { "add_assign", 2 },
        { "subtract_assign", 2 },
        { "multiply_assign", 2 },
        { "divide_assign", 2 },
        { "assign_string", 2 },
        { "float_texture", 4 },
        { "vec3_texture", 4 },
        { "float_environment", 3 },
        { "vec3_environment", 3 },
        { "shadow", 4 },
        { "call", 0 },
        { "ambient", 2 },
        { "solar", 0 },
        { "solar_axis_angle", 4 },
        { "illuminate", 5 },
        { "illuminate_axis_angle", 7 },
        { "illuminance_axis_angle", 7 }
    };

    const unsigned char* begin = code.data();
    const unsigned char* end = begin + code.size();
    const unsigned char* i = begin;
    while ( i < end )
    {        
        int instruction = *reinterpret_cast<const char*>( i );
        REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction <= INSTRUCTION_COUNT );
        const char* name = INSTRUCTIONS[instruction].name;
        int size = INSTRUCTIONS[instruction].size;
        printf( "%ld: %s", i - begin, name );
        i += sizeof(int);
        if ( instruction == INSTRUCTION_CALL )
        {
            REYES_ASSERT( size == 0 );
            int index = *reinterpret_cast<const int*>( i );
            printf( " %d", index );
            i += sizeof(int);
            size = *reinterpret_cast<const int*>( i ) + 1;
            i += sizeof(int);
        }
        else if ( instruction == INSTRUCTION_JUMP || instruction == INSTRUCTION_JUMP_EMPTY || instruction == INSTRUCTION_JUMP_NOT_EMPTY || instruction == INSTRUCTION_JUMP_ILLUMINANCE )
        {
            REYES_ASSERT( size == 0 );
            int jump_distance = *reinterpret_cast<const int*>( i );
            i += sizeof(int);
            printf( ", %d (%ld)", jump_distance, i + jump_distance - begin );
        }

        for ( int j = 0; j < size; ++j )
        {
            Address address = Address( *reinterpret_cast<const int*>(i) );
            printf( " %d:%d", address.segment(), address.offset() );
            i += sizeof(int);
        }
        printf( "\n" );
    }
    
    printf( "\n\n" );
}

void Debugger::dump_grid( const Grid& grid, const math::vec4& color, const char* format, ... ) const
{
    FILE* stream = stdout;
    if ( format )
    {
        char filename [1024];
        va_list args;
        va_start( args, format );
        vsnprintf( filename, sizeof(filename), format, args );
        va_end( args );
        
        stream = fopen( filename, "wb" );
        REYES_ASSERT( stream );
    }

    fprintf( stream, "\n" );
    fprintf( stream, "mesh {\n" );
    fprintf( stream, "   primitive lines\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   format {\n" );
    fprintf( stream, "       0 position float32 3\n" );
    fprintf( stream, "       0 color float32 4\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   vertices {\n" );

    const vec3* positions = grid.vec3_value( "P" );
    const int width = grid.width();
    const int height = grid.height();
    int i = 0;
    int index = 0;    

    for ( int y = 0; y < height - 1; ++y )
    {
        for ( int x = 0; x < width - 1; ++x )
        {
            int i0 = i + x;
            int i1 = i + width + x;
            int i2 = i + width + x + 1;
            int i3 = i + x + 1;

            fprintf( stream, "       // #%d/%d, %d, %d, %d, %d\n", index, index + 1, i0, i1, i2, i3 );
            fprintf( stream, "       %f %f %f\n", positions[i0].x, positions[i0].y, positions[i0].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i1].x, positions[i1].y, positions[i1].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i1].x, positions[i1].y, positions[i1].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i2].x, positions[i2].y, positions[i2].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i2].x, positions[i2].y, positions[i2].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i3].x, positions[i3].y, positions[i3].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i3].x, positions[i3].y, positions[i3].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i0].x, positions[i0].y, positions[i0].z );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "\n" );
            
            index += 2;
        }
        i += width;
    }

    fprintf( stream, "   }\n" );
    fprintf( stream, "   indices {\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "}\n" );
    
    if ( stream != stdout )
    {
        fclose( stream );
        stream = NULL;
    }
}

void Debugger::dump_sample_buffer( const SampleBuffer& sample_buffer, const math::vec4& color, const math::mat4x4& screen_transform, const math::vec4& crop_window, const char* format, ... ) const
{
    FILE* stream = stdout;
    if ( format )
    {
        char filename [1024];
        va_list args;
        va_start( args, format );
        vsnprintf( filename, sizeof(filename), format, args );
        va_end( args );
        
        stream = fopen( filename, "wb" );
        REYES_ASSERT( stream );
    }

    fprintf( stream, "\n" );
    fprintf( stream, "mesh {\n" );
    fprintf( stream, "   primitive lines\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   format {\n" );
    fprintf( stream, "       0 position float32 3\n" );
    fprintf( stream, "       0 color float32 4\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   vertices {\n" );

    const int width = sample_buffer.width();
    const int height = sample_buffer.height();
    const float dx = 1.0f / float(width - 1);
    const float dy = 1.0f / float(height - 1);
    const int x0 = floorf( crop_window.x * (width + 1) );
    const int x1 = floorf( crop_window.y * (width + 1) ) + 1;
    const int y0 = floorf( crop_window.z * (height + 1) );
    const int y1 = floorf( crop_window.w * (height + 1) ) + 1;
    
    const mat4x4 inverse_screen_transform = inverse( screen_transform );
    
    for ( int y = y0; y < y1; ++y )
    {
        const float* left = sample_buffer.position( x0, y );
        vec3 xx0 = unproject( left, inverse_screen_transform, float(width), float(height) ) - vec3( dx, 0.0f, 0.0f );
        fprintf( stream, "       // y=%d\n", y );
        fprintf( stream, "       %f %f %f\n", xx0.x, xx0.y, xx0.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );

        const float* right = sample_buffer.position( x1 - 1, y );
        vec3 xx1 = unproject( right, inverse_screen_transform, float(width), float(height) ) + vec3( dx, 0.0f, 0.0f );
        fprintf( stream, "       %f %f %f\n", xx1.x, xx1.y, xx1.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
    }

    for ( int x = x0; x < x1; ++x )
    {        
        const float* bottom = sample_buffer.position( x, y1 - 1 );
        vec3 yy0 = unproject( bottom, inverse_screen_transform, float(width), float(height) ) - vec3( 0.0f, dy, 0.0f );
        fprintf( stream, "       // x=%d\n", x );
        fprintf( stream, "       %f %f %f\n", yy0.x, yy0.y, yy0.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );

        const float* top = sample_buffer.position( x, y0 );
        vec3 yy1 = unproject( top, inverse_screen_transform, float(width), float(height) ) + vec3( 0.0f, dy, 0.0f );
        fprintf( stream, "       %f %f %f\n", yy1.x, yy1.y, yy1.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
    }

    fprintf( stream, "   }\n" );
    fprintf( stream, "   indices {\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "}\n" );
    
    if ( stream != stdout )
    {
        fclose( stream );
        stream = NULL;
    }
}

void Debugger::dump_samples( int x0, int x1, int y0, int y1, const int* bounds, const int* indices, const float* positions, int polygons, const math::vec4& color, const char* format, ... ) const
{
    REYES_ASSERT( x0 <= x1 );
    REYES_ASSERT( y0 <= y1 );
    REYES_ASSERT( bounds );
    REYES_ASSERT( indices );
    REYES_ASSERT( positions );

    FILE* stream = stdout;
    if ( format )
    {
        char filename [1024];
        va_list args;
        va_start( args, format );
        vsnprintf( filename, sizeof(filename), format, args );
        va_end( args );
        
        stream = fopen( filename, "wb" );
        REYES_ASSERT( stream );
    }

    fprintf( stream, "\n" );
    fprintf( stream, "mesh {\n" );
    fprintf( stream, "   primitive lines\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   format {\n" );
    fprintf( stream, "       0 position float32 3\n" );
    fprintf( stream, "       0 color float32 4\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   vertices {\n" );

    for ( int i = 0; i < polygons; ++i )
    {
        int sx0 = bounds[i * 4 + 0];
        int sx1 = bounds[i * 4 + 1];
        int sy0 = bounds[i * 4 + 2];
        int sy1 = bounds[i * 4 + 3];

        if ( sx1 >= x0 && sx0 <= x1 && sy1 >= y0 && sy0 <= y1 )
        {
            int i0 = indices[i * 3 + 0];
            int i1 = indices[i * 3 + 1];
            int i2 = indices[i * 3 + 2];

            fprintf( stream, "       // #%d: %d, %d, %d\n", i, i0, i1, i2 );
            fprintf( stream, "       %f %f %f\n", positions[i0 * 4 + 0], positions[i0 * 4 + 1], positions[i0 * 4 + 2] );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i1 * 4 + 0], positions[i1 * 4 + 1], positions[i1 * 4 + 2] );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i1 * 4 + 0], positions[i1 * 4 + 1], positions[i1 * 4 + 2] );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i2 * 4 + 0], positions[i2 * 4 + 1], positions[i2 * 4 + 2] );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i2 * 4 + 0], positions[i2 * 4 + 1], positions[i2 * 4 + 2] );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "       %f %f %f\n", positions[i0 * 4 + 0], positions[i0 * 4 + 1], positions[i0 * 4 + 2] );
            fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
            fprintf( stream, "\n" );
        }
    }

    fprintf( stream, "   }\n" );
    fprintf( stream, "   indices {\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "}\n" );
    
    if ( stream != stdout )
    {
        fclose( stream );
        stream = NULL;
    }
}

void Debugger::dump_micropolygons( const int* indices, const float* raster_positions, const int* polygons, int size, const math::vec4& color, const math::mat4x4& screen_transform, float width, float height, const char* format, ... ) const
{
    REYES_ASSERT( indices );
    REYES_ASSERT( raster_positions );
    REYES_ASSERT( size >= 0 );
    REYES_ASSERT( width > 0.0f );
    REYES_ASSERT( height > 0.0f );

    FILE* stream = stdout;
    if ( format )
    {
        char filename [1024];
        va_list args;
        va_start( args, format );
        vsnprintf( filename, sizeof(filename), format, args );
        va_end( args );
        
        stream = fopen( filename, "wb" );
        REYES_ASSERT( stream );
    }

    fprintf( stream, "\n" );
    fprintf( stream, "mesh {\n" );
    fprintf( stream, "   primitive lines\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   format {\n" );
    fprintf( stream, "       0 position float32 3\n" );
    fprintf( stream, "       0 color float32 4\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "\n" );
    fprintf( stream, "   vertices {\n" );

    mat4x4 inverse_screen_transform = inverse( screen_transform );

    for ( int i = 0; i < size; ++i )
    {
        int polygon = polygons ? polygons[i] : i;
             
        int i0 = indices[polygon * 3 + 0];
        int i1 = indices[polygon * 3 + 1];
        int i2 = indices[polygon * 3 + 2];
        
        vec3 p0 = unproject( &raster_positions[i0 * 4], inverse_screen_transform, width, height );
        vec3 p1 = unproject( &raster_positions[i1 * 4], inverse_screen_transform, width, height );
        vec3 p2 = unproject( &raster_positions[i2 * 4], inverse_screen_transform, width, height );

        fprintf( stream, "       // #%d: %d, %d, %d\n", polygon, i0, i1, i2 );
        fprintf( stream, "       %f %f %f\n", p0.x, p0.y, p0.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
        fprintf( stream, "       %f %f %f\n", p1.x, p1.y, p1.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
        fprintf( stream, "       %f %f %f\n", p1.x, p1.y, p1.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
        fprintf( stream, "       %f %f %f\n", p2.x, p2.y, p2.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
        fprintf( stream, "       %f %f %f\n", p2.x, p2.y, p2.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
        fprintf( stream, "       %f %f %f\n", p0.x, p0.y, p0.z );
        fprintf( stream, "       %f %f %f %f\n", color.x, color.y, color.z, color.w );
        fprintf( stream, "\n" );
    }

    fprintf( stream, "   }\n" );
    fprintf( stream, "   indices {\n" );
    fprintf( stream, "   }\n" );
    fprintf( stream, "}\n" );
    
    if ( stream != stdout )
    {
        fclose( stream );
        stream = NULL;
    }
}

math::vec3 Debugger::unproject( const float* raster_position, const math::mat4x4& inverse_screen_transform, float width, float height ) const
{
    REYES_ASSERT( raster_position );
    float x = 2.0f * raster_position[0] / width - 1.0f;
    float y = 2.0f * (height - raster_position[1]) / height - 1.0f;
    return vec3( inverse_screen_transform * vec4(x, y, 0.0f, 1.0f) );
}
