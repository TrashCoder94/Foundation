#include "fdpch.h"
#include "GraphicsContext.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Foundation
{
	UniquePtr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				FD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateUniquePtr<OpenGLContext>(static_cast<GLFWwindow*>(window));
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}