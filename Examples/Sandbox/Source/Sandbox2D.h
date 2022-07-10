#pragma once

#include <Foundation.h>

class Sandbox2D : public Foundation::Layer
{
	public:
		Sandbox2D();
		~Sandbox2D();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Foundation::Timestep ts) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnEvent(Foundation::Event& event) override;

	private:
		Foundation::OrthographicCameraController m_OrthographicCameraController;
		glm::vec4 m_SquareColour;
		float m_Rotation;
		float m_RotationSpeed;

		Foundation::SharedPtr<Foundation::Texture2D> m_pCheckerboardTexture;
		Foundation::SharedPtr<Foundation::Texture2D> m_pSpriteSheet;
		
		Foundation::ParticleProperties m_ParticleProperties;
		Foundation::ParticleSystem m_ParticleSystem;
};