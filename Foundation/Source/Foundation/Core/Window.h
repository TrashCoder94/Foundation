#pragma once

#include <sstream>
#include "Foundation/Events/Event.h"

namespace Foundation
{
	struct WindowProperties
	{
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;

		WindowProperties(const std::string& title = "Foundation Engine", 
			uint32_t width = 1600, 
			uint32_t height = 900) :
			m_Title(title),
			m_Width(width),
			m_Height(height)
		{}
	};

	class Window
	{
		public:
			using EventCallbackFn = std::function<void(Event&)>;

			virtual ~Window() {}

			virtual void OnUpdate() = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
			virtual void SetVSync(bool enabled) = 0;
			virtual bool IsVSync() const = 0;

			virtual void* GetNativeWindow() const = 0;

			static UniquePtr<Window> Create(const WindowProperties& properties = WindowProperties());
	};
}
