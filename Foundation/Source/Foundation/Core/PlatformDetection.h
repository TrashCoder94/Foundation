#pragma once

// Platform detection using predefined macros
// Windows
#ifdef _WIN32
	// Windows x64/x86
	#define WIN32_LEAN_AND_MEAN
	#ifdef _WIN64
		#define FD_PLATFORM_WINDOWS
	#else
		#error "x86 builds are not supported!"
	#endif
// Apple
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/*
		TARGET_OS_MAC exists on all the platforms
		so we must check all of them (in this order)
		to ensure that we're running on MAC and
		not some other Apple platform
	*/
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define FD_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define FD_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/*
	We also have to check __ANDROID__ before __linux__
	since Android is based on the linux kernel
	it has __linux__ defined
*/
// Android
#elif defined(__ANDROID__)
	#define FD_PLATFORM_ANDROID
	#error "Android is not supported!"
// Linux
#elif defined(__linux__)
	#define FD_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif	// End of platform detection.
