//
// LuaMath.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LuaMath.hpp"
#include "LuaVec2.hpp"
#include "LuaVec3.hpp"
#include "LuaVec4.hpp"
#include "LuaQuat.hpp"
#include "LuaMat4x4.hpp"
#include <sweet/assert/assert.hpp>
#include <stddef.h>

using namespace sweet;
using namespace sweet::math;

LuaMath::LuaMath()
: lua_vec2_( NULL ),
  lua_vec3_( NULL ),
  lua_vec4_( NULL ),
  lua_quat_( NULL ),
  lua_mat4x4_( NULL )
{
}

LuaMath::~LuaMath()
{
    delete lua_mat4x4_;
    delete lua_quat_;
    delete lua_vec4_;
    delete lua_vec3_;
    delete lua_vec2_;
}

void LuaMath::create( lua::Lua* lua )
{
    SWEET_ASSERT( lua );
    lua_vec2_ = new math::LuaVec2( *lua );
    lua_vec3_ = new math::LuaVec3( *lua );
    lua_vec4_ = new math::LuaVec4( *lua );
    lua_quat_ = new math::LuaQuat( *lua );
    lua_mat4x4_ = new math::LuaMat4x4( *lua );
}

void LuaMath::destroy()
{
}

math::LuaVec2* LuaMath::lua_vec2() const
{
    return lua_vec2_;
}

math::LuaVec3* LuaMath::lua_vec3() const
{
    return lua_vec3_;
}

math::LuaVec4* LuaMath::lua_vec4() const
{
    return lua_vec4_;
}

math::LuaQuat* LuaMath::lua_quat() const
{
    return lua_quat_;
}

math::LuaMat4x4* LuaMath::lua_mat4x4() const
{
    return lua_mat4x4_;
}
