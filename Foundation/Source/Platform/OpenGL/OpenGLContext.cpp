#include "fdpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Foundation
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		m_pWindowHandle(windowHandle)
	{
		FD_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		FD_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_pWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FD_CORE_ASSERT(status, "Failed not initialize GLAD!");

		FD_LOG_INFO("OpenGL Info:");
		FD_LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		FD_LOG_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		FD_LOG_INFO(" Version: {0}", glGetString(GL_VERSION));

		FD_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Foundation requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		FD_PROFILE_FUNCTION();

		glfwSwapBuffers(m_pWindowHandle);
	}
}
