#include "fdpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Foundation
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		FD_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		FD_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		FD_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		FD_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		FD_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer)
	{
		FD_PROFILE_FUNCTION();

		FD_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t vertexBufferIndex = 0;
		const BufferLayout& vertexLayout = vertexBuffer->GetLayout();
		for (const BufferElement& element : vertexLayout)
		{
			switch (element.m_Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					// Enable index 0 of our vertex array.
					glEnableVertexAttribArray(vertexBufferIndex);

					// Describing our vertex data.
					glVertexAttribPointer(vertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.m_Type),
						element.m_Normalized ? GL_TRUE : GL_FALSE,
						vertexLayout.GetStride(),
						(const void*)(UINT_PTR)element.m_Offset);

					vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; ++i)
					{
						// Enable index 0 of our vertex array.
						glEnableVertexAttribArray(vertexBufferIndex);

						// Describing our vertex data.
						glVertexAttribPointer(vertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element.m_Type),
							element.m_Normalized ? GL_TRUE : GL_FALSE,
							vertexLayout.GetStride(),
							(const void*)(element.m_Offset + sizeof(float) * count * i));

						vertexBufferIndex++;
					}
					
					break;
				}
				default:
				{
					FD_CORE_ASSERT(false, "Unknown ShaderDataType!");
					break;
				}
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer)
	{
		FD_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_pIndexBuffer = indexBuffer;
	}

	const std::vector<SharedPtr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const SharedPtr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_pIndexBuffer;
	}
}
