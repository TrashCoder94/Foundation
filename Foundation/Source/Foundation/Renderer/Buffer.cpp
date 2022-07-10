#include "fdpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Foundation
{
	/////////////////////////////////////////////////////////////////////////////////
	// Buffer Element

	BufferElement::BufferElement() :
		m_Type(ShaderDataType::Float),
		m_Name(""),
		m_Size(ShaderDataTypeSize(ShaderDataType::Float)),
		m_Offset(0),
		m_Normalized(false)
	{
	}

	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized) :
		m_Type(type),
		m_Name(name),
		m_Size(ShaderDataTypeSize(type)),
		m_Offset(0),
		m_Normalized(normalized)
	{
	}

	uint32_t BufferElement::GetComponentCount() const
	{
		switch (m_Type)
		{
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3; // 3 * Float3
			case ShaderDataType::Mat4:		return 4; // 4 * Float4
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
		}

		FD_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Buffer Layout

	BufferLayout::BufferLayout() :
		m_Elements(),
		m_Stride(0)
	{
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& element) :
		m_Elements(element),
		m_Stride(0)
	{
		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (BufferElement& element : m_Elements)
		{
			element.m_Offset = offset;
			offset += element.m_Size;
			m_Stride += element.m_Size;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Vertex Buffer

	VertexBuffer::~VertexBuffer()
	{
	}

	SharedPtr<VertexBuffer> VertexBuffer::Create(uint32_t size)
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
				return CreateSharedPtr<OpenGLVertexBuffer>(size);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	SharedPtr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
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
				return CreateSharedPtr<OpenGLVertexBuffer>(vertices, size);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Index Buffer

	IndexBuffer::~IndexBuffer()
	{
	}

	SharedPtr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
				return CreateSharedPtr<OpenGLIndexBuffer>(indices, count);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}