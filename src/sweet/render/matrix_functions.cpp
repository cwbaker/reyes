
#include "stdafx.hpp"
#include "matrix_functions.hpp"
#include "Grid.hpp"
#include "Value.hpp"
#include "Renderer.hpp"
#include <math/scalar.ipp>
#include <math/vec3.ipp>
#include <math/mat4x4.ipp>
#include <sweet/assert/assert.hpp>

using namespace sweet;
using namespace sweet::math;

namespace sweet
{

namespace render
{

void comp_matrix( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> row_value, std::shared_ptr<Value> column_value )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( matrix );
    SWEET_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( row_value );
    SWEET_ASSERT( row_value->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( column_value );
    SWEET_ASSERT( column_value->storage() == STORAGE_UNIFORM );
    
    result->reset( TYPE_FLOAT, STORAGE_UNIFORM, 1 );
    
    const mat4x4& m = matrix->mat4x4_value();
    int row = int(row_value->float_value());
    int column = int(column_value->float_value());
    float* values = result->float_values();
    values[0] = m.m[row * 4 + column];
}

void setcomp_matrix( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> row_value, std::shared_ptr<Value> column_value, std::shared_ptr<Value> value )
{
    SWEET_ASSERT( matrix );
    SWEET_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( row_value );
    SWEET_ASSERT( row_value->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( column_value );
    SWEET_ASSERT( column_value->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( value );
    SWEET_ASSERT( value->storage() == STORAGE_UNIFORM );

    const mat4x4& m = matrix->mat4x4_value();
    int row = int(row_value->float_value());
    int column = int(column_value->float_value());
    mat4x4* values = matrix->mat4x4_values();
    values[0].m[row * 4 + column] = value->float_value();
}

void determinant( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix )
{
    SWEET_ASSERT( matrix );
    SWEET_ASSERT( matrix->storage() == STORAGE_UNIFORM );

    result->reset( TYPE_FLOAT, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    float* values = result->float_values();
    values[0] = math::determinant( m );
}

void translate_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> t )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( matrix );
    SWEET_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( t );
    SWEET_ASSERT( t->storage() == STORAGE_UNIFORM );    

    result->reset( TYPE_MATRIX, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    mat4x4* values = result->mat4x4_values();
    values[0] = m * math::translate( t->vec3_value() );
}

void rotate_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> angle, std::shared_ptr<Value> axis )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( matrix );
    SWEET_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( angle );
    SWEET_ASSERT( angle->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( axis );
    SWEET_ASSERT( axis->storage() == STORAGE_UNIFORM );

    result->reset( TYPE_MATRIX, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    mat4x4* values = result->mat4x4_values();
    values[0] = m * math::rotate( axis->vec3_value(), angle->float_value() );
}

void scale_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> s )
{
    SWEET_ASSERT( result );
    SWEET_ASSERT( matrix );
    SWEET_ASSERT( matrix->storage() == STORAGE_UNIFORM );
    SWEET_ASSERT( s );
    SWEET_ASSERT( s->storage() == STORAGE_UNIFORM );

    result->reset( TYPE_MATRIX, STORAGE_UNIFORM, 1 );
    const mat4x4& m = matrix->mat4x4_value();
    mat4x4* values = result->mat4x4_values();
    values[0] = m * math::scale( s->vec3_value() );
}

}

}
