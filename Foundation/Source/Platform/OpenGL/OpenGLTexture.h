#pragma once

#include "Foundation/Renderer/Texture.h"
#include <glad/glad.h>

namespace Foundation
{
	class OpenGLTexture2D : public Texture2D
	{
		public:
			OpenGLTexture2D(uint32_t width, uint32_t height);
			OpenGLTexture2D(const std::string& filepath);
			~OpenGLTexture2D();

			uint32_t GetWidth() const override { return m_Width; }
			uint32_t GetHeight() const override { return m_Height; }
			uint32_t GetRendererID() const override { return m_RendererID; }

			void SetData(void* data, uint32_t size) override;

			void Bind(uint32_t slot = 0) const override;
			void Unbind(uint32_t slot = 0) const override;

			const std::string& GetFilepath() const override { return m_Filepath; }

			virtual bool operator==(const Texture& other) const override
			{
				return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
			}

		private:
			std::string m_Filepath;
			uint32_t m_Width;
			uint32_t m_Height;
			uint32_t m_RendererID;
			GLenum m_InternalFormat;
			GLenum m_DataFormat;
	};
}