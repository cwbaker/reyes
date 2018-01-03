#ifndef SWEET_MATH_LUAVEC4_HPP_INCLUDED
#define SWEET_MATH_LUAVEC4_HPP_INCLUDED

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

struct vec4;

class SWEET_MATH_LUA_DECLSPEC LuaVec4
{
    lua::Lua& lua_;
    lua::LuaObject* vec4_prototype_;
    lua::LuaObject* vec4_metatable_;

public:
    LuaVec4( lua::Lua& lua );
    ~LuaVec4();    
    void push_vec4( lua_State* lua_state, const math::vec4& vv );
    static float vec4_x( const math::vec4& xx );
    static float vec4_y( const math::vec4& xx );
    static float vec4_z( const math::vec4& xx );
    static float vec4_w( const math::vec4& xx );
    static int xyzw( lua_State* lua_state );
    static int srgb( lua_State* lua_state );
    static int zero( lua_State* lua_state );
    static int one( lua_State* lua_state );
    static int add( lua_State* lua_state );
    static int subtract( lua_State* lua_state );
    static int multiply( lua_State* lua_state );
    static int divide( lua_State* lua_state );
    static int unary_minus( lua_State* lua_state );
    static int lerp( lua_State* lua_state );
    static int normalize( lua_State* lua_state );
    static int length( lua_State* lua_state );
    static int dot( lua_State* lua_state );
    static int transparent( lua_State* lua_state );
    static int opaque( lua_State* lua_state );
};

}

}

#endif
