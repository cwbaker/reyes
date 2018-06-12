
#include "stdafx.hpp"
#include "Debugger.hpp"
#include "Symbol.hpp"
#include "Value.hpp"
#include "Grid.hpp"
#include "SampleBuffer.hpp"
#include "SyntaxNode.hpp"
#include "Shader.hpp"
#include "Instruction.hpp"
#include "ValueStorage.hpp"
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
        
        printf( "%s %s'%s' ", NODE_TYPES[node->node_type()], STORAGES[node->storage()], node->lexeme().c_str() );
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
    printf( "initialize=%d, shade=%d, parameters=%d, variables=%d, constants=%d, permanent_registers=%d, registers=%d\n", 
        shader.initialize_address(),
        shader.shade_address(),
        shader.parameters(),
        shader.variables(),
        shader.constants(),
        shader.permanent_registers(),
        shader.registers()
    );    
    dump_registers( shader.parameters(), shader.symbols(), shader.values() );
    dump_symbols( shader.symbols() );
    dump_code( shader.code() );
}

void Debugger::dump_registers( int parameters, const std::vector<shared_ptr<Symbol> >& symbols, const std::vector<shared_ptr<Value> >& values ) const
{
    vector<shared_ptr<Symbol> >::const_iterator i = symbols.begin();
    vector<shared_ptr<Value> >::const_iterator j = values.begin();

    int register_index = 0;                
    while ( i != symbols.end() && j != values.end() && register_index < parameters )
    {
        const Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        
        printf( "%d, %d, %s\n", register_index, symbol->register_index(), symbol->identifier().c_str() );
        ++i;
        ++j;
        ++register_index;
    }
    
    while ( j != values.end() )
    {
        printf( "%d, CONSTANT\n", register_index );
        ++j;
        ++register_index;
    }
    
    while ( i != symbols.end() )
    {
        const Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        printf( "%d, %d, %s\n", register_index, symbol->register_index(), symbol->identifier().c_str() );
        ++i;
        ++register_index;
    }

    printf( "\n\n" );
}

void Debugger::dump_symbols( const std::vector<std::shared_ptr<Symbol> >& symbols ) const
{
    for ( vector<shared_ptr<Symbol> >::const_iterator i = symbols.begin(); i != symbols.end(); ++i )
    {
        const Symbol* symbol = i->get();
        REYES_ASSERT( symbol );
        printf( "%d, %s\n", symbol->index(), symbol->identifier().c_str() );
    }

    printf( "\n\n" );
}

void Debugger::dump_values( const std::vector<std::shared_ptr<Value> >& values ) const
{
    int index = 0;
    for ( vector<shared_ptr<Value> >::const_iterator i = values.begin(); i != values.end(); ++i )
    {
        const Value* value = i->get();
        REYES_ASSERT( value );
        switch ( value->type() )
        {
            case TYPE_FLOAT:
                printf( "%d, float, %d, %.02f\n", index, value->size(), *value->float_values() );
                break;
                
            case TYPE_COLOR:
            case TYPE_POINT:
            case TYPE_VECTOR:
            case TYPE_NORMAL:
            {
                const vec3& vec3_value = *value->vec3_values();
                printf( "%d, vec3, %d, (%.02f, %.02f, %.02f)\n", index, value->size(), vec3_value.x, vec3_value.y, vec3_value.z );
                break;
            }
                
            case TYPE_STRING:
            {
                const string& string_value = value->string_value();
                printf( "%d, string, '%s'\n", index, string_value.c_str() );
                break;
            }
                
            default:
                printf( "%d, unknown\n", index );
        }
        ++index;
    }

    printf( "\n\n" );
}

void Debugger::dump_code( const std::vector<unsigned char>& code ) const
{
    /*
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
        { "jump_empty", 1 },
        { "jump_not_empty", 1 },
        { "jump_illuminance", 1 },
        { "jump", 1 },
        { "transform", 2 },
        { "transform_vector", 2 },
        { "transform_normal", 2 },
        { "transform_color", 2 },
        { "transform_matrix", 2 },
        { "dot", 2 },
        { "multiply_float", 2 },
        { "multiply_vec3", 2 },
        { "divide_float", 2 },
        { "divide_vec3", 2 },
        { "add_float", 2 },
        { "add_vec3", 2 },
        { "subtract_float", 2 },
        { "subtract_vec3", 2 },
        { "greater", 2 },
        { "greater_equal", 2 },
        { "less", 2 },
        { "less_equal", 2 },
        { "and", 2 },
        { "or", 2 },
        { "equal_float", 2 },
        { "equal_vec3", 2 },
        { "not_equal_float", 2 },
        { "not_equal_vec3", 2 },
        { "negate", 1 },
        { "promote_integer", 1 },
        { "promote_float", 1 },
        { "promote_vec3", 1 },
        { "float_to_color", 1 },
        { "float_to_point", 1 },
        { "float_to_vector", 1 },
        { "float_to_normal", 1 },
        { "float_to_matrix", 1 },
        { "assign_float", 2 },
        { "assign_vec3", 2 },
        { "assign_mat4x4", 2 },
        { "assign_integer", 2 },
        { "assign_string", 2 },
        { "add_assign_float", 2 },
        { "add_assign_vec3", 2 },
        { "multiply_assign_float", 2 },
        { "multiply_assign_vec3", 2 },
        { "float_texture", 3 },
        { "vec3_texture", 3 },
        { "float_environment", 2 },
        { "vec3_environment", 2 },
        { "shadow", 3 },
        { "call", 1 },
        { "call", 2 },
        { "call", 3 },
        { "call", 4 },
        { "call", 5 },
        { "call", 6 },
        { "ambient", 2 },
        { "solar", 0 },
        { "solar_axis_angle", 4 },
        { "illuminate", 5 },
        { "illuminate_axis_angle", 7 },
        { "illuminance_axis_angle", 6 }
    };
    
    const short* i = &code[0];
    const short* instructions_end = i + code.size();
    while ( i < instructions_end )
    {        
        int instruction = *i;
        REYES_ASSERT( instruction >= INSTRUCTION_NULL && instruction <= INSTRUCTION_COUNT );
        const char* name = INSTRUCTIONS[instruction].name;
        int size = INSTRUCTIONS[instruction].size;        
        printf( "%ld: %s", i - &code[0], name );
        ++i;
        if ( instruction == INSTRUCTION_JUMP || instruction == INSTRUCTION_JUMP_EMPTY || instruction == INSTRUCTION_JUMP_NOT_EMPTY || instruction == INSTRUCTION_JUMP_ILLUMINANCE )
        {
            REYES_ASSERT( size == 1 );
            int jump_distance = *i;
            ++i;
            printf( ", %d (%ld)", jump_distance, i + jump_distance - &code[0] );
        }
        else
        {
            for ( int j = 0; j < size; ++j )
            {
                int register_index = *i;
                printf( ", %d", register_index );
                ++i;
            }
        }
        printf( "\n" );
    }
    
    printf( "\n\n" );
    */
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

    const vec3* positions = grid["P"].vec3_values();
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
