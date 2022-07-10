#include "ExampleLayer.h"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

ExampleLayer::ExampleLayer() : Layer("Example"),
	m_pShader(nullptr),
	m_pVertexArray(Foundation::VertexArray::Create()),
	m_pFlatColourShader(nullptr),
	m_pSquareVertexArray(nullptr),
	m_OrthographicCameraController(1280.0f / 720.0f),
	m_SquarePosition(glm::vec3(0.0f)),
	m_SquareMoveSpeed(1.0f),
	m_SquareColour(0.2f, 0.3f, 0.8f),
	m_TriangleTransform(glm::mat4(1.0f))
{
	// ============================================================
	// TRIANGLE

	// Counter clockwise winding - the default setting.
	float vertices[3 * 7] =
	{
		// Vertex position	// Pixel colour
		-0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,	// Index Buffer 0
		 0.5f, -0.5f, 0.0f,	0.2f, 0.3f, 0.8f, 1.0f,	// Index Buffer 1
		 0.0f,  0.5f, 0.0f,	0.8f, 0.8f, 0.2f, 1.0f	// Index Buffer 2
	};

	Foundation::SharedPtr<Foundation::VertexBuffer> pVertexBuffer = Foundation::VertexBuffer::Create(vertices, sizeof(vertices));
	Foundation::BufferLayout layout =
	{
		{ Foundation::ShaderDataType::Float3, "m_Position" },
		{ Foundation::ShaderDataType::Float4, "m_Colour" }
	};
	pVertexBuffer->SetLayout(layout);
	m_pVertexArray->AddVertexBuffer(pVertexBuffer);

	// Three points in our triangle.
	// So use them in the order we have specified for our vertex array - counter clockwise.
	unsigned int indices[3] =
	{
		0, 1, 2
	};
	Foundation::SharedPtr<Foundation::IndexBuffer> pIndexBuffer = Foundation::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_pVertexArray->SetIndexBuffer(pIndexBuffer);

	// ============================================================
	// SQUARE

	// Counter clockwise winding - the default setting.
	float squareVertices[5 * 4] =
	{
		// Vertex position   // Texture Coordinates
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom Left
		 0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, // Bottom Right
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // Top Right
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // Top Left
	};

	m_pSquareVertexArray = Foundation::VertexArray::Create();

	Foundation::SharedPtr<Foundation::VertexBuffer> pSquareVertexBuffer = Foundation::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	pSquareVertexBuffer->SetLayout({
			{ Foundation::ShaderDataType::Float3, "m_Position" },
			{ Foundation::ShaderDataType::Float2, "m_TexCoord" }
		});
	m_pSquareVertexArray->AddVertexBuffer(pSquareVertexBuffer);

	unsigned int squareIndices[6] =
	{
		0, 1, 2, 2, 3, 0
	};
	Foundation::SharedPtr<Foundation::IndexBuffer> pSquareIndexBuffer = Foundation::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_pSquareVertexArray->SetIndexBuffer(pSquareIndexBuffer);

	// ============================================================
	// SHADERS

	std::string vertexSource = R"(
				#version 330 core

				layout(location = 0) in vec3 m_Position;
				layout(location = 1) in vec4 m_Colour;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;
				out vec4 v_Colour;

				void main()
				{
					v_Position = m_Position;
					v_Colour = m_Colour;
					gl_Position = u_ViewProjection * u_Transform * vec4(m_Position, 1.0);
				}
			)";

	std::string fragmentSource = R"(
				#version 330 core

				layout(location = 0) out vec4 m_Colour;

				uniform vec3 u_Colour;

				in vec3 v_Position;
				in vec4 v_Colour;

				void main()
				{
					m_Colour = vec4(v_Position * 0.5 + 0.5, 1.0);
					m_Colour = v_Colour;
				}
			)";

	m_pShader = Foundation::Shader::Create("VertexPositionColour", vertexSource, fragmentSource);

	std::string flatColourShaderVertexSource = R"(
				#version 330 core

				layout(location = 0) in vec3 m_Position;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main()
				{
					v_Position = m_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(m_Position, 1.0);
				}
			)";

	std::string flatColourShaderFragmentSource = R"(
				#version 330 core

				layout(location = 0) out vec4 m_Colour;

				uniform vec3 u_Colour;

				in vec3 v_Position;

				void main()
				{
					m_Colour = vec4(u_Colour, 1.0);
				}
			)";

	m_pFlatColourShader = Foundation::Shader::Create("FlatColour", flatColourShaderVertexSource, flatColourShaderFragmentSource);

	Foundation::SharedPtr<Foundation::Shader> pTextureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");

	m_pTexture = Foundation::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_pCodeJourneyLogoTexture = Foundation::Texture2D::Create("Assets/Textures/CodeJourneyLogoTransparent.png");

	pTextureShader->Bind();
	pTextureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Foundation::Timestep ts)
{
	// Camera Movement + Rotation
	m_OrthographicCameraController.OnUpdate(ts);

	// Square Movement
	if (Foundation::Input::IsKeyPressed(Foundation::Key::I))
	{
		m_SquarePosition.y += (m_SquareMoveSpeed * ts);
	}
	else if (Foundation::Input::IsKeyPressed(Foundation::Key::K))
	{
		m_SquarePosition.y -= (m_SquareMoveSpeed * ts);
	}

	if (Foundation::Input::IsKeyPressed(Foundation::Key::L))
	{
		m_SquarePosition.x += (m_SquareMoveSpeed * ts);
	}
	else if (Foundation::Input::IsKeyPressed(Foundation::Key::J))
	{
		m_SquarePosition.x -= (m_SquareMoveSpeed * ts);
	}
}

void ExampleLayer::OnRender()
{
	Foundation::RenderCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1.0f });
	Foundation::RenderCommand::Clear();

	Foundation::Renderer::BeginScene(m_OrthographicCameraController.GetCamera());
	{
		m_pFlatColourShader->Bind();
		m_pFlatColourShader->SetFloat3("u_Colour", m_SquareColour);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(m_SquarePosition.x + x * 0.11f, m_SquarePosition.y + y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Foundation::Renderer::Submit(m_pFlatColourShader, m_pSquareVertexArray, transform);
			}
		}

		Foundation::SharedPtr<Foundation::Shader> pTextureShader = m_ShaderLibrary.Get("Texture");

		m_pTexture->Bind();
		Foundation::Renderer::Submit(pTextureShader, m_pSquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_pCodeJourneyLogoTexture->Bind();
		Foundation::Renderer::Submit(pTextureShader, m_pSquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Foundation::Renderer::Submit(m_pShader, m_pVertexArray, m_TriangleTransform);
	}
	Foundation::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	{
		ImGui::ColorEdit3("Square Colour", glm::value_ptr(m_SquareColour));
	}
	ImGui::End();
}

void ExampleLayer::OnEvent(Foundation::Event& event)
{
	m_OrthographicCameraController.OnEvent(event);
}
