#include "fdpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Foundation
{
	UniquePtr<RendererAPI> RenderCommand::s_pRendererAPI = RendererAPI::Create();
}