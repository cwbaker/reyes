#pragma once

namespace reyes
{

enum SyntaxNodeType
{
    SHADER_NODE_NULL,
    SHADER_NODE_LIST,
    SHADER_NODE_LIGHT_SHADER,
    SHADER_NODE_SURFACE_SHADER,
    SHADER_NODE_VOLUME_SHADER,
    SHADER_NODE_DISPLACEMENT_SHADER,
    SHADER_NODE_IMAGER_SHADER,
    SHADER_NODE_FLOAT_TYPE,
    SHADER_NODE_STRING_TYPE,
    SHADER_NODE_COLOR_TYPE,
    SHADER_NODE_POINT_TYPE,
    SHADER_NODE_VECTOR_TYPE,
    SHADER_NODE_NORMAL_TYPE,
    SHADER_NODE_MATRIX_TYPE,
    SHADER_NODE_VOID_TYPE,
    SHADER_NODE_VARYING,
    SHADER_NODE_UNIFORM,
    SHADER_NODE_OUTPUT,
    SHADER_NODE_EXTERN,
    SHADER_NODE_FUNCTION,
    SHADER_NODE_VARIABLE,
    SHADER_NODE_TYPE,
    SHADER_NODE_CALL,
    SHADER_NODE_RETURN,
    SHADER_NODE_BREAK,
    SHADER_NODE_CONTINUE,
    SHADER_NODE_IF,
    SHADER_NODE_IF_ELSE,
    SHADER_NODE_WHILE,
    SHADER_NODE_FOR,
    SHADER_NODE_AMBIENT,
    SHADER_NODE_SOLAR,
    SHADER_NODE_ILLUMINATE,
    SHADER_NODE_ILLUMINANCE,
    SHADER_NODE_STATEMENT,
    SHADER_NODE_DOT,
    SHADER_NODE_CROSS,
    SHADER_NODE_MULTIPLY,
    SHADER_NODE_DIVIDE,
    SHADER_NODE_ADD,
    SHADER_NODE_SUBTRACT,
    SHADER_NODE_GREATER,
    SHADER_NODE_GREATER_EQUAL,
    SHADER_NODE_LESS,
    SHADER_NODE_LESS_EQUAL,
    SHADER_NODE_EQUAL,
    SHADER_NODE_NOT_EQUAL,
    SHADER_NODE_AND,
    SHADER_NODE_OR,
    SHADER_NODE_NEGATE,
    SHADER_NODE_TERNARY,
    SHADER_NODE_TYPECAST,
    SHADER_NODE_ASSIGN,    
    SHADER_NODE_ADD_ASSIGN,
    SHADER_NODE_SUBTRACT_ASSIGN,
    SHADER_NODE_MULTIPLY_ASSIGN,
    SHADER_NODE_DIVIDE_ASSIGN,
    SHADER_NODE_INTEGER,
    SHADER_NODE_REAL,
    SHADER_NODE_STRING,
    SHADER_NODE_TEXTURE,
    SHADER_NODE_SHADOW,
    SHADER_NODE_ENVIRONMENT,
    SHADER_NODE_TRIPLE,
    SHADER_NODE_SIXTEENTUPLE,
    SHADER_NODE_IDENTIFIER,
    SHADER_NODE_COUNT
};

}
