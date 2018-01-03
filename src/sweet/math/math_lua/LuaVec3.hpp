#ifndef SWEET_MATH_LUAVEC3_HPP_INCLUDED
#define SWEET_MATH_LUAVEC3_HPP_INCLUDED

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

struct vec3;

class SWEET_MATH_LUA_DECLSPEC LuaVec3
{
    lua::Lua& lua_;
    lua::LuaObject* vec3_prototype_;
    lua::LuaObject* vec3_metatable_;

public:
    LuaVec3( lua::Lua& lua );
    ~LuaVec3();    
    void push_vec3( lua_State* lua_state, const math::vec3& vv );
    static float vec3_x( const math::vec3& xx );
    static float vec3_y( const math::vec3& xx );
    static float vec3_z( const math::vec3& xx );
    static int xyz( lua_State* lua_state );
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
    static int cross( lua_State* lua_state );
    static int rgb_from_hsv( lua_State* lua_state );
    static int rgb_from_hsl( lua_State* lua_state );
};

}

}

#endif
