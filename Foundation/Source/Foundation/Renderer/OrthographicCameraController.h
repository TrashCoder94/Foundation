#pragma once

#include "Foundation/Core/Timestep.h"
#include "Foundation/Events/ApplicationEvent.h"
#include "Foundation/Events/MouseEvent.h"
#include "OrthographicCamera.h"

namespace Foundation
{
	class OrthographicCameraController
	{
		public:
			OrthographicCameraController(float aspectRatio, bool rotation = false);

			void OnUpdate(Timestep ts);
			void OnEvent(Event& e);

			void OnResize(float width, float height);

			OrthographicCamera& GetCamera() { return m_Camera; }
			const OrthographicCamera& GetCamera() const { return m_Camera; }

			const float GetZoomLevel() const { return m_ZoomLevel; }
			void SetZoomLevel(float NewZoomLevel) { m_ZoomLevel = NewZoomLevel; CalculateView(); }

		private:
			bool OnMouseScrolled(MouseScrolledEvent& e);
			bool OnWindowResized(WindowResizeEvent& e);

			void CalculateView();

		private:
			bool m_RotationEnabled;
			float m_AspectRatio;
			float m_ZoomLevel;
			OrthographicCamera m_Camera;

			glm::vec3 m_CameraPosition;
			float m_CameraRotation;
			float m_CameraMoveSpeed;
			float m_CameraRotateSpeed;
	};
}