#include "fdpch.h"
#include "Renderer.h"

// TEMP
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Foundation
{
	UniquePtr<Renderer::SceneData> Renderer::s_pSceneData = CreateUniquePtr<Renderer::SceneData>();

	void Renderer::Init()
	{
		FD_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_pSceneData->m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const glm::mat4& transform /*= glm::mat4(1.0f)*/)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_pSceneData->m_ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);	// The model matrix.

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}