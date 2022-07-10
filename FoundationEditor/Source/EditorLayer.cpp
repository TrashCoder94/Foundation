#include "EditorLayer.h"

#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <iostream>

#include <Foundation/Components/CameraComponent.h>
#include <Foundation/Components/TransformComponent.h>
#include <Foundation/Scene/Scene.h>
#include <Foundation/Scene/SceneSerializer.h>
#include <Foundation/Utilities/PlatformUtilities.h>
//#include <ImGuizmo.h>

#include "Objects/EditorCamera.h"

namespace Foundation
{
	EditorLayer::EditorLayer() : Layer("EditorLayer"),
		m_SceneState(SceneState::Edit),
		m_pEditorCamera(nullptr),
		m_pFrameBuffer(nullptr),
		m_pEditorScene(nullptr),
		m_pRuntimeScene(nullptr),
		m_pPlayButtonTexture(nullptr),
		m_pStopButtonTexture(nullptr),
		m_pTranslateGizmoTexture(nullptr),
		m_pRotateGizmoTexture(nullptr),
		m_pScaleGizmoTexture(nullptr),
		m_pSquareObject(),
		m_pMainCameraObject(), 
		m_pSecondCameraObject(),
		m_GizmoType(-1),
		m_PrimaryCamera(true),
		m_ViewportSize(glm::vec2(0.0f)),
		m_ViewportFocused(false),
		m_ViewportHovered(false),
		m_SceneHierarchyPanel()
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		FD_PROFILE_FUNCTION();

		FrameBufferSpecification fbSpec;
		fbSpec.m_Width = 1280;
		fbSpec.m_Height = 720;
		m_pFrameBuffer = FrameBuffer::Create(fbSpec);

		m_pEditorScene = CreateSharedPtr<Scene>();

		m_SceneHierarchyPanel.SetScene(m_pEditorScene);

		m_pPlayButtonTexture = Texture2D::Create("Assets/Textures/EditorPlayButton.png");
		m_pStopButtonTexture = Texture2D::Create("Assets/Textures/EditorStopButton.png");

		CreateEditorCamera();
	}

	void EditorLayer::OnDetach()
	{
		FD_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		FD_PROFILE_FUNCTION();

		FrameBufferSpecification spec = m_pFrameBuffer->GetFrameBufferSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y &&
			(spec.m_Width != m_ViewportSize.x || spec.m_Height != m_ViewportSize.y))
		{
			m_pFrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			switch (m_SceneState)
			{
				case SceneState::Edit:
				{
					m_pEditorScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
					break;
				}
				case SceneState::Play:
				{
					m_pRuntimeScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
					break;
				}
			}
		}

		if (m_ViewportFocused)
		{
			if (m_SceneState == SceneState::Play)
			{
				if (Input::IsKeyPressed(Key::Escape))
				{
					OnSceneStop();
				}
			}

			//m_pEditorCamera.Update(ts);

			switch (m_SceneState)
			{
				case SceneState::Edit:
				{
					m_pEditorScene->Update(ts);
					break;
				}
				case SceneState::Play:
				{
					m_pRuntimeScene->Update(ts); 
					break;
				}
			}
		}
	}

	void EditorLayer::OnRender()
	{
		FD_PROFILE_FUNCTION();

		Renderer2D::ResetStats();
		m_pFrameBuffer->Bind();
		RenderCommand::SetClearColour({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				m_pEditorScene->Render();
				break;
			}
			case SceneState::Play:
			{
				m_pRuntimeScene->Render();
				break;
			}
		}

		m_pFrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		FD_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static constexpr bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Foundation Dockspace", &dockspaceOpen, window_flags);
		{
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWindowSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("FoundationDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWindowSizeX;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						SaveSceneAs();
					}

					if (ImGui::MenuItem("Exit"))
					{
						Application::Get().Close();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_SceneHierarchyPanel.OnImGuiRender();

			bool editorPlayButtonPressed = false;
			ImGui::Begin("Toolbar");
			{
				float buttonWidth = 32.0f;
				float width = ImGui::GetContentRegionAvail().x;
				float halfWidth = width * 0.5f;

				switch (m_SceneState)
				{
					case SceneState::Edit:
					{
						ImGui::SameLine(halfWidth - buttonWidth);
						if (ImGui::ImageButton((ImTextureID)(UINT_PTR)m_pPlayButtonTexture->GetRendererID(), ImVec2{ 32, 32 }))
						{
							OnScenePlay();
							editorPlayButtonPressed = true;
						}

						break;
					}
					case SceneState::Play:
					{
						ImGui::SameLine(halfWidth - buttonWidth);
						if (ImGui::ImageButton((ImTextureID)(UINT_PTR)m_pStopButtonTexture->GetRendererID(), ImVec2{ 32, 32 }))
						{
							OnSceneStop();
						}

						break;
					}
					case SceneState::Pause:
					{
						break;
					}
				}
			}
			ImGui::End();

			ImGui::Begin("Stats");
			{
				Renderer2D::Statistics stats = Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats");
				ImGui::Text("Draw   Calls: %d", stats.m_DrawCalls);
				ImGui::Text("Quad   Count: %d", stats.m_QuadCount);
				ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
				ImGui::Text("Index  Count: %d", stats.GetTotalIndexCount());
			}
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
			ImGui::Begin("Viewport");
			{
				if (editorPlayButtonPressed)
				{
					ImGui::SetWindowFocus();
				}

				m_ViewportFocused = (ImGui::IsWindowFocused() || m_SceneState == SceneState::Play);
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				uint64_t textureID = m_pFrameBuffer->GetColourAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
			}

			// Gizmos
			Object* pSelectedObject = m_SceneHierarchyPanel.GetSelectedObject();
			/*if (ImGui::ImageButton((ImTextureID)(UINT_PTR)m_pPlayButtonTexture->GetRendererID(), ImVec2{ 32, 32 }))
			{
				if (selectedEntity)
				{

				}
			}*/

			if (pSelectedObject)
			{
				//ImGuizmo::SetOrthographic(false);
				//ImGuizmo::SetDrawlist();
				//float windowWidth = (float)ImGui::GetWindowWidth();
				//float windowHeight = (float)ImGui::GetWindowHeight();
				//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				//if (CameraObject* pCameraObject = m_pEditorScene->GetCurrentCamera())
				//{
				//	const SceneCamera& camera = pCameraObject->GetComponent<CameraComponent>()->m_Camera;
				//	const glm::mat4& cameraProjection = camera.GetProjection();
				//	const TransformComponent* pCameraTransform = pCameraObject->GetComponent<TransformComponent>();
				//	glm::mat4 cameraView = glm::inverse(pCameraTransform->GetTransform());

				//	TransformComponent* pSelectedTransformComponent = pSelectedObject->GetComponent<TransformComponent>();
				//	glm::mat4 selectedTransform = pSelectedTransformComponent->GetTransform();

				//	// Snapping
				//	bool snap = Input::IsKeyPressed(Key::LeftControl);
				//	float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				//	// Snap to 45 degrees for rotation
				//	if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				//		snapValue = 45.0f;

				//	float snapValues[3] = { snapValue, snapValue, snapValue };

				//	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				//		(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(selectedTransform),
				//		nullptr, snap ? snapValues : nullptr);

				//	if (ImGuizmo::IsUsing())
				//	{
				//		pSelectedTransformComponent->m_Position = glm::vec3(selectedTransform[3]);
				//	}
				//}
			}

			ImGui::End();
			ImGui::PopStyleVar();

			//static bool show = true;
			//ImGui::ShowDemoWindow(&show);

			ImGui::Begin("Log");
			{
				if (ImGui::BeginPopupContextWindow(0, 1, false))
				{
					if (ImGui::MenuItem("Clear Log"))
					{
						Log::Clear();
					}

					ImGui::EndPopup();
				}

				ImGui::Text(Log::GetString().c_str());
			}
			ImGui::End();
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_pEditorScene->OnEvent(event);

		EventDispatcher eventDispatcher(event);
		eventDispatcher.Dispatch<KeyPressedEvent>(FD_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::CreateEditorCamera()
	{
		// Clean up editor camera before trying to create again
		if (m_pEditorCamera)
		{
			m_pEditorCamera->End();
			delete m_pEditorCamera;
			m_pEditorCamera = nullptr;
		}

		// Setup our new editor camera based on the current editor scene.
		m_pEditorCamera = new EditorCamera();

		m_pEditorCamera->AddComponent<TagComponent>("Editor Camera");
		IDComponent* pIDComponent = m_pEditorCamera->AddComponent<IDComponent>();
		pIDComponent->m_UUID = {};
		m_pEditorCamera->AddComponent<TransformComponent>(glm::vec3(0.0f));
		m_pEditorCamera->SetOwner(m_pEditorScene.get());
		m_pEditorCamera->Start();

		CameraComponent* pCameraComponent = m_pEditorCamera->GetComponent<CameraComponent>();
		SceneCamera& sceneCamera = pCameraComponent->m_Camera;
		sceneCamera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
		sceneCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		m_pEditorScene->SetCurrentCamera(m_pEditorCamera);
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.GetRepeatCount() > 0)
		{
			return false;
		}

		bool controlPressed = (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl));
		bool shiftPressed = (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift));

		switch (event.GetKeyCode())
		{
			case Key::N:
			{
				if (controlPressed)
				{
					NewScene();
				}
				break;
			}
			case Key::O:
			{
				if (controlPressed)
				{
					OpenScene();
				}
				break;
			}
			case Key::S:
			{
				if (controlPressed && shiftPressed)
				{
					SaveSceneAs();
				}
				break;
			}

			// Gizmos
			case Key::Q:
			{
				m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				Object* pSelectedObject = m_SceneHierarchyPanel.GetSelectedObject();
				if (pSelectedObject)
				{
					//m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				}
				break;
			}
			case Key::E:
			{
				Object* pSelectedObject = m_SceneHierarchyPanel.GetSelectedObject();
				if (pSelectedObject)
				{
					//m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				}
				break;
			}
			case Key::R:
			{
				Object* pSelectedObject = m_SceneHierarchyPanel.GetSelectedObject();
				if (pSelectedObject)
				{
					//m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		if (m_SceneState == SceneState::Play)
		{
			FD_CORE_LOG_WARN("Cannot create new scene whilst playing the current scene, press the stop button first.");
			return;
		}

		m_pEditorScene->End();
		m_pEditorScene = CreateSharedPtr<Scene>();
		m_pEditorScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetScene(m_pEditorScene);
		m_pEditorScene->Start();
		CreateEditorCamera();
	}

	void EditorLayer::OpenScene()
	{
		if (m_SceneState == SceneState::Play)
		{
			FD_CORE_LOG_WARN("Cannot open a new scene whilst playing the current scene, press the stop button first.");
			return;
		}

		std::optional<std::string> filepath = FileDialog::OpenFile("Foundation Scene (*.flevel)\0*.flevel\0");
		if (filepath)
		{
			// Destroy old scene.
			m_pEditorScene->End();
			m_pEditorScene = nullptr;

			// Setup new scene.
			m_pEditorScene = CreateSharedPtr<Scene>();
			//m_pEditorScene->Start();

			// Open scene.
			SceneSerializer serializer(m_pEditorScene);
			serializer.Deserialize(*filepath);
			m_pEditorScene->Start();
			m_pEditorScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetScene(m_pEditorScene);

			CreateEditorCamera();
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		if (m_SceneState == SceneState::Play)
		{
			FD_CORE_LOG_WARN("Cannot save the current scene whilst it's playing, press the stop button first.");
			return;
		}

		std::optional<std::string> filepath = FileDialog::SaveFile("Foundation Scene (*.flevel)\0*.flevel\0");
		if (filepath)
		{
			std::filesystem::path path{ *filepath };
			const std::string& sceneName = path.stem().string();
			m_pEditorScene->m_Name = sceneName;

			SceneSerializer serializer(m_pEditorScene);
			serializer.Serialize(*filepath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
		m_pRuntimeScene = CreateSharedPtr<Scene>();
		m_pRuntimeScene->Copy(m_pEditorScene);
		m_pRuntimeScene->Start();
		m_SceneHierarchyPanel.SetScene(m_pRuntimeScene);
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
		m_SceneHierarchyPanel.SetScene(m_pEditorScene);
		m_pRuntimeScene->End();
		m_pRuntimeScene = nullptr;
	}
}