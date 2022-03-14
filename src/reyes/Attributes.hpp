#ifndef REYES_ATTRIBUTES_HPP_INCLUDED
#define REYES_ATTRIBUTES_HPP_INCLUDED

#include <math/vec3.hpp>
#include <math/vec4.hpp>
#include <math/mat4x4.hpp>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace reyes
{

class Grid;
class Shader;
class VirtualMachine;

/**
// Attributes for a single level of the renderer's attribute stack.
*/
class Attributes
{
    VirtualMachine* virtual_machine_; ///< The VirtualMachine used to initialize shader parameters.
    float shading_rate_; ///< The current shading rate.
    bool matte_; ///< The current matte object flag.
    bool two_sided_; ///< The current two sided object flag.
    bool transform_left_handed_; ///< True if the current transform is left handed (false indicates right handed).
    bool geometry_left_handed_; ///< True if geometry is currently expected to be left handed (false indicates right handed).
    math::vec3 color_; ///< The current color.
    math::vec3 opacity_; ///< The current opacity.
    const math::vec4 *u_basis_; ///< The 4 rows that define the cubic basis in the u direction for patches.
    const math::vec4 *v_basis_; ///< The 4 rows that define the cubic basis in the u direction for patches.
    Grid* displacement_grid_; ///< The grid for the currently active displacement shader.
    Shader* displacement_shader_; ///< The currently active displacement shader or null if there is no displacement shader.
    Grid* surface_grid_; ///< The grid for the currently active surface shader.
    Shader* surface_shader_; ///< The currently active surface shader or null if there is no surface shader.
    std::vector<std::pair<Shader*, std::shared_ptr<Grid> > > light_shaders_; ///< The currently allocated light shaders.
    std::vector<Grid*> active_light_shaders_; ///< The currently active light shaders.
    std::vector<math::mat4x4> transforms_; ///< The transform stack.
    std::map<std::string, math::mat4x4> named_transforms_; ///< Transform from camera space to the named space.
    
public:
    Attributes( VirtualMachine* virtual_machine );
    Attributes( const Attributes& attributes );
    ~Attributes();

    float shading_rate() const;
    bool matte() const;
    bool two_sided() const;
    bool transform_left_handed() const;
    bool geometry_left_handed() const;
    const math::vec3& color() const;
    const math::vec3& opacity() const;
    const math::vec4* u_basis() const;
    const math::vec4* v_basis() const;
    const std::vector<math::mat4x4>& transforms() const;
    const std::map<std::string, math::mat4x4>& named_transforms() const;

    void set_shading_rate( float shading_rate );
    void set_matte( bool matte );
    void set_transform_left_handed( bool transform_left_handed );
    void set_geometry_left_handed( bool geometry_left_handed );
    void set_two_sided( bool two_sided );
    void set_color( const math::vec3& color );
    void set_opacity( const math::vec3& opacity );
    void set_u_basis( const math::vec4* u_basis );
    void set_v_basis( const math::vec4* v_basis );

    void displacement_shade( Grid& grid );
    void set_displacement_shader( Shader* displacement_shader, const math::mat4x4& camera_transform );
    Shader* displacement_shader() const;
    Grid& displacement_parameters() const;

    void surface_shade( Grid& grid );
    void set_surface_shader( Shader* surface_shader, const math::mat4x4& camera_transform );
    Shader* surface_shader() const;
    Grid& surface_parameters() const;

    void light_shade( Grid& grid );
    Grid& add_light_shader( Shader* light_shader, const math::mat4x4& camera_transform );
    void activate_light_shader( Grid& grid );
    void deactivate_light_shader( Grid& grid );
    std::vector<Grid*>::iterator find_active_light_shader_by_grid( const Grid& grid );
    
    void push_transform();
    void pop_transform();
    void identity();
    void transform( const math::mat4x4& transform );
    void concat_transform( const math::mat4x4& transform );
    void perspective( float fov, float aspect_ratio, float near_clip_distance, float far_clip_distance );
    const math::mat4x4& transform() const;

    void add_coordinate_system( const char* name, const math::mat4x4& transform );
    void remove_coordinate_system( const char* name );
    math::mat4x4 transform_from( const std::string& name ) const;
};

}

#endif
