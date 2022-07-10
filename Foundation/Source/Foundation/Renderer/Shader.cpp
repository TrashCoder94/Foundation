#include "fdpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Foundation
{
	/////////////////////////////////////////////////////////////////////////////////
	// Shader

	Shader::~Shader()
	{
	}

	SharedPtr<Shader> Shader::Create(const std::string& filepath)
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
				return CreateSharedPtr<OpenGLShader>(filepath);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	SharedPtr<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& pixelSource)
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
				return CreateSharedPtr<OpenGLShader>(name, vertexSource, pixelSource);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////
	// Shader Library

	void ShaderLibrary::Add(const std::string& name, const SharedPtr<Shader>& shader)
	{
		FD_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const SharedPtr<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		SharedPtr<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		SharedPtr<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		FD_CORE_ASSERT(Exists(name), "Shader doesn't exist!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}