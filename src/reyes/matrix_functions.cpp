
#include "stdafx.hpp"
#include "matrix_functions.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include "Renderer.hpp"
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include "assert.hpp"

using namespace sweet;
using namespace sweet::math;

namespace sweet
{

namespace reyes
{

void comp_matrix( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> row_value, std::shared_ptr<Value> column_value )
{
    REYES_ASSERT( result );
    REYES_ASSERT( matrix );
    REYES_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( row_value );
    REYES_ASSERT( row_value->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( column_value );
    REYES_ASSERT( column_value->storage() == STORAGE_UNIFORM );
    
    result->reset( TYPE_FLOAT, STORAGE_UNIFORM, 1 );
    
    const mat4x4& m = matrix->mat4x4_value();
    int row = int(row_value->float_value());
    int column = int(column_value->float_value());
    float* values = result->float_values();
    values[0] = m.m[row * 4 + column];
}

void setcomp_matrix( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> row_value, std::shared_ptr<Value> column_value, std::shared_ptr<Value> value )
{
    REYES_ASSERT( matrix );
    REYES_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( row_value );
    REYES_ASSERT( row_value->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( column_value );
    REYES_ASSERT( column_value->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( value );
    REYES_ASSERT( value->storage() == STORAGE_UNIFORM );

    const mat4x4& m = matrix->mat4x4_value();
    int row = int(row_value->float_value());
    int column = int(column_value->float_value());
    mat4x4* values = matrix->mat4x4_values();
    values[0].m[row * 4 + column] = value->float_value();
}

void determinant( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix )
{
    REYES_ASSERT( matrix );
    REYES_ASSERT( matrix->storage() == STORAGE_UNIFORM );

    result->reset( TYPE_FLOAT, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    float* values = result->float_values();
    values[0] = math::determinant( m );
}

void translate_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> t )
{
    REYES_ASSERT( result );
    REYES_ASSERT( matrix );
    REYES_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( t );
    REYES_ASSERT( t->storage() == STORAGE_UNIFORM );    

    result->reset( TYPE_MATRIX, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    mat4x4* values = result->mat4x4_values();
    values[0] = m * math::translate( t->vec3_value() );
}

void rotate_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> angle, std::shared_ptr<Value> axis )
{
    REYES_ASSERT( result );
    REYES_ASSERT( matrix );
    REYES_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( angle );
    REYES_ASSERT( angle->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( axis );
    REYES_ASSERT( axis->storage() == STORAGE_UNIFORM );

    result->reset( TYPE_MATRIX, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    mat4x4* values = result->mat4x4_values();
    values[0] = m * math::rotate( axis->vec3_value(), angle->float_value() );
}

void scale_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> s )
{
    REYES_ASSERT( result );
    REYES_ASSERT( matrix );
    REYES_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    REYES_ASSERT( s );
    REYES_ASSERT( s->storage() == STORAGE_UNIFORM );

    result->reset( TYPE_MATRIX, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    mat4x4* values = result->mat4x4_values();
    values[0] = m * math::scale( s->vec3_value() );
}

}

}
