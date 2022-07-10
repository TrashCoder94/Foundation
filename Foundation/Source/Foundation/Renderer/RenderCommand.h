#pragma once

#include "RendererAPI.h"

namespace Foundation
{
	class RenderCommand
	{
		public:
			inline static void Init()
			{
				s_pRendererAPI->Init();
			}

			inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
			{
				s_pRendererAPI->SetViewport(x, y, width, height);
			}

			inline static void SetClearColour(const glm::vec4& clearColour)
			{
				s_pRendererAPI->SetClearColour(clearColour);
			}

			inline static void Clear()
			{
				s_pRendererAPI->Clear();
			}

			inline static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t count = 0)
			{
				s_pRendererAPI->DrawIndexed(vertexArray, count);
			}

		private:
			static UniquePtr<RendererAPI> s_pRendererAPI;
	};
}