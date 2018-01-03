//
// LuaQuat.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LuaQuat.hpp"
#include <sweet/math/quat.ipp>
#include <sweet/math/scalar.ipp>
#include <sweet/lua/LuaUserDataTemplate.ipp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>
#include <stddef.h>

using namespace sweet;
using namespace sweet::lua;
using namespace sweet::math;

LuaQuat::LuaQuat( lua::Lua& lua )
: lua_( lua ),
  quat_prototype_( NULL ),
  quat_metatable_( NULL )
{
    quat_prototype_ = new lua::LuaObject( lua_ );
    quat_metatable_ = new lua::LuaObject( lua_ );
    
    quat_prototype_->members()
        ( "x", &LuaQuat::quat_x )
        ( "y", &LuaQuat::quat_y )
        ( "z", &LuaQuat::quat_z )
        ( "w", &LuaQuat::quat_w )
        ( "xyzw", raw(LuaQuat::xyzw), this )
        ( "identity", raw(LuaQuat::identity), this )
        ( "axis_angle", raw(LuaQuat::axis_angle), this )
        ( "scale", raw(LuaQuat::scale), this )
        ( "slerp", raw(LuaQuat::slerp), this )
        ( "normalize", raw(LuaQuat::normalize), this )
        ( "dot", raw(LuaQuat::dot), this )
    ;
    
    quat_metatable_->members()
        ( "__add", raw(&LuaQuat::add), this )
        ( "__mul", raw(&LuaQuat::multiply), this )
        ( "__index", quat_prototype_ )
    ;
    
    lua_.globals()
        ( "quat", quat_prototype_ )
    ;
}

LuaQuat::~LuaQuat()
{
    delete quat_metatable_;
    quat_metatable_ = NULL;
    
    delete quat_prototype_;
    quat_prototype_ = NULL;
}

void LuaQuat::push_quat( lua_State* lua_state, const math::quat& q )
{
    SWEET_ASSERT( lua_state );
    math::quat* qq = reinterpret_cast<math::quat*>( lua_newuserdata(lua_state, sizeof(lua::LuaUserDataTemplate<math::quat>)) );
    new (qq) lua::LuaUserDataTemplate<math::quat>( SWEET_STATIC_TYPEID(math::quat), q );
    lua_push_object( lua_state, quat_metatable_ );
    lua_setmetatable( lua_state, -2 );    
}

float LuaQuat::quat_x( const math::quat& q )
{
    return q.x;
}

float LuaQuat::quat_y( const math::quat& q )
{
    return q.y;
}

float LuaQuat::quat_z( const math::quat& q )
{
    return q.z;
}

float LuaQuat::quat_w( const math::quat& q )
{
    return q.w;
}

int LuaQuat::xyzw( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    const int X = 1;
    const int Y = 2;
    const int Z = 3;
    const int W = 4;
    float x = !lua_isnoneornil( lua_state, X ) ? lua_tonumber( lua_state, X ) : 0.0f;
    float y = !lua_isnoneornil( lua_state, Y ) ? lua_tonumber( lua_state, Y ) : 0.0f;
    float z = !lua_isnoneornil( lua_state, Z ) ? lua_tonumber( lua_state, Z ) : 0.0f;
    float w = !lua_isnoneornil( lua_state, Z ) ? lua_tonumber( lua_state, W ) : 1.0f;
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, math::quat(x, y, z, w) );
    return 1;
}

int LuaQuat::identity( lua_State* lua_state )
{
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, math::quat(0.0f, 0.0f, 0.0f, 1.0f) );
    return 1;
}

int LuaQuat::axis_angle( lua_State* lua_state )
{
    const int AXIS = 1;
    const int ANGLE = 2;
    const math::vec3& axis = lua_to_value<math::vec3>( lua_state, AXIS );
    float angle = lua_tonumber( lua_state, ANGLE );
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, math::quat(axis, angle) );
    return 1;
}

int LuaQuat::add( lua_State* lua_state )
{
    const int Q0 = 1;
    const int Q1 = 2;
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    const math::quat& q1 = lua_to_value<math::quat>( lua_state, Q1 );
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, q0 + q1 );
    return 1;
}

int LuaQuat::multiply( lua_State* lua_state )
{
    const int Q0 = 1;
    const int Q1 = 2;
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    const math::quat& q1 = lua_to_value<math::quat>( lua_state, Q1 );
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, q0 * q1 );
    return 1;
}

int LuaQuat::scale( lua_State* lua_state )
{
    const int SCALAR = 1;
    const int Q0 = 2;
    float scalar = lua_tonumber( lua_state, SCALAR );
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, scalar * q0 );
    return 1;
}

int LuaQuat::slerp( lua_State* lua_state )
{
    const int Q0 = 1;
    const int Q1 = 2;
    const int T = 3;
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    const math::quat& q1 = lua_to_value<math::quat>( lua_state, Q1 );
    float t = lua_tonumber( lua_state, T );
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, math::slerp(q0, q1, clamp(t, 0.0f, 1.0f)) );
    return 1;
}

int LuaQuat::normalize( lua_State* lua_state )
{
    const int Q0 = 1;
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    LuaQuat* lua_quat = reinterpret_cast<LuaQuat*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_quat );
    lua_quat->push_quat( lua_state, math::normalize(q0) );
    return 1;
}

int LuaQuat::dot( lua_State* lua_state )
{
    const int Q0 = 1;
    const int Q1 = 2;
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    const math::quat& q1 = lua_to_value<math::quat>( lua_state, Q1 );
    lua_pushnumber( lua_state, math::dot(q0, q1) );
    return 1;
}
