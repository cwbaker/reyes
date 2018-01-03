#ifndef SWEET_MATH_LUAQUAT_HPP_INCLUDED
#define SWEET_MATH_LUAQUAT_HPP_INCLUDED

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

struct quat;

class SWEET_MATH_LUA_DECLSPEC LuaQuat
{
    lua::Lua& lua_;
    lua::LuaObject* quat_prototype_;
    lua::LuaObject* quat_metatable_;

public:
    LuaQuat( lua::Lua& lua );
    ~LuaQuat();    
    void push_quat( lua_State* lua_state, const math::quat& q );
    static float quat_x( const math::quat& q );
    static float quat_y( const math::quat& q );
    static float quat_z( const math::quat& q );
    static float quat_w( const math::quat& q );
    static int xyzw( lua_State* lua_state );
    static int identity( lua_State* lua_state );
    static int axis_angle( lua_State* lua_state );
    static int add( lua_State* lua_state );
    static int multiply( lua_State* lua_state );
    static int scale( lua_State* lua_state );
    static int slerp( lua_State* lua_state );
    static int normalize( lua_State* lua_state );
    static int dot( lua_State* lua_state );
};

}

}

#endif
