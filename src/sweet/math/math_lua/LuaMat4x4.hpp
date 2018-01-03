#ifndef SWEET_MATH_LUAMAT4X4_HPP_INCLUDED
#define SWEET_MATH_LUAMAT4X4_HPP_INCLUDED

#include "declspec.hpp"

struct lua_State;

namespace sweet
{

namespace lua
{

class LuaObject;
class Lua;

}

namespace math
{

struct mat4x4;

class SWEET_MATH_LUA_DECLSPEC LuaMat4x4
{
    lua::Lua& lua_;
    lua::LuaObject* mat4x4_prototype_;
    lua::LuaObject* mat4x4_metatable_;

public:
    LuaMat4x4( lua::Lua& lua );
    ~LuaMat4x4();    
    void push_mat4x4( lua_State* lua_state, const math::mat4x4& q );
    static int identity( lua_State* lua_state );
    static int add( lua_State* lua_state );
    static int subtract( lua_State* lua_state );
    static int multiply( lua_State* lua_state );
    static int inverse( lua_State* lua_state );
    static int transpose( lua_State* lua_state );
    static int rotate( lua_State* lua_state );
    static int translate( lua_State* lua_state );
    static int scale( lua_State* lua_state );
    static int shear( lua_State* lua_state );
};

}

}

#endif
