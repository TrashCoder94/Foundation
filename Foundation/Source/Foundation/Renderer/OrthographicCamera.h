#pragma once

#include <glm/glm.hpp>

namespace Foundation
{
	struct OrthographicCameraBounds
	{
		float m_Left = 0.0f;
		float m_Right = 0.0f;
		float m_Bottom = 0.0f;
		float m_Top = 0.0f;

		float GetWidth() { return m_Right - m_Left; }
		float GetHeight() { return m_Top - m_Bottom; }
	};

	// 2D Camera
	class OrthographicCamera
	{
		friend class OrthographicCameraController;

		public:
			OrthographicCamera(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

			void SetProjection(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

			void SetPosition(const glm::vec3& position);
			const glm::vec3& GetPosition() const;

			void SetRotation(float rotation);
			const float& GetRotation() const;

			const glm::mat4& GetProjectionMatrix() const;
			const glm::mat4& GetViewMatrix() const;
			const glm::mat4& GetViewProjectionMatrix() const;

			const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

		private:
			void CalculateViewMatrix();

		private:
			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;
			glm::mat4 m_ViewProjectionMatrix;

			OrthographicCameraBounds m_Bounds;

			glm::vec3 m_Position;
			float m_Rotation;	// Only care about Z rotation for orthographic camera.
	};
}