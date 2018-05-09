#ifndef REYES_RENDERER_HPP_INCLUDED
#define REYES_RENDERER_HPP_INCLUDED

#include "declspec.hpp"
#include <math/vec3.hpp>
#include <math/vec4.hpp>
#include <math/mat4x4.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <string>

namespace sweet
{

namespace render
{

class ErrorPolicy;
class SampleBuffer;
class ImageBuffer;
class Options;
class Attributes;
class SymbolTable;
class VirtualMachine;
class Sampler;
class Value;
class Grid;
class Geometry;
class Texture;
class Shader;

/**
// The main interface to the renderer.
*/
class REYES_DECLSPEC Renderer
{
    ErrorPolicy* error_policy_; ///< The error policy that errors are reported to.
    SymbolTable* symbol_table_; ///< The symbol table used to store symbols when compiling shaders.
    VirtualMachine* virtual_machine_; ///< The virtual machine used to execute shaders.
    Shader* null_surface_shader_; ///< The null surface shader used when no surface shader is set.
    SampleBuffer* sample_buffer_; ///< The sample buffer that grids are sampled into.
    ImageBuffer* image_buffer_; ///< The image buffer that the final image is filtered, exposed, and quantized into.
    Sampler* sampler_; ///< The sampler that samples grids into the sample buffer.
    math::mat4x4 screen_transform_; ///< Transform camera space to screen space.    
    math::mat4x4 camera_transform_; ///< Transform world space to camera space.
    std::map<std::string, Texture*> textures_; ///< The textures that have been loaded (by filename).
    std::map<std::string, Shader*> shaders_; ///< The shaders that have been loaded (by filename).
    Options* options_; /// The options used for this renderer.
    std::vector<std::shared_ptr<Attributes>> attributes_; ///< The attributes stack.

    public:
        Renderer();
        ~Renderer();
                        
        ErrorPolicy& error_policy() const;
        SymbolTable& symbol_table() const;
        
        void set_options( const Options& options );
        const Options& options() const;

        void push_attributes();
        void pop_attributes();
        Attributes& attributes() const;
        
        void shading_rate( float shading_rate );
        void matte( bool matte );
        void two_sided( bool two_sided );
        void orient_inside();
        void orient_outside();
        void orient_left_handed();
        void orient_right_handed();
        void color( const math::vec3& color );        
        void opacity( const math::vec3& opacity );
        
        void begin();
        void end();        
        void begin_world();
        void end_world();
        void projection();
        const math::mat4x4& screen_transform() const;
        const math::mat4x4& camera_transform() const;
        
        void add_coordinate_system( const char* name, const math::mat4x4& transform );
        void remove_coordinate_system( const char* name );
        math::mat4x4 transform_from( const std::string& name ) const;
        math::mat4x4 transform_to( const std::string& name ) const;
        math::mat4x4 transform_between( const std::string& from, const std::string& to ) const;        

        void begin_transform();
        void end_transform();
        void identity();
        void transform( const math::mat4x4& transform );
        void concat_transform( const math::mat4x4& transform );
        void orthographic();
        void perspective( float fov );
        void translate( float x, float y, float z );
        void translate( const math::vec3& translation );
        void rotate( float angle, float x, float y, float z );
        void scale( float x, float y, float z );
        void look_at( const math::vec3& at, const math::vec3& eye, const math::vec3& up );
        const math::mat4x4& current_transform() const;
        
        Grid& displacement_shader( Shader* displacement_shader );
        Grid& surface_shader( Shader* surface_shader );        
        Grid& light_shader( Shader* light_shader );
        Grid& displacement_shader( const char* filename );
        Grid& surface_shader( const char* filename );
        Grid& light_shader( const char* filename );
        void activate_light_shader( const Grid& grid );
        void deactivate_light_shader( const Grid& grid );

        void cone( float height, float radius, float thetamax );
        void sphere( float radius );
        void sphere( float radius, float zmin, float zmax, float thetamax );
        void cylinder( float radius, float zmin, float zmax, float thetamax );
        void hyperboloid( const math::vec3& point1, const math::vec3& point2, float thetamax );
        void paraboloid( float rmax, float zmin, float zmax, float thetamax );
        void disk( float height, float radius, float thetamax );
        void torus( float rmajor, float rminor, float phimin, float phimax, float thetamax );
        void polygon( int vertices, const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates );
        void cubic_patch( const math::vec3* positions );
        void linear_patch( const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates );
        void polygon_mesh( int polygons, const int* vertices, const int* indices, const math::vec3* positions, const math::vec3* normals, const math::vec2* texture_coordinates );

        void split( const Geometry& geometry );        
        void displacement_shade( Grid& grid );
        void surface_shade( Grid& grid );
        void light_shade( Grid& grid );
        void sample( const Grid& grid );
        
        void save_image( const char* format, ... ) const;
        void save_image_as_png( const char* format, ... ) const;
        void save_samples( int mode, const char* format, ... ) const;
        void save_samples_as_png( int mode, const char* format, ... ) const;

        void texture( const char* filename );
        void environment( const char* filename );
        void cubic_environment( const char* filename );
        void shadow_from_framebuffer( const char* name );
        void texture_from_framebuffer( const char* name );
        Texture* find_texture( const char* filename ) const;

        Shader* shader( const char* filename );
        Shader* find_shader( const char* filename ) const;

        const math::vec4* bezier_basis() const;
        const math::vec4* bspline_basis() const;
        const math::vec4* catmull_rom_basis() const;
        const math::vec4* hermite_rom_basis() const;
        const math::vec4* power_basis() const;

        math::vec4 raster( const math::vec3& x ) const;
        float min( float a, float b, float c, float d ) const;
        float max( float a, float b, float c, float d ) const;
        float lb( float x ) const;
};

}

}

#endif
