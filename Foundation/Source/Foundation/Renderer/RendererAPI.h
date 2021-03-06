#pragma once

#include <glm/vec4.hpp>

#include "VertexArray.h"

namespace Foundation
{
	class RendererAPI
	{
		public:
			enum class API
			{
				None = 0,
				OpenGL = 1
			};

			virtual ~RendererAPI() = default;

			virtual void Init() = 0;
			virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

			virtual void SetClearColour(const glm::vec4& clearColour) = 0;
			virtual void Clear() = 0;

			virtual void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

			inline static API GetAPI() { return s_API; }

			static UniquePtr<RendererAPI> Create();

		private:
			static API s_API;
	};
}