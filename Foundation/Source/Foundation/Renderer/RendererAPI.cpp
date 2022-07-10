#include "fdpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Foundation
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	UniquePtr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:
			{
				FD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateUniquePtr<OpenGLRendererAPI>();
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}