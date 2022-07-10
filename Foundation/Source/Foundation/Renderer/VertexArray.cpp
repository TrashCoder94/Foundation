#include "fdpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Foundation
{
	VertexArray::~VertexArray()
	{
	}

	SharedPtr<VertexArray> VertexArray::Create()
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
				return CreateSharedPtr<OpenGLVertexArray>();
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}