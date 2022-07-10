#pragma once

#include "Foundation/Renderer/RendererAPI.h"

namespace Foundation
{
	class OpenGLRendererAPI : public RendererAPI
	{
		public:
			OpenGLRendererAPI();
			~OpenGLRendererAPI();

			void Init() override;
			void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

			void SetClearColour(const glm::vec4& clearColour) override;
			void Clear() override;

			void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};
}