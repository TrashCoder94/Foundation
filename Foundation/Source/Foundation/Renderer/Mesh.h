#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Foundation/Core/Base.h"

namespace Foundation
{
	class Shader;

	struct MeshVertex
	{
		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec2 m_TextureCoordinates;
		glm::vec3 m_Tangent;
		glm::vec3 m_Bitangent;

		MeshVertex() :
			m_Position(0.0f),
			m_Normal(0.0f),
			m_TextureCoordinates(0.0f),
			m_Tangent(0.0f),
			m_Bitangent(0.0f)
		{}
	};

	struct MeshTexture
	{
		unsigned int id;
		std::string type;
		std::string path;

		MeshTexture() :
			id(0),
			type(""),
			path("")
		{}
	};

	class Mesh
	{
		public:
			std::vector<MeshVertex> m_Vertices;
			std::vector<unsigned int> m_Indices;
			std::vector<MeshTexture> m_Textures;

		protected:
			explicit Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
			
		public:	
			virtual ~Mesh();
			virtual void Draw(const SharedPtr<Shader>& pShader) = 0;

		protected:
			virtual void SetupMesh() {};

			unsigned int VAO;
			unsigned int VBO;
			unsigned int EBO;

		public:
			static SharedPtr<Mesh> Create(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);			
	};
}