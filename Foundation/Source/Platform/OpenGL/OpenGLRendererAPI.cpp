#include "fdpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Foundation
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:			FD_CORE_LOG_CRITICAL(message);	return;
			case GL_DEBUG_SEVERITY_MEDIUM:			FD_CORE_LOG_ERROR(message);		return;
			case GL_DEBUG_SEVERITY_LOW:				FD_CORE_LOG_WARN(message);		return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	FD_CORE_LOG_TRACE(message);		return;
		}

		FD_CORE_ASSERT(false, "Unknown severity level!");
	}

	OpenGLRendererAPI::OpenGLRendererAPI()
	{
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
	{
	}

	void OpenGLRendererAPI::Init()
	{
		FD_PROFILE_FUNCTION();

#ifdef FD_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& clearColour)
	{
		glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount /*= 0*/)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}