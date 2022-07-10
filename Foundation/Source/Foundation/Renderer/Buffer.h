#pragma once

#include "Foundation/Core/Base.h"

namespace Foundation
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		FD_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType m_Type;
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Offset;
		bool m_Normalized;

		BufferElement();
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		uint32_t GetComponentCount() const;
	};

	class BufferLayout
	{
		public:
			BufferLayout();
			BufferLayout(const std::initializer_list<BufferElement>& element);

			inline uint32_t GetStride() const { return m_Stride; }
			inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

			std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
			std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		private:
			void CalculateOffsetsAndStride();

		private:
			std::vector<BufferElement> m_Elements;
			uint32_t m_Stride;
	};

	class VertexBuffer
	{
		public:
			virtual ~VertexBuffer();

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void SetLayout(const BufferLayout& layout) = 0;
			virtual const BufferLayout& GetLayout() const = 0;

			virtual void SetData(const void* data, uint32_t size) = 0;

			static SharedPtr<VertexBuffer> Create(uint32_t size);
			static SharedPtr<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	// Only support 32-bit index buffers at the moment.
	class IndexBuffer
	{
		public:
			virtual ~IndexBuffer();

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual uint32_t GetCount() const = 0;

			static SharedPtr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}