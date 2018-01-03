#ifndef SWEET_MATH_LUA_DECLSPEC_HPP
#define SWEET_MATH_LUA_DECLSPEC_HPP

#if defined(BUILD_MODULE_MATH_LUA) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_MATH_LUA_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_MATH_LUA_DECLSPEC __declspec(dllimport)
#else
#define SWEET_MATH_LUA_DECLSPEC
#endif 

#endif
