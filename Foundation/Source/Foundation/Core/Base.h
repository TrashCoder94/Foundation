#pragma once

#include <memory>
#include "PlatformDetection.h"

// DLL support.
#ifdef FD_PLATFORM_WINDOWS
	#if FD_DYNAMIC_LINK
		#ifdef FD_BUILD_DLL
			#define FOUNDATION_API __declspec(dllexport)
		#else
			#define FOUNDATION_API __declspec(dllimport)
		#endif
	#else 
		#define FOUNDATION_API
	#endif
#else
	#error "Foundation only supports Windows"
#endif	// End of DLL support.

#ifdef FD_DEBUG
	#if defined(FD_PLATFORM_WINDOWS)
		#define FD_DEBUGBREAK() __debugbreak();
	#elif defined(FD_PLATFORM_LINUX)
		#include <signal.h>
		#define FD_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define FD_ENABLE_ASSERTS
#else
	#define FD_DEBUGBREAK()
#endif

#define FD_EXPAND_MACRO(x) x
#define FD_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define FD_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Foundation
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr UniquePtr<T> CreateUniquePtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SharedPtr<T> CreateSharedPtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakPtr = std::weak_ptr<T>;
	template<typename T, typename ... Args>
	constexpr WeakPtr<T> CreateWeakPtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Foundation/Core/Log.h"
#include "Foundation/Core/Assert.h"