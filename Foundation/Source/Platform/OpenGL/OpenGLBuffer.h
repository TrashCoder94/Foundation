#pragma once

#include "Foundation/Renderer/Buffer.h"

namespace Foundation
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
		public:
			OpenGLVertexBuffer(uint32_t size);
			OpenGLVertexBuffer(float* vertices, uint32_t size);
			~OpenGLVertexBuffer();

			void Bind() const override;
			void Unbind() const override;

			virtual void SetData(const void* data, uint32_t size) override;

			virtual void SetLayout(const BufferLayout& layout) override;
			virtual const BufferLayout& GetLayout() const override;

		private:
			uint32_t m_RendererID;
			BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
		public:
			OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
			~OpenGLIndexBuffer();

			void Bind() const override;
			void Unbind() const override;
			uint32_t GetCount() const override;

		private:
			uint32_t m_RendererID;
			uint32_t m_Count;
	};
}