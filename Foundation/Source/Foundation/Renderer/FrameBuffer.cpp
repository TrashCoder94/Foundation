#include "fdpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Foundation
{
	SharedPtr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& frameBufferSpecification)
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
				return CreateSharedPtr<OpenGLFrameBuffer>(frameBufferSpecification);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}