#pragma once

#include <math/vec3.hpp>
#include <math/mat4x4.hpp>
#include <string>

namespace reyes
{

class Grid;

class SetValueHelper
{
    Grid* grid_;
    int offset_;

public:
    SetValueHelper();
    SetValueHelper( Grid* grid, int offset );
    SetValueHelper& operator=( float value );
    SetValueHelper& operator=( const math::vec3& value );
    SetValueHelper& operator=( const math::mat4x4& value );
    SetValueHelper& operator=( const char* value );
    SetValueHelper& operator=( const std::string& value );
};

}
