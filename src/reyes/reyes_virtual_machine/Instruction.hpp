#pragma once

namespace reyes
{

/**
// Instructions interpreted by the virtual machine.
*/
enum Instruction
{
    INSTRUCTION_NULL,
    INSTRUCTION_HALT,
    INSTRUCTION_RESET,
    INSTRUCTION_CLEAR_MASK,
    INSTRUCTION_GENERATE_MASK,
    INSTRUCTION_INVERT_MASK,
    INSTRUCTION_JUMP_EMPTY,
    INSTRUCTION_JUMP_NOT_EMPTY,
    INSTRUCTION_JUMP_ILLUMINANCE,
    INSTRUCTION_JUMP,
    INSTRUCTION_TRANSFORM_POINT,
    INSTRUCTION_TRANSFORM_VECTOR,
    INSTRUCTION_TRANSFORM_NORMAL,
    INSTRUCTION_TRANSFORM_COLOR,
    INSTRUCTION_TRANSFORM_MATRIX,
    INSTRUCTION_DOT,
    INSTRUCTION_MULTIPLY,
    INSTRUCTION_DIVIDE,
    INSTRUCTION_ADD,
    INSTRUCTION_SUBTRACT,
    INSTRUCTION_GREATER,
    INSTRUCTION_GREATER_EQUAL,
    INSTRUCTION_LESS,
    INSTRUCTION_LESS_EQUAL,
    INSTRUCTION_AND,
    INSTRUCTION_OR,
    INSTRUCTION_EQUAL,
    INSTRUCTION_NOT_EQUAL,
    INSTRUCTION_NEGATE,
    INSTRUCTION_CONVERT,
    INSTRUCTION_PROMOTE,
    INSTRUCTION_ASSIGN,
    INSTRUCTION_ADD_ASSIGN,
    INSTRUCTION_SUBTRACT_ASSIGN,
    INSTRUCTION_MULTIPLY_ASSIGN,
    INSTRUCTION_DIVIDE_ASSIGN,
    INSTRUCTION_STRING_ASSIGN,
    INSTRUCTION_FLOAT_TEXTURE,
    INSTRUCTION_VEC3_TEXTURE,
    INSTRUCTION_FLOAT_ENVIRONMENT,
    INSTRUCTION_VEC3_ENVIRONMENT,
    INSTRUCTION_SHADOW,
    INSTRUCTION_CALL,
    INSTRUCTION_AMBIENT,
    INSTRUCTION_SOLAR,
    INSTRUCTION_SOLAR_AXIS_ANGLE,
    INSTRUCTION_ILLUMINATE,
    INSTRUCTION_ILLUMINATE_AXIS_ANGLE,
    INSTRUCTION_ILLUMINANCE_AXIS_ANGLE,
    INSTRUCTION_COUNT
};

}
