//
// LuaMat4x4.cpp
// Copyright (c) 2012 - 2013 Charles Baker.  All rights reserved.
//

#include "LuaMat4x4.hpp"
#include <sweet/math/mat4x4.ipp>
#include <sweet/math/mat4x4.ipp>
#include <sweet/math/vec4.ipp>
#include <sweet/math/scalar.ipp>
#include <sweet/lua/LuaUserDataTemplate.ipp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>
#include <stddef.h>

using namespace sweet;
using namespace sweet::lua;
using namespace sweet::math;

LuaMat4x4::LuaMat4x4( lua::Lua& lua )
: lua_( lua ),
  mat4x4_prototype_( NULL ),
  mat4x4_metatable_( NULL )
{
    mat4x4_prototype_ = new lua::LuaObject( lua_ );
    mat4x4_metatable_ = new lua::LuaObject( lua_ );
    
    mat4x4_prototype_->members()
        ( "identity", raw(LuaMat4x4::identity), this )
        ( "inverse", raw(LuaMat4x4::inverse), this )
        ( "transpose", raw(LuaMat4x4::transpose), this )
        ( "rotate", raw(LuaMat4x4::rotate), this )
        ( "translate", raw(LuaMat4x4::translate), this )
        ( "scale", raw(LuaMat4x4::scale), this )
        ( "shear", raw(LuaMat4x4::shear), this )
    ;
    
    mat4x4_metatable_->members()
        ( "__add", raw(&LuaMat4x4::add), this )
        ( "__sub", raw(&LuaMat4x4::subtract), this )
        ( "__mul", raw(&LuaMat4x4::multiply), this )
        ( "__index", mat4x4_prototype_ )
    ;
    
    lua_.globals()
        ( "mat4x4", mat4x4_prototype_ )
    ;
}

LuaMat4x4::~LuaMat4x4()
{
    delete mat4x4_metatable_;
    delete mat4x4_prototype_;
}

void LuaMat4x4::push_mat4x4( lua_State* lua_state, const math::mat4x4& m )
{
    SWEET_ASSERT( lua_state );
    math::mat4x4* mm = reinterpret_cast<math::mat4x4*>( lua_newuserdata(lua_state, sizeof(lua::LuaUserDataTemplate<math::mat4x4>)) );
    new (mm) lua::LuaUserDataTemplate<math::mat4x4>( SWEET_STATIC_TYPEID(math::mat4x4), m );
    lua_push_object( lua_state, mat4x4_metatable_ );
    lua_setmetatable( lua_state, -2 );    
}

int LuaMat4x4::identity( lua_State* lua_state )
{
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::identity() );
    return 1;
}

int LuaMat4x4::add( lua_State* lua_state )
{
    const int M0 = 1;
    const int M1 = 2;
    const math::mat4x4& m0 = lua_to_value<math::mat4x4>( lua_state, M0 );
    const math::mat4x4& m1 = lua_to_value<math::mat4x4>( lua_state, M1 );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, m0 + m1 );
    return 1;
}

int LuaMat4x4::subtract( lua_State* lua_state )
{
    const int M0 = 1;
    const int M1 = 2;
    const math::mat4x4& m0 = lua_to_value<math::mat4x4>( lua_state, M0 );
    const math::mat4x4& m1 = lua_to_value<math::mat4x4>( lua_state, M1 );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, m0 - m1 );
    return 1;
}

int LuaMat4x4::multiply( lua_State* lua_state )
{
    const int M0 = 1;
    const int M1 = 2;
    const math::mat4x4& m0 = lua_to_value<math::mat4x4>( lua_state, M0 );
    const math::mat4x4& m1 = lua_to_value<math::mat4x4>( lua_state, M1 );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, m0 * m1 );
    return 1;
}

int LuaMat4x4::inverse( lua_State* lua_state )
{
    const int M0 = 1;
    const math::mat4x4& m0 = lua_to_value<math::mat4x4>( lua_state, M0 );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::inverse(m0) );
    return 1;
}

int LuaMat4x4::transpose( lua_State* lua_state )
{
    const int M0 = 1;
    const math::mat4x4& m0 = lua_to_value<math::mat4x4>( lua_state, M0 );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::transpose(m0) );
    return 1;
}

int LuaMat4x4::rotate( lua_State* lua_state )
{
    const int Q0 = 1;
    const math::quat& q0 = lua_to_value<math::quat>( lua_state, Q0 );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::rotate(q0) );
    return 1;
}

int LuaMat4x4::translate( lua_State* lua_state )
{
    const int TX = 1;
    const int TY = 2;
    const int TZ = 3;
    float tx = lua_tonumber( lua_state, TX );
    float ty = lua_tonumber( lua_state, TY );
    float tz = lua_tonumber( lua_state, TZ );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::translate(tx, ty, tz) );
    return 1;
}

int LuaMat4x4::scale( lua_State* lua_state )
{
    const int SX = 1;
    const int SY = 2;
    const int SZ = 3;
    float sx = lua_tonumber( lua_state, SX );
    float sy = lua_tonumber( lua_state, SY );
    float sz = lua_tonumber( lua_state, SZ );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::scale(sx, sy, sz) );
    return 1;
}

int LuaMat4x4::shear( lua_State* lua_state )
{
    const int HX = 1;
    const int HY = 2;
    const int HZ = 3;
    float hx = lua_tonumber( lua_state, HX );
    float hy = lua_tonumber( lua_state, HY );
    float hz = lua_tonumber( lua_state, HZ );
    LuaMat4x4* lua_mat4x4 = reinterpret_cast<LuaMat4x4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_mat4x4 );
    lua_mat4x4->push_mat4x4( lua_state, math::shear(hx, hy, hz) );
    return 1;
}
