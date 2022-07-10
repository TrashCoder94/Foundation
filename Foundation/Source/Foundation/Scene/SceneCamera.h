#pragma once

#include "Foundation/Renderer/Camera.h"

namespace Foundation
{
	class SceneCamera : public Camera
	{
		public:
			enum class ProjectionType
			{
				Perspective = 0,
				Orthographic
			};

			SceneCamera();
			~SceneCamera() = default;

			ProjectionType GetProjectionType() const { return m_ProjectionType; }
			void SetProjectionType(const ProjectionType projectionType) { m_ProjectionType = projectionType; CalculateProjection(); }

			void SetViewportSize(uint32_t width, uint32_t height);

			// =============================================================================
			// Perspective

			void SetPerspective(float verticalFOV, float nearClip, float farClip);

			float GetPerspectiveVerticalFOV() const { return m_PerspectiveVerticalFOV; }
			void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveVerticalFOV = verticalFOV; }

			float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
			void SetPerspectiveNearClip(const float nearClip) { m_PerspectiveNear = nearClip; }

			float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
			void SetPerspectiveFarClip(const float farClip) { m_PerspectiveFar = farClip; }

			// =============================================================================
			// Orthographic

			void SetOrthographic(float size, float nearClip, float farClip);

			float GetOrthographicSize() const { return m_OrthographicSize; }
			void SetOrthographicSize(float size) { m_OrthographicSize = size; CalculateProjection(); }

			float GetOrthographicNearClip() const { return m_OrthographicNear; }
			void SetOrthographicNearClip(const float nearClip) { m_OrthographicNear = nearClip; CalculateProjection(); }

			float GetOrthographicFarClip() const { return m_OrthographicFar; }
			void SetOrthographicFarClip(const float farClip) { m_OrthographicFar = farClip; CalculateProjection(); }

			// =============================================================================

		private:
			void CalculateProjection();

		private:
			ProjectionType m_ProjectionType;

			float m_PerspectiveVerticalFOV;
			float m_PerspectiveNear;
			float m_PerspectiveFar;

			float m_OrthographicSize;
			float m_OrthographicNear;
			float m_OrthographicFar;

			float m_AspectRatio;
	};
}