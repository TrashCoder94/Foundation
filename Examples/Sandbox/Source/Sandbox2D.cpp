#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"),
	m_OrthographicCameraController(1920.0f / 1080.0f),
	m_SquareColour({ 0.8f, 0.2f, 0.3f, 1.0f }),
	m_Rotation(0.0f),
	m_RotationSpeed(20.0f),
	m_pCheckerboardTexture(nullptr),
	m_pSpriteSheet(nullptr),
	m_ParticleProperties(),
	m_ParticleSystem(10000)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	FD_PROFILE_FUNCTION();

	m_pCheckerboardTexture = Foundation::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_pSpriteSheet = Foundation::Texture2D::Create("Assets/Textures/roguelikeSheet_transparent.png");

//#if USE_PIXEL_COORDS
//	//m_pTextureGreenTree = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 306.0f, 323.0f }, { 17.0f, 34.0f });
//
//	m_pTextureOrangeBush = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 340.0f, 357.0f }, { 17.0f, 17.0f });
//	m_pTextureWater = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 0.0f, 509.0f }, { 17.0f, 17.0f });
//	m_pTextureDirt = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 102.0f, 509.0f }, { 17.0f, 17.0f });
//#else
//	//m_pTextureGreenTree = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 18.0f, 19.0f }, { 17.0f, 17.0f }, { 1.0f, 2.0f });
//	
//	m_pTextureOrangeBush = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 20.0f, 21.0f }, { 17.0f, 17.0f });
//	m_pTextureWater = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 0.0f, 29.0f }, { 17.0f, 17.0f });
//	m_pTextureDirt = Foundation::SubTexture2D::Create(m_pSpriteSheet, { 6.0f, 30.0f }, { 17.0f, 17.0f });
//#endif

	m_ParticleProperties.m_Position = { 0.0f, 0.0f, 0.0f };
	m_ParticleProperties.m_Velocity = { 0.0f, 0.0f, 0.0f };
	m_ParticleProperties.m_VelocityVariation = { 3.0f, 1.0f, 0.0f };
	m_ParticleProperties.m_ColourBegin = { 0.8f, 0.2f, 0.3f, 1.0f }; // Red
	m_ParticleProperties.m_ColourEnd = { 0.2f, 0.3f, 0.8f, 1.0f };	 // Blue
	m_ParticleProperties.m_SizeBegin = { 0.5f, 0.5f };
	m_ParticleProperties.m_SizeEnd = { 0.0f, 0.0f };
	m_ParticleProperties.m_SizeVariation = { 0.3f, 0.3f };
	m_ParticleProperties.m_Rotation = 0.0f;
	m_ParticleProperties.m_LifeTime = 5.0f;

	m_OrthographicCameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	FD_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Foundation::Timestep ts)
{
	FD_PROFILE_FUNCTION();
	m_OrthographicCameraController.OnUpdate(ts);
	m_Rotation += ts * m_RotationSpeed;

	if (Foundation::Input::IsMouseButtonPressed(Foundation::Mouse::ButtonLeft))
	{
		glm::vec2 mousePos = Foundation::Input::GetMousePosition();
		auto width = Foundation::Application::Get().GetWindow().GetWidth();
		auto height = Foundation::Application::Get().GetWindow().GetHeight();

		auto bounds = m_OrthographicCameraController.GetCamera().GetBounds();
		auto pos = m_OrthographicCameraController.GetCamera().GetPosition();
		mousePos.x = (mousePos.x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		mousePos.y = bounds.GetHeight() * 0.5f - (mousePos.y / height) * bounds.GetHeight();
		m_ParticleProperties.m_Position = { mousePos.x + pos.x, mousePos.y + pos.y, 1.0f };

		for (int i = 0; i < 5; ++i)
		{
			m_ParticleSystem.Emit(m_ParticleProperties);
		}
	}

	m_ParticleSystem.OnUpdate(ts);
}

void Sandbox2D::OnRender()
{
	FD_PROFILE_FUNCTION();

	Foundation::Renderer2D::ResetStats();

	{
		FD_PROFILE_SCOPE("Renderer Prep");
		Foundation::RenderCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1.0f });
		Foundation::RenderCommand::Clear();
	}

	{
		FD_PROFILE_SCOPE("Renderer Draw");

		// Particle System
		m_ParticleSystem.OnRender(m_OrthographicCameraController.GetCamera());

		Foundation::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
		{
			// Draw Coloured Quad.
			Foundation::Renderer2D::DrawQuad({ { -3.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, m_SquareColour });
			
			// Big Background
			// Draw Textured Quad.
			// OpenGL uses a right handed coordinate system, so -z means push objects further back.
			Foundation::Renderer2D::DrawQuad({ { 0.0f, 0.0f, -0.2f }, 0.0f, { 20.0f, 20.0f }, m_pCheckerboardTexture });

			// Draw Tiled Textured Quad. (Rotated).
			Foundation::Renderer2D::DrawQuad({ { -1.0f, 0.0f }, glm::radians(m_Rotation), { 1.0f, 1.0f }, m_pCheckerboardTexture, 2.0f });

			// Draw Textured Green Coloured Quad. (Rotated).
			Foundation::Renderer2D::DrawQuad({ { 1.0f, 0.0f }, glm::radians(45.0f), { 1.0f, 1.0f }, m_pCheckerboardTexture, { 0.8f, 1.0f, 0.8f, 1.0f } });

			// Slightly Bigger
			// Draw Tiled Textured Blue Coloured Quad.
			Foundation::Renderer2D::DrawQuad({ { 3.0f, 0.0f, -0.1f }, 0.0f, { 2.0f, 2.0f }, m_pCheckerboardTexture, 10.0f, { 0.8f, 0.8f, 1.0f, 1.0f } });
		}
		Foundation::Renderer2D::EndScene();

		Foundation::Renderer2D::BeginScene(m_OrthographicCameraController.GetCamera());
		{
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 colour = { ((x + 5.0f) / 10.0f), 0.4f, ((y + 5.0f) / 10.0f), 0.7f };
					Foundation::Renderer2D::DrawQuad({ { x, y }, 0.0f, { 0.45f, 0.45f }, colour });
				}
			}
		}

		Foundation::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	FD_PROFILE_FUNCTION();
	
	ImGui::Begin("Settings");
	{
		Foundation::Renderer2D::Statistics stats = Foundation::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats");
		ImGui::Text("Draw   Calls: %d", stats.m_DrawCalls);
		ImGui::Text("Quad   Count: %d", stats.m_QuadCount);
		ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("Index  Count: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Particle Begin Colour", glm::value_ptr(m_ParticleProperties.m_ColourBegin));
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Foundation::Event& event)
{
	m_OrthographicCameraController.OnEvent(event);
}
