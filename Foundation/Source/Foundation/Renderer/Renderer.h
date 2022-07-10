#pragma once

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Foundation
{
	class Renderer
	{
		public:
			static void Init();
			static void Shutdown();

			static void OnWindowResized(uint32_t width, uint32_t height);

			static void BeginScene(OrthographicCamera& camera);	// TODO: Scene parameters: camera, lighting, environment.
			static void EndScene();

			static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

			inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		private:
			struct SceneData
			{
				glm::mat4 m_ViewProjectionMatrix;
			};

			static UniquePtr<SceneData> s_pSceneData;
	};
}