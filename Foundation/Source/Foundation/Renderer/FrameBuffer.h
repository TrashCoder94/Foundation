#pragma once

#include "Foundation/Core/Base.h"

namespace Foundation
{
	struct FrameBufferSpecification
	{
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		//FrameBufferFormat = ;
		uint32_t m_Samples = 1;

		bool m_SwapChainTarget = false;
	};

	class FrameBuffer
	{
		public:
			virtual ~FrameBuffer() = default;
			
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;
			
			virtual void Resize(uint32_t width, uint32_t height) = 0;

			virtual const uint32_t GetColourAttachmentRendererID() const = 0;
			virtual const FrameBufferSpecification& GetFrameBufferSpecification() const = 0;

			static SharedPtr<FrameBuffer> Create(const FrameBufferSpecification& frameBufferSpecification);
	};
}
