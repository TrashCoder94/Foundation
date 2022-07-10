#include "fdpch.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLMesh.h"

namespace Foundation
{
	Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures) :
		m_Vertices(vertices),
		m_Indices(indices),
		m_Textures(textures),
		VAO(0),
		VBO(0),
		EBO(0)
	{
		SetupMesh();
	}

	Mesh::~Mesh()
	{
		m_Vertices.clear();
		m_Indices.clear();
		m_Textures.clear();
	}

	SharedPtr<Mesh> Mesh::Create(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures)
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
				return CreateSharedPtr<OpenGLMesh>(vertices, indices, textures);
			}
		}

		FD_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}