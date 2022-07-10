#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Foundation
{
	class Shader
	{
		public:
			virtual ~Shader();

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void SetInt(const std::string& name, int value) const = 0;
			virtual void SetIntArray(const std::string& name, int* values, uint32_t count) const = 0;

			virtual void SetFloat(const std::string& name, float value) const = 0;
			virtual void SetFloat2(const std::string& name, const glm::vec2& value) const = 0;
			virtual void SetFloat3(const std::string& name, const glm::vec3& value) const = 0;
			virtual void SetFloat4(const std::string& name, const glm::vec4& value) const = 0;
 
			virtual void SetMat3(const std::string& name, const glm::mat3& matrix) const = 0;
			virtual void SetMat4(const std::string& name, const glm::mat4& matrix) const = 0;

			virtual const std::string& GetName() const = 0;

			static SharedPtr<Shader> Create(const std::string& filepath);
			static SharedPtr<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& pixelSource);
	};

	class ShaderLibrary
	{
		public:
			void Add(const std::string& name, const SharedPtr<Shader>& shader);
			void Add(const SharedPtr<Shader>& shader);
			
			SharedPtr<Shader> Load(const std::string& filepath);
			SharedPtr<Shader> Load(const std::string& name, const std::string& filepath);

			SharedPtr<Shader> Get(const std::string& name);

			bool Exists(const std::string& name) const;

		private:
			std::unordered_map<std::string, SharedPtr<Shader>> m_Shaders;
	};
}