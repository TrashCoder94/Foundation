#include "fdpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Foundation
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
		m_Filepath(""),
		m_Width(width),
		m_Height(height),
		m_RendererID(0),
		m_InternalFormat(GL_RGBA8),
		m_DataFormat(GL_RGBA)
	{
		FD_PROFILE_FUNCTION();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) :
		m_Filepath(filepath),
		m_Width(0),
		m_Height(0),
		m_RendererID(0),
		m_InternalFormat(0),
		m_DataFormat(0)
	{
		FD_PROFILE_FUNCTION();

		int width = 0;
		int height = 0;
		int channels = 0;

		// OpenGL expects textures to be given bottom to top, stbi works in the opposite direction.
		// Use this function to flip it and sync up with OpenGL.
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		{
			FD_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}
		FD_CORE_ASSERT(data, "Texture failed to load!");

		m_Width = width;
		m_Height = height;

		GLint internalFormat = 0;
		GLint dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		FD_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		FD_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		FD_PROFILE_FUNCTION();

		uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		FD_CORE_ASSERT(size == (m_Width * m_Height * bytesPerPixel), "Data must be the entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot /*= 0*/) const
	{
		FD_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot /*= 0*/) const
	{
		FD_PROFILE_FUNCTION();

		glBindTextureUnit(slot, 0);
	}
}