#include "fdpch.h"
#include "WindowsWindow.h"

#include "Foundation/Events/ApplicationEvent.h"
#include "Foundation/Events/KeyEvent.h"
#include "Foundation/Events/MouseEvent.h"
#include "Foundation/Core/Input.h"
#include "Foundation/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Foundation
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* message)
	{
		FD_CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, message);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		FD_PROFILE_FUNCTION();

		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		FD_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		FD_PROFILE_FUNCTION();

		glfwPollEvents();
		m_pContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		FD_PROFILE_FUNCTION();

		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.m_VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.m_VSync;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_pWindow;
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		FD_PROFILE_FUNCTION();

		m_Data.m_Title = properties.m_Title;
		m_Data.m_Width = properties.m_Width;
		m_Data.m_Height = properties.m_Height;

		FD_CORE_LOG_INFO("Creating window {0} ({1}, {2})", properties.m_Title, properties.m_Width, properties.m_Height);

		if (s_GLFWWindowCount == 0)
		{
			FD_PROFILE_SCOPE("glfwInit");
			FD_CORE_LOG_INFO("Initialising GLFW!");
			int success = glfwInit();
			FD_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			FD_PROFILE_SCOPE("glfwCreateWindow");
#ifdef FD_DEBUG
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

			m_pWindow = glfwCreateWindow((int)properties.m_Width, (int)properties.m_Height, properties.m_Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_pContext = GraphicsContext::Create(m_pWindow);
		m_pContext->Init();

		glfwSetWindowUserPointer(m_pWindow, &m_Data);
		SetVSync(true);

		// Callbacks

		// Window Resize Event
		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height) 
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.m_Width = width;
			data.m_Height = height;
			
			// Creating and dispatching the event.
			WindowResizeEvent event(width, height);
			data.m_Callback(event);
		});

		// Window Close Event
		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* window)
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Creating and dispatching the event.
			WindowCloseEvent event;
			data.m_Callback(event);
		});

		// Key Event
		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Creating and dispatching the event.
			switch (action)
			{
				// Key press.
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.m_Callback(event);
					break;
				}
				// Key release.
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.m_Callback(event);
					break;
				}
				// Key repeat.
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.m_Callback(event);
					break;
				}
			}
		});

		// Key Event
		glfwSetCharCallback(m_pWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Creating and dispatching the event.
			KeyTypedEvent event(keycode);
			data.m_Callback(event);
		});

		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods) 
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Creating and dispatching the event.
			switch (action)
			{
				// Mouse button press.
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.m_Callback(event);
					break;
				}
				// Mouse button release.
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.m_Callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* window, double xOffset, double yOffset) 
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Creating and dispatching the event.
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.m_Callback(event);
		});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// Creating and dispatching the event.
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.m_Callback(event);
		});

		glfwSetWindowFocusCallback(m_pWindow, [](GLFWwindow* window, int focus)
		{
			// Data from the callback.
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// focus > 0 - gained focus
			if (focus > 0)
			{
				WindowFocusEvent event;
				data.m_Callback(event);
			}
			// focus <= 0 - lost focus
			else
			{
				WindowLostFocusEvent event;
				data.m_Callback(event);
			}
		});
	}

	void WindowsWindow::Shutdown()
	{
		FD_PROFILE_FUNCTION();

		glfwDestroyWindow(m_pWindow);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			FD_CORE_LOG_INFO("Terminating GLFW!");
			glfwTerminate();
		}
	}
}
