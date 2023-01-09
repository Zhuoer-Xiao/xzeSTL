#pragma once
#ifndef XZESTL_CONFIG_H
#define XZESTL_CONFIG_H
//不同编译器对C++的支持程度不同，定义config便于移植

#ifndef _STL_EXPLICIT_FUNCTION_TMPL_ARGS
#	define _STL_NULL_TMPL_ARGS <>
#else 
#	define _STL_NULL_TMPL_ARGS
#endif // !_STL_EXPLICIT_FUNCTION_TMPL_ARGS

#endif // !XZESTL_CONFIG_H
