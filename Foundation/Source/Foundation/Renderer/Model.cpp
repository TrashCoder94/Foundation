#include "fdpch.h"
#include "Model.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLModel.h"

namespace Foundation
{
	Model::Model(const std::string& filepath) :
		m_pMeshes(),
		m_Filepath(filepath),
		m_LoadedTextures()
	{
		{
			FD_PROFILE_SCOPE("Load - Model::Model(const std::string& filepath)");
			Load();
		}
	}

	Model::~Model()
	{
	}

	void Model::Load()
	{
		Assimp::Importer import;
		const aiScene* pScene = import.ReadFile(m_Filepath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
		{
			std::string errorMessage = "ERROR::ASSIMP::" + std::string(import.GetErrorString());
			FD_CORE_ASSERT(false, errorMessage);
			return;
		}

		ProcessNode(pScene->mRootNode, pScene);
	}

	void Model::Draw(const SharedPtr<Shader>& pShader)
	{
		for (const SharedPtr<Mesh>& pMesh : m_pMeshes)
		{
			if (!pMesh)
			{
				continue;
			}

			pMesh->Draw(pShader);
		}
	}

	void Model::ProcessNode(aiNode* pNode, const aiScene* pScene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
		{
			aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_pMeshes.push_back(ProcessMesh(pMesh, pScene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}

	SharedPtr<Mesh> Model::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
	{
		// data to fill
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			MeshVertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = pMesh->mVertices[i].x;
			vector.y = pMesh->mVertices[i].y;
			vector.z = pMesh->mVertices[i].z;
			vertex.m_Position = vector;
			// normals
			if (pMesh->HasNormals())
			{
				vector.x = pMesh->mNormals[i].x;
				vector.y = pMesh->mNormals[i].y;
				vector.z = pMesh->mNormals[i].z;
				vertex.m_Normal = vector;
			}
			// texture coordinates
			if (pMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = pMesh->mTextureCoords[0][i].x;
				vec.y = pMesh->mTextureCoords[0][i].y;
				vertex.m_TextureCoordinates = vec;

				if (pMesh->mTangents)
				{
					// tangent
					vector.x = pMesh->mTangents[i].x;
					vector.y = pMesh->mTangents[i].y;
					vector.z = pMesh->mTangents[i].z;
					vertex.m_Tangent = vector;
				}

				if (pMesh->mBitangents)
				{
					// bitangent
					vector.x = pMesh->mBitangents[i].x;
					vector.y = pMesh->mBitangents[i].y;
					vector.z = pMesh->mBitangents[i].z;
					vertex.m_Bitangent = vector;
				}
			}
			else
				vertex.m_TextureCoordinates = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			aiFace face = pMesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		if (pMesh->mMaterialIndex >= 0)
		{
			aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
			
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN

			// 1. diffuse maps
			std::vector<MeshTexture> diffuseMaps;
			LoadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "texture_diffuse", diffuseMaps);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// 2. specular maps
			std::vector<MeshTexture> specularMaps;
			LoadMaterialTextures(pMaterial, aiTextureType_SPECULAR, "texture_specular", specularMaps);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			// 3. normal maps
			std::vector<MeshTexture> normalMaps;
			LoadMaterialTextures(pMaterial, aiTextureType_NORMALS, "texture_normal", normalMaps);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			// 4. height maps
			std::vector<MeshTexture> heightMaps;
			LoadMaterialTextures(pMaterial, aiTextureType_HEIGHT, "texture_height", heightMaps);
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}

		// return a mesh object created from the extracted mesh data
		return Mesh::Create(vertices, indices, textures);
	}

	void Model::LoadMaterialTextures(aiMaterial* pMat, aiTextureType type, const std::string& typeName, std::vector<MeshTexture>& outTextures)
	{
		for (unsigned int i = 0; i < pMat->GetTextureCount(type); i++)
		{
			aiString str;
			pMat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
			{
				if (std::strcmp(m_LoadedTextures[j].path.data(), str.C_Str()) == 0)
				{
					outTextures.push_back(m_LoadedTextures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				MeshTexture texture;
				const std::string& stringFullFilepath = fs::path(m_Filepath).parent_path().string() + "\\" + str.C_Str();
				const fs::path& fullFilepath = fs::path(stringFullFilepath);
				texture.id = TextureFromFile(fullFilepath);
				texture.type = typeName;
				texture.path = str.C_Str();
				outTextures.push_back(texture);
				m_LoadedTextures.push_back(texture); // add to loaded textures
			}
		}
	}

	SharedPtr<Model> Model::Create(const std::string& filepath)
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
				return CreateSharedPtr<OpenGLModel>(filepath);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}