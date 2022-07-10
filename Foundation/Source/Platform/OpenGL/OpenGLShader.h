#pragma once

#include "Foundation/Renderer/Shader.h"

// TODO: Remove - Should be able to do so by removing OpenGLShader.h from Sandbox.cpp
// Then we can use #include <glad/glad.h> here without any issues.
typedef unsigned int GLenum;

namespace Foundation
{
	class OpenGLShader : public Shader
	{
		public:
			OpenGLShader(const std::string& filepath);
			OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
			~OpenGLShader();

			void Bind() const override;
			void Unbind() const override;

			void SetInt(const std::string& name, int value) const override;
			void SetIntArray(const std::string& name, int* values, uint32_t count) const override;

			void SetFloat(const std::string& name, float value) const override;
			void SetFloat2(const std::string& name, const glm::vec2& value) const override;
			void SetFloat3(const std::string& name, const glm::vec3& value) const override;
			void SetFloat4(const std::string& name, const glm::vec4& value) const override;

			void SetMat3(const std::string& name, const glm::mat3& matrix) const override;
			void SetMat4(const std::string& name, const glm::mat4& matrix) const override;

			const std::string& GetName() const override;

		private:
			void UploadUniformInt(const std::string& name, int value) const;
			void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const;
			
			void UploadUniformFloat(const std::string& name, float value) const;
			void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
			void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
			void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
			
			void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
			void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

			std::string ReadFile(const std::string& filepath);
			std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
			void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		private:
			std::string m_Name;
			uint32_t m_RendererID;
	};
}