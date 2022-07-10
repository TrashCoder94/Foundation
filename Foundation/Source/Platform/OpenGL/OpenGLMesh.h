#pragma once

#include "Foundation/Renderer/Mesh.h"

namespace Foundation
{
	class OpenGLMesh : public Mesh
	{
		public:
			explicit OpenGLMesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MeshTexture>& textures);
			~OpenGLMesh();

			virtual void Draw(const SharedPtr<Shader>& pShader) override;

		protected:
			virtual void SetupMesh() override;
	};
}