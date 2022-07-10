#pragma once

#include "Foundation/Core/Window.h"
#include "Foundation/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Foundation
{
	class WindowsWindow : public Window
	{
		public:
			WindowsWindow(const WindowProperties& properties);
			virtual ~WindowsWindow();

			void OnUpdate() override;

			inline uint32_t GetWidth() const override { return m_Data.m_Width; }
			inline uint32_t GetHeight() const override { return m_Data.m_Height; }

			inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.m_Callback = callback; }
			void SetVSync(bool enabled) override;
			bool IsVSync() const override;

			void* GetNativeWindow() const override;

		private:
			virtual void Init(const WindowProperties& properties);
			virtual void Shutdown();

		private:
			GLFWwindow* m_pWindow;
			UniquePtr<GraphicsContext> m_pContext;

			struct WindowData
			{
				std::string m_Title = "";
				uint32_t m_Width = 0;
				uint32_t m_Height = 0;
				bool m_VSync = false;
				EventCallbackFn m_Callback;
			};
			WindowData m_Data;
	};
}