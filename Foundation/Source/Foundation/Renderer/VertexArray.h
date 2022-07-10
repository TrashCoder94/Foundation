#pragma once

#include <memory>
#include "Buffer.h"

namespace Foundation
{
	class VertexArray
	{
	public:
		virtual ~VertexArray();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;

		static SharedPtr<VertexArray> Create();
	};
}