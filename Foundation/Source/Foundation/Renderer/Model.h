#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <string>

namespace fs = std::filesystem;

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;
enum aiTextureType;

namespace Foundation
{
	class Mesh;
	class Shader;
	struct MeshTexture;

	class Model
	{
		protected:
			explicit Model(const std::string& filepath);

		public:
			virtual ~Model();
			const std::string& GetFilepath() { return m_Filepath; }

		protected:
			void Load();

		public:
			void Draw(const SharedPtr<Shader>& pShader);
		
		protected:
			// model data
			std::vector<SharedPtr<Mesh>> m_pMeshes;
			std::string m_Filepath;
			std::vector<MeshTexture> m_LoadedTextures;

			virtual unsigned int TextureFromFile(const fs::path& filepath) { return 0; };

			void ProcessNode(aiNode* pNode, const aiScene* pScene);
			SharedPtr<Mesh> ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
			void LoadMaterialTextures(aiMaterial* pMat, aiTextureType type, const std::string& typeName, std::vector<MeshTexture>& outTextures);

		public:
			static SharedPtr<Model> Create(const std::string& filepath);
	};
}