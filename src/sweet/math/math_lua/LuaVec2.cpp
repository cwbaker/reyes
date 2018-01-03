//
// LuaVec2.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LuaVec2.hpp"
#include <sweet/math/vec2.ipp>
#include <sweet/math/scalar.ipp>
#include <sweet/lua/LuaUserDataTemplate.ipp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>

using namespace sweet;
using namespace sweet::lua;
using namespace sweet::math;

LuaVec2::LuaVec2( lua::Lua& lua )
: lua_( lua ),
  vec2_prototype_( NULL ),
  vec2_metatable_( NULL )
{
    vec2_prototype_ = new lua::LuaObject( lua_ );
    vec2_metatable_ = new lua::LuaObject( lua_ );

    vec2_prototype_->members()
        ( "x", &LuaVec2::vec2_x )
        ( "y", &LuaVec2::vec2_y )
        ( "xy", raw(&LuaVec2::xy), this )
        ( "zero", raw(&LuaVec2::zero), this )
        ( "one", raw(&LuaVec2::one), this )
        ( "lerp", raw(&LuaVec2::lerp), this )
        ( "normalize", raw(&LuaVec2::normalize), this )
        ( "length", raw(&LuaVec2::length), this )
    ;
    
    vec2_metatable_->members()
        ( "__add", raw(&LuaVec2::add), this )
        ( "__sub", raw(&LuaVec2::subtract), this )
        ( "__mul", raw(&LuaVec2::multiply), this )
        ( "__div", raw(&LuaVec2::divide), this )
        ( "__unm", raw(&LuaVec2::unary_minus), this )
        ( "__index", vec2_prototype_ )
    ;

    lua_.globals()
        ( "vec2", vec2_prototype_ )
    ;
}

LuaVec2::~LuaVec2()
{
    delete vec2_metatable_;
    delete vec2_prototype_;
}

void LuaVec2::push_vec2( lua_State* lua_state, const math::vec2& vv )
{
    SWEET_ASSERT( lua_state );
    math::vec2* xx = reinterpret_cast<math::vec2*>( lua_newuserdata(lua_state, sizeof(lua::LuaUserDataTemplate<math::vec2>)) );
    new (xx) lua::LuaUserDataTemplate<math::vec2>( SWEET_STATIC_TYPEID(math::vec2), vv );
    lua_push_object( lua_state, vec2_metatable_ );
    lua_setmetatable( lua_state, -2 );    
}

float LuaVec2::vec2_x( const math::vec2& xx )
{
    return xx.x;
}

float LuaVec2::vec2_y( const math::vec2& xx )
{
    return xx.y;
}

int LuaVec2::xy( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    const int X = 1;
    const int Y = 2;
    float x = !lua_isnoneornil( lua_state, X ) ? lua_tonumber( lua_state, X ) : 0.0f;
    float y = !lua_isnoneornil( lua_state, Y ) ? lua_tonumber( lua_state, Y ) : 0.0f;
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, math::vec2(x, y) );
    return 1;
}

int LuaVec2::zero( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, math::vec2(0.0f, 0.0f) );
    return 1;
}

int LuaVec2::one( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, math::vec2(1.0f, 1.0f) );
    return 1;
}

int LuaVec2::add( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec2& lhs = lua_to_value<math::vec2>( lua_state, LHS );
    const math::vec2& rhs = lua_to_value<math::vec2>( lua_state, RHS );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, lhs + rhs );
    return 1;
}

int LuaVec2::subtract( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec2& lhs = lua_to_value<math::vec2>( lua_state, LHS );
    const math::vec2& rhs = lua_to_value<math::vec2>( lua_state, RHS );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, lhs - rhs );
    return 1;
}

int LuaVec2::multiply( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    float lhs = lua_tonumber( lua_state, LHS );
    const math::vec2& rhs = lua_to_value<math::vec2>( lua_state, RHS );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, lhs * rhs );
    return 1;
}

int LuaVec2::divide( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec2& lhs = lua_to_value<math::vec2>( lua_state, LHS );
    float rhs = lua_tonumber( lua_state, RHS );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, lhs / rhs );
    return 1;
}

int LuaVec2::unary_minus( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const math::vec2& lhs = lua_to_value<math::vec2>( lua_state, LHS );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, -lhs );
    return 1;
}

int LuaVec2::lerp( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V0 = 1;
    const int V1 = 2;
    const int T = 2;
    const math::vec2& v0 = lua_to_value<math::vec2>( lua_state, V0 );
    const math::vec2& v1 = lua_to_value<math::vec2>( lua_state, V1 );
    float t = clamp( lua_tonumber(lua_state, T), 0.0f, 1.0f );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, math::lerp(v0, v1, t) );
    return 1;
}

int LuaVec2::normalize( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V = 1;
    const math::vec2& v = lua_to_value<math::vec2>( lua_state, V );
    LuaVec2* lua_vec2 = reinterpret_cast<LuaVec2*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec2 );
    lua_vec2->push_vec2( lua_state, math::normalize(v) );
    return 1;
}

int LuaVec2::length( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V = 1;
    const math::vec2& v = lua_to_value<math::vec2>( lua_state, V );
    lua_pushnumber( lua_state, math::length(v) );
    return 1;
}
