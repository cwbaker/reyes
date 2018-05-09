#ifndef REYES_MATRIX_FUNCTIONS_HPP_INCLUDED
#define REYES_MATRIX_FUNCTIONS_HPP_INCLUDED

#include <memory>

namespace sweet
{

namespace render
{

class Grid;
class Value;
class Renderer;

void comp_matrix( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> row, std::shared_ptr<Value> column );
void setcomp_matrix( const Renderer& renderer, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> row, std::shared_ptr<Value> column, std::shared_ptr<Value> value );
void determinant( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix );
void translate_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> t );
void rotate_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> angle, std::shared_ptr<Value> axis );
void scale_matrix( const Renderer& render, const Grid& grid, std::shared_ptr<Value> result, std::shared_ptr<Value> matrix, std::shared_ptr<Value> s );

}

}

#endif
