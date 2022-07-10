#include "fdpch.h"
#include "OpenGLShader.h"

#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

namespace Foundation
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		FD_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) :
		m_Name(""),
		m_RendererID(0)
	{
		FD_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Returns the file's name stripped of the extension.
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) :
		m_Name(name),
		m_RendererID(0)
	{
		FD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		FD_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		FD_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		FD_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) const
	{
		FD_PROFILE_FUNCTION();

		UploadUniformMat4(name, matrix);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}

	//
	// Assumes you bind the program before you upload uniforms.
	//

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
	{
		// Get the uniform var from the shader source.
		// -1 means the var doesn't exist in the shader source.
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		// Sends this vec3 to the uniform var in shader.
		// i means int, f means float, v means array, etc.
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		FD_PROFILE_FUNCTION();

		std::string result;
		std::ifstream file(filepath.c_str(), std::ios::in | std::ios::binary);
		if (file)
		{
			const fs::path& filePath(filepath);
			const size_t& inputFileSize = static_cast<size_t>(fs::file_size(filePath));
			
			file.seekg(0, std::ios::end);
			result.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			if (result.size() <= inputFileSize)
			{
				file.read(&result[0], result.size());
			}
			else
			{
				FD_CORE_LOG_ERROR("Result size {0} bytes was greater than the buffer of the file '{1}' of {2} bytes", result.size(), filepath.c_str(), inputFileSize);
			}
			file.close();
		}
		else
		{
			FD_CORE_LOG_ERROR("Could not open file '{0}'", filepath.c_str());
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		FD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const std::string& typeToken = "#type";
		const size_t& typeTokenLength = typeToken.size();
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			FD_CORE_ASSERT(eol != std::string::npos, "Shader syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			FD_CORE_ASSERT(ShaderTypeFromString(type), "Shader type not supported!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		FD_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());

		for (auto&& [key, value] : shaderSources)
		{
			const GLenum type = key;
			const std::string& source = value;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				FD_CORE_LOG_ERROR("{0}", infoLog.data());
				FD_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (GLenum shaderID : glShaderIDs)
			{
				glDeleteShader(shaderID);
			}

			FD_CORE_LOG_ERROR("{0}", infoLog.data());
			FD_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (GLenum shaderID : glShaderIDs)
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		m_RendererID = program;
	}
}