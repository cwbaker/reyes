#ifndef SWEET_MATH_LUAVEC2_HPP_INCLUDED
#define SWEET_MATH_LUAVEC2_HPP_INCLUDED

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

struct vec2;

class SWEET_MATH_LUA_DECLSPEC LuaVec2
{
    lua::Lua& lua_;
    lua::LuaObject* vec2_prototype_;
    lua::LuaObject* vec2_metatable_;

public:
    LuaVec2( lua::Lua& lua );
    ~LuaVec2();    
    void push_vec2( lua_State* lua_state, const math::vec2& vv );
    static float vec2_x( const math::vec2& xx );
    static float vec2_y( const math::vec2& xx );
    static int xy( lua_State* lua_state );
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
};

}

}

#endif
