#include <Foundation.h>


class ExampleLayer : public Foundation::Layer
{
public:
	ExampleLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Foundation::Timestep ts) override;

	void OnRender() override;
	void OnImGuiRender() override;

	void OnEvent(Foundation::Event& event) override;

private:
	Foundation::ShaderLibrary m_ShaderLibrary;
	Foundation::SharedPtr<Foundation::Shader> m_pShader;
	Foundation::SharedPtr<Foundation::VertexArray> m_pVertexArray;

	Foundation::SharedPtr<Foundation::Shader> m_pFlatColourShader;
	Foundation::SharedPtr<Foundation::VertexArray> m_pSquareVertexArray;

	Foundation::SharedPtr<Foundation::Texture2D> m_pTexture;
	Foundation::SharedPtr<Foundation::Texture2D> m_pCodeJourneyLogoTexture;

	Foundation::OrthographicCameraController m_OrthographicCameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed;
	glm::vec3 m_SquareColour;

	glm::mat4 m_TriangleTransform;
};