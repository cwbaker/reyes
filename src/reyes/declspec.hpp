//
// declspec.hpp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved.
//

#ifndef REYES_DECLSPEC_HPP_INCLUDED
#define REYES_DECLSPEC_HPP_INCLUDED

#if defined(BUILD_MODULE_RENDER) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define REYES_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define REYES_DECLSPEC __declspec(dllimport)
#else
#define REYES_DECLSPEC
#endif 

#endif
