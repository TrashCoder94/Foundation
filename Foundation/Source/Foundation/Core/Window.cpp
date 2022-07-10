#include "fdpch.h"
#include "Window.h"

#ifdef FD_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Foundation
{
	UniquePtr<Window> Window::Create(const WindowProperties& properties)
	{
#ifdef FD_PLATFORM_WINDOWS
		return CreateUniquePtr<WindowsWindow>(properties);
#else
		FD_CORE_ASSERT(false, "Unknown platform!")
		return nullptr;
#endif
	}
}