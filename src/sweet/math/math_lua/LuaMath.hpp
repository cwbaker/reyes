#ifndef SWEET_MATH_LUAMATH_HPP_INCLUDED
#define SWEET_MATH_LUAMATH_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace lua
{

class Lua;

}
    
namespace math
{

class LuaVec2;
class LuaVec3;
class LuaVec4;
class LuaQuat;
class LuaMat4x4;

class SWEET_MATH_LUA_DECLSPEC LuaMath
{
    math::LuaVec2* lua_vec2_;
    math::LuaVec3* lua_vec3_;
    math::LuaVec4* lua_vec4_;
    math::LuaQuat* lua_quat_;
    math::LuaMat4x4* lua_mat4x4_;

public:
    LuaMath();
    ~LuaMath();
    void create( lua::Lua* lua );
    void destroy();
    math::LuaVec2* lua_vec2() const;
    math::LuaVec3* lua_vec3() const;
    math::LuaVec4* lua_vec4() const;
    math::LuaQuat* lua_quat() const;
    math::LuaMat4x4* lua_mat4x4() const;
};

}

}

#endif
