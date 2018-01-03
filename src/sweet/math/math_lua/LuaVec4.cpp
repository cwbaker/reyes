//
// LuaVec4.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "LuaVec4.hpp"
#include <sweet/math/vec4.ipp>
#include <sweet/math/scalar.ipp>
#include <sweet/lua/LuaUserDataTemplate.ipp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Lua.hpp>

using namespace sweet;
using namespace sweet::lua;
using namespace sweet::math;

LuaVec4::LuaVec4( lua::Lua& lua )
: lua_( lua ),
  vec4_prototype_( NULL ),
  vec4_metatable_( NULL )
{
    vec4_prototype_ = new lua::LuaObject( lua_ );
    vec4_metatable_ = new lua::LuaObject( lua_ );

    vec4_prototype_->members()
        ( "x", &LuaVec4::vec4_x )
        ( "y", &LuaVec4::vec4_y )
        ( "z", &LuaVec4::vec4_z )
        ( "w", &LuaVec4::vec4_w )
        ( "red", &LuaVec4::vec4_x )
        ( "green", &LuaVec4::vec4_y )
        ( "blue", &LuaVec4::vec4_z )
        ( "alpha", &LuaVec4::vec4_w )
        ( "xyzw", raw(&LuaVec4::xyzw), this )
        ( "srgb", raw(&LuaVec4::xyzw), this )
        ( "zero", raw(&LuaVec4::zero), this )
        ( "one", raw(&LuaVec4::one), this )
        ( "lerp", raw(&LuaVec4::lerp), this )
        ( "normalize", raw(&LuaVec4::normalize), this )
        ( "length", raw(&LuaVec4::length), this )
        ( "dot", raw(&LuaVec4::dot), this )
        ( "transparent", raw(&LuaVec4::transparent), this )
        ( "opaque", raw(&LuaVec4::opaque), this )
    ;
    
    vec4_metatable_->members()
        ( "__add", raw(&LuaVec4::add), this )
        ( "__sub", raw(&LuaVec4::subtract), this )
        ( "__mul", raw(&LuaVec4::multiply), this )
        ( "__div", raw(&LuaVec4::divide), this )
        ( "__unm", raw(&LuaVec4::unary_minus), this )
        ( "__index", vec4_prototype_ )
    ;

    lua_.globals()
        ( "vec4", vec4_prototype_ )
    ;
}

LuaVec4::~LuaVec4()
{
    delete vec4_metatable_;
    vec4_metatable_ = NULL;

    delete vec4_prototype_;
    vec4_prototype_ = NULL;
}

void LuaVec4::push_vec4( lua_State* lua_state, const math::vec4& vv )
{
    SWEET_ASSERT( lua_state );
    math::vec4* xx = reinterpret_cast<math::vec4*>( lua_newuserdata(lua_state, sizeof(lua::LuaUserDataTemplate<math::vec4>)) );
    new (xx) lua::LuaUserDataTemplate<math::vec4>( SWEET_STATIC_TYPEID(math::vec4), vv );
    lua_push_object( lua_state, vec4_metatable_ );
    lua_setmetatable( lua_state, -2 );    
}

float LuaVec4::vec4_x( const math::vec4& xx )
{
    return xx.x;
}

float LuaVec4::vec4_y( const math::vec4& xx )
{
    return xx.y;
}

float LuaVec4::vec4_z( const math::vec4& xx )
{
    return xx.z;
}

float LuaVec4::vec4_w( const math::vec4& xx )
{
    return xx.w;
}

int LuaVec4::xyzw( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    const int X = 1;
    const int Y = 2;
    const int Z = 3;
    const int W = 4;
    float x = !lua_isnoneornil( lua_state, X ) ? lua_tonumber( lua_state, X ) : 0.0f;
    float y = !lua_isnoneornil( lua_state, Y ) ? lua_tonumber( lua_state, Y ) : 0.0f;
    float z = !lua_isnoneornil( lua_state, Z ) ? lua_tonumber( lua_state, Z ) : 0.0f;
    float w = !lua_isnoneornil( lua_state, W ) ? lua_tonumber( lua_state, W ) : 0.0f;
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, math::vec4(x, y, z, w) );
    return 1;
}

int LuaVec4::srgb( lua_State* lua_state )
{
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    if ( lua_gettop(lua_state) == 1 )
    {
        const int RGBA = 1;
        const math::vec4& rgba = lua_to_value<math::vec4>( lua_state, RGBA );
        lua_vec4->push_vec4( lua_state, math::srgb(rgba) );
    }
    else
    {
        const int RED = 1;
        const int GREEN = 2;
        const int BLUE = 3;
        const int ALPHA = 4;
        float red = luaL_optnumber( lua_state, RED, static_cast<lua_Number>(1.0f) );
        float green = luaL_optnumber( lua_state, GREEN, static_cast<lua_Number>(1.0f) );
        float blue = luaL_optnumber( lua_state, BLUE, static_cast<lua_Number>(1.0f) );
        float alpha = luaL_optnumber( lua_state, ALPHA, static_cast<lua_Number>(1.0f) );
        lua_vec4->push_vec4( lua_state, math::srgb(vec4(red, green, blue, alpha)) );
    }
    return 1;
}

int LuaVec4::zero( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, math::vec4(0.0f, 0.0f, 0.0f, 0.0f) );
    return 1;
}

int LuaVec4::one( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, math::vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    return 1;
}

int LuaVec4::add( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec4& lhs = lua_to_value<math::vec4>( lua_state, LHS );
    const math::vec4& rhs = lua_to_value<math::vec4>( lua_state, RHS );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, lhs + rhs );
    return 1;
}

int LuaVec4::subtract( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec4& lhs = lua_to_value<math::vec4>( lua_state, LHS );
    const math::vec4& rhs = lua_to_value<math::vec4>( lua_state, RHS );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, lhs - rhs );
    return 1;
}

int LuaVec4::multiply( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    float lhs = lua_tonumber( lua_state, LHS );
    const math::vec4& rhs = lua_to_value<math::vec4>( lua_state, RHS );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, lhs * rhs );
    return 1;
}

int LuaVec4::divide( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec4& lhs = lua_to_value<math::vec4>( lua_state, LHS );
    float rhs = lua_tonumber( lua_state, RHS );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, lhs / rhs );
    return 1;
}

int LuaVec4::unary_minus( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const math::vec4& lhs = lua_to_value<math::vec4>( lua_state, LHS );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, -lhs );
    return 1;
}

int LuaVec4::lerp( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V0 = 1;
    const int V1 = 2;
    const int T = 3;
    const math::vec4& v0 = lua_to_value<math::vec4>( lua_state, V0 );
    const math::vec4& v1 = lua_to_value<math::vec4>( lua_state, V1 );
    float t = clamp( lua_tonumber(lua_state, T), 0.0f, 1.0f );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, math::lerp(v0, v1, t) );
    return 1;
}

int LuaVec4::normalize( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V = 1;
    const math::vec4& v = lua_to_value<math::vec4>( lua_state, V );
    LuaVec4* lua_vec4 = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_vec4 );
    lua_vec4->push_vec4( lua_state, math::normalize(v) );
    return 1;
}

int LuaVec4::length( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int V = 1;
    const math::vec4& v = lua_to_value<math::vec4>( lua_state, V );
    lua_pushnumber( lua_state, math::length(v) );
    return 1;
}

int LuaVec4::dot( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int LHS = 1;
    const int RHS = 2;
    const math::vec4& lhs = lua_to_value<math::vec4>( lua_state, LHS );
    const math::vec4& rhs = lua_to_value<math::vec4>( lua_state, RHS );
    lua_pushnumber( lua_state, math::dot(lhs, rhs) );
    return 1;  
}

int LuaVec4::transparent( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int COLOR = 1;
    const int ALPHA = 2;
    const math::vec4& color = lua_to_value<math::vec4>( lua_state, COLOR );
    float alpha = lua_isnumber( lua_state, ALPHA ) ? lua_tonumber( lua_state, ALPHA ) : 0.0f;
    LuaVec4* lua_color = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_color );
    lua_color->push_vec4( lua_state, math::vec4(color.x, color.y, color.z, alpha) );
    return 1;
}

int LuaVec4::opaque( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int COLOR = 1;
    const int ALPHA = 2;
    const math::vec4& color = lua_to_value<math::vec4>( lua_state, COLOR );
    float alpha = lua_isnumber( lua_state, ALPHA ) ? lua_tonumber( lua_state, ALPHA ) : 1.0f;
    LuaVec4* lua_color = reinterpret_cast<LuaVec4*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_color );
    lua_color->push_vec4( lua_state, math::vec4(color.x, color.y, color.z, alpha) );
    return 1;
}
