#ifndef REYES_DEBUGGER_HPP_INCLUDED
#define REYES_DEBUGGER_HPP_INCLUDED

#include <math/vec4.hpp>
#include <math/mat4x4.hpp>
#include <vector>
#include <memory>

namespace reyes
{

class SyntaxNode;
class Symbol;
class Shader;
class Value;
class Grid;
class SampleBuffer;

class Debugger
{
public:
    void dump_syntax_tree( const SyntaxNode* node, int level = 0 ) const;
    void dump_shader( Shader& shader ) const;
    void dump_registers( int parameters, const std::vector<std::shared_ptr<Symbol> >& symbols, const std::vector<std::shared_ptr<Value> >& values ) const;
    void dump_symbols( const std::vector<std::shared_ptr<Symbol> >& symbols ) const;
    void dump_values( const std::vector<std::shared_ptr<Value> >& values ) const;
    void dump_code( const std::vector<unsigned char>& code ) const;
    void dump_grid( const Grid& grid, const math::vec4& color, const char* format = NULL, ... ) const;
    void dump_sample_buffer( const SampleBuffer& sample_buffer, const math::vec4& color, const math::mat4x4& screen_transform, const math::vec4& crop_window, const char* format = NULL, ... ) const;
    void dump_samples( int x0, int x1, int y0, int y1, const int* bounds, const int* indices, const float* positions, int polygons, const math::vec4& color, const char* format = NULL, ... ) const;
    void dump_micropolygons( const int* indices, const float* raster_positions, const int* polygons, int size, const math::vec4& color, const math::mat4x4& screen_transform, float width, float height, const char* format = NULL, ... ) const;

private:
    math::vec3 unproject( const float* raster_position, const math::mat4x4& inverse_screen_transform, float width, float height ) const;
};

}

#endif
