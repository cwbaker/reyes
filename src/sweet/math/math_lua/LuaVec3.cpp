//
// LuaVec3.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LuaVec3.hpp"
#include <sweet/math/vec3.ipp>
#include <sweet/math/scalar.ipp>
#include <sweet/lua/LuaUserDataTemplate.ipp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>

using namespace sweet;
using namespace sweet::lua;
using namespace sweet::math;

LuaVec3::LuaVec3( lua::Lua& lua )
: lua_( lua ),
  vec3_prototype_( NULL ),
  vec3_metatable_( NULL )
{
    vec3_prototype_ = new lua::LuaObject( lua_ );
    vec3_metatable_ = new lua::LuaObject( lua_ );

    vec3_prototype_->members()
        ( "x", &LuaVec3::vec3_x )
        ( "y", &LuaVec3::vec3_y )
        ( "z", &LuaVec3::vec3_z )
        ( "xyz", raw(&LuaVec3::xyz), this )
        ( "zero", raw(&LuaVec3::zero), this )
        ( "one", raw(&LuaVec3::one), this )
        ( "lerp", raw(&LuaVec3::lerp), this )
        ( "normalize", raw(&LuaVec3::normalize), this )
        ( "length", raw(&LuaVec3::length), this )
        ( "dot", raw(&LuaVec3::dot), this )
        ( "cross", raw(&LuaVec3::cross), this )
        ( "rgb_from_hsv", raw(&LuaVec3::rgb_from_hsv), this )
        ( "rgb_from_hsl", raw(&LuaVec3::rgb_from_hsl), this )
    ;
    
    vec3_metatable_->members()
        ( "__add", raw(&LuaVec3::add), this )
        ( "__sub", raw(&LuaVec3::subtract), this )
        ( "__mul", raw(&LuaVec3::multiply), this )
        ( "__div", raw(&LuaVec3::divide), this )
        ( "__unm", raw(&LuaVec3::unary_minus), this )
        ( "__index", vec3_prototype_ )
    ;

    lua_.globals()
        ( "vec3", vec3_prototype_ )
    ;
}

LuaVec3::~LuaVec3()
{
    delete vec3_metatable_;
    vec3_metatable_ = NULL;

    delete vec3_prototype_;
    vec3_prototype_ = NULL;
}

void LuaVec3::push_vec3( lua_State* lua_state, const math::vec3& vv )
{
    SWEET_ASSERT( lua_state );
    math::vec3* xx = reinterpret_cast<math::vec3*>( lua_newuserdata(lua_state, sizeof(lua::LuaUserDataTemplate<math::vec3>)) );
    new (xx) lua::LuaUserDataTemplate<math::vec3>( SWEET_STATIC_TYPEID(math::vec3), vv );
    lua_push_object( lua_state, vec3_metatable_ );
    lua_setmetatable( lua_state, -2 );    
}

float LuaVec3::vec3_x( const math::vec3& xx )
{
    return xx.x;
}

float LuaVec3::vec3_y( const math::vec3& xx )
{
    return xx.y;
}

float LuaVec3::vec3_z( const math::vec3& xx )
{
    return xx.z;
}

int LuaVec3::xyz( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    const int X = 1;
    const int Y = 2;
    const int Z = 3;
    float x = !lua_isnoneornil( lua_state, X ) ? lua_tonumber( lua_state, X ) : 0.0f;
    float y = !lua_isnoneornil( lua_state, Y ) ? lua_tonumber( lua_state, Y ) : 0.0f;
    float z = !lua_isnoneornil( lua_state, Z ) ? lua_tonumber( lua_state, Z ) : 0.0f;
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, math::vec3(x, y, z) );
    return 1;
}

int LuaVec3::zero( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, math::vec3(0.0f, 0.0f, 0.0f) );
    return 1;
}

int LuaVec3::one( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, math::vec3(1.0f, 1.0f, 1.0f) );
    return 1;
}

int LuaVec3::add( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec3& lhs = lua_to_value<math::vec3>( lua_state, LHS );
    const math::vec3& rhs = lua_to_value<math::vec3>( lua_state, RHS );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, lhs + rhs );
    return 1;
}

int LuaVec3::subtract( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec3& lhs = lua_to_value<math::vec3>( lua_state, LHS );
    const math::vec3& rhs = lua_to_value<math::vec3>( lua_state, RHS );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, lhs - rhs );
    return 1;
}

int LuaVec3::multiply( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    float lhs = lua_tonumber( lua_state, LHS );
    const math::vec3& rhs = lua_to_value<math::vec3>( lua_state, RHS );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, lhs * rhs );
    return 1;
}

int LuaVec3::divide( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec3& lhs = lua_to_value<math::vec3>( lua_state, LHS );
    float rhs = lua_tonumber( lua_state, RHS );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, lhs / rhs );
    return 1;
}

int LuaVec3::unary_minus( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const math::vec3& lhs = lua_to_value<math::vec3>( lua_state, LHS );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, -lhs );
    return 1;
}

int LuaVec3::lerp( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V0 = 1;
    const int V1 = 2;
    const int T = 3;
    const math::vec3& v0 = lua_to_value<math::vec3>( lua_state, V0 );
    const math::vec3& v1 = lua_to_value<math::vec3>( lua_state, V1 );
    float t = clamp( lua_tonumber(lua_state, T), 0.0f, 1.0f );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, math::lerp(v0, v1, t) );
    return 1;
}

int LuaVec3::normalize( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V = 1;
    const math::vec3& v = lua_to_value<math::vec3>( lua_state, V );
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    lua_vec3->push_vec3( lua_state, math::normalize(v) );
    return 1;
}

int LuaVec3::length( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V = 1;
    const math::vec3& v = lua_to_value<math::vec3>( lua_state, V );
    lua_pushnumber( lua_state, math::length(v) );
    return 1;
}

int LuaVec3::dot( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec3& lhs = lua_to_value<math::vec3>( lua_state, LHS );
    const math::vec3& rhs = lua_to_value<math::vec3>( lua_state, RHS );
    lua_pushnumber( lua_state, math::dot(lhs, rhs) );
    return 1;  
}

int LuaVec3::cross( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    const math::vec3& lhs = lua_to_value<math::vec3>( lua_state, LHS );
    const math::vec3& rhs = lua_to_value<math::vec3>( lua_state, RHS );
    lua_vec3->push_vec3( lua_state, math::cross(lhs, rhs) );
    return 1;  
}

int LuaVec3::rgb_from_hsv( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int HSV = 1;
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    const math::vec3& hsv = lua_to_value<math::vec3>( lua_state, HSV );
    lua_vec3->push_vec3( lua_state, math::rgb_from_hsv(hsv) );
    return 1;  
}

int LuaVec3::rgb_from_hsl( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int HSL = 1;
    LuaVec3* lua_vec3 = reinterpret_cast<LuaVec3*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec3 );
    const math::vec3& hsl = lua_to_value<math::vec3>( lua_state, HSL );
    lua_vec3->push_vec3( lua_state, math::rgb_from_hsl(hsl) );
    return 1;  
}
