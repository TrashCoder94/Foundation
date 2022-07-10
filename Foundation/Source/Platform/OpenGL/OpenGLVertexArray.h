#pragma once

#include "Foundation/Renderer/VertexArray.h"

namespace Foundation
{
	class OpenGLVertexArray : public VertexArray
	{
		public:
			OpenGLVertexArray();
			~OpenGLVertexArray();

			void Bind() const override;
			void Unbind() const override;

			void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) override;
			void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) override;

			const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override;
			const SharedPtr<IndexBuffer>& GetIndexBuffer() const override;

		private:
			uint32_t m_RendererID;
			std::vector<SharedPtr<VertexBuffer>> m_VertexBuffers;
			SharedPtr<IndexBuffer> m_pIndexBuffer;
	};
}