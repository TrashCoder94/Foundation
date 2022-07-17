#pragma once

#include <Foundation.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Foundation
{
	class EditorCamera;
	class Object;

	class EditorLayer : public Layer
	{
		public:
			EditorLayer();
			~EditorLayer();

			void OnAttach() override;
			void OnDetach() override;
			void OnUpdate(Timestep ts) override;
			void OnRender() override;
			void OnImGuiRender() override;
			void OnEvent(Event& event) override;

		private:
			void CreateEditorCamera();

			bool OnKeyPressed(KeyPressedEvent& event);

			void NewScene();
			void OpenScene();
			void SaveSceneAs();

			void OnScenePlay();
			void OnSceneStop();

		private:
			enum class SceneState
			{
				Edit = 0,
				Play,
				Pause
			};
			SceneState m_SceneState;

			EditorCamera* m_pEditorCamera;
			SharedPtr<FrameBuffer> m_pFrameBuffer;
			SharedPtr<Scene> m_pEditorScene;
			SharedPtr<Scene> m_pRuntimeScene;
			SharedPtr<Texture2D> m_pPlayButtonTexture;
			SharedPtr<Texture2D> m_pStopButtonTexture;
			SharedPtr<Texture2D> m_pTranslateGizmoTexture;
			SharedPtr<Texture2D> m_pRotateGizmoTexture;
			SharedPtr<Texture2D> m_pScaleGizmoTexture;
			Object* m_pSquareObject;
			Object* m_pMainCameraObject;
			Object* m_pSecondCameraObject;
			int m_GizmoType;
			std::string m_CurrentSceneFilepath;

			bool m_PrimaryCamera;

			glm::vec2 m_ViewportSize;
			bool m_ViewportFocused;
			bool m_ViewportHovered;

			// Panels
			SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}