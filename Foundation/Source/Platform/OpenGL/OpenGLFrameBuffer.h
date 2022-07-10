#pragma once

#include "Foundation/Renderer/FrameBuffer.h"

namespace Foundation
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
		public:
			OpenGLFrameBuffer(const FrameBufferSpecification& frameBufferSpecification);
			~OpenGLFrameBuffer();

			void Invalidate();

			void Bind() const override;
			void Unbind() const override;

			void Resize(uint32_t width, uint32_t height) override;

			const uint32_t GetColourAttachmentRendererID() const override { return m_ColourAttachment; };
			const FrameBufferSpecification& GetFrameBufferSpecification() const override { return m_Specification; };

		private:
			uint32_t m_RendererID;
			uint32_t m_ColourAttachment;
			uint32_t m_DepthAttachment;
			FrameBufferSpecification m_Specification;
	};
}