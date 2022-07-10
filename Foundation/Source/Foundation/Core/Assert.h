#pragma once

#include "Foundation/Core/Base.h"
#include "Foundation/Core/Log.h"

#ifdef FD_ENABLE_ASSERTS
	namespace Foundation::Assert
	{
		constexpr const char* CurrentFileName(const char* path)
		{
			const char* file = path;
			while (*path)
			{
				if (*path == '/' || *path == '\\')
					file = ++path;
				else
					path++;
			}
			return file;
		}
	}

	#define FD_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { FD##type##LOG_ERROR(msg, __VA_ARGS__); FD_DEBUGBREAK() } }
	#define FD_INTERNAL_ASSERT_WITH_MSG(type, check, ...) FD_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: '{0}'", __VA_ARGS__)
	#define FD_INTERNAL_ASSERT_NO_MSG(type, check) FD_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at '{1}':'{2}'", FD_STRINGIFY_MACRO(check), ::Foundation::Assert::CurrentFileName(__FILE__), __LINE__)

	#define FD_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define FD_INTERNAL_ASSERT_GET_MACRO(...) FD_EXPAND_MACRO( FD_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, FD_INTERNAL_ASSERT_WITH_MSG, FD_INTERNAL_ASSERT_NO_MSG) )

	#define FD_ASSERT(...) FD_EXPAND_MACRO( FD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define FD_CORE_ASSERT(...) FD_EXPAND_MACRO( FD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define FD_ASSERT(...)
	#define FD_CORE_ASSERT(...)
#endif