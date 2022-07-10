#include "fdpch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Foundation
{
	SceneCamera::SceneCamera() : Camera(),
		m_ProjectionType(ProjectionType::Perspective),
		m_PerspectiveVerticalFOV(glm::radians(45.0f)),
		m_PerspectiveNear(0.01f),
		m_PerspectiveFar(10000.0f),
		m_OrthographicSize(10.0f),
		m_OrthographicNear(-1.0f),
		m_OrthographicFar(1.0f),
		m_AspectRatio(1.0f)
	{
		CalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (width < 1.0f || height < 1.0f) ? 1.0f : (float)width / (float)height;
		CalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveVerticalFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		CalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		CalculateProjection();
	}

	void SceneCamera::CalculateProjection()
	{
		switch (m_ProjectionType)
		{
			case ProjectionType::Perspective:
			{
				m_Projection = glm::perspective(m_PerspectiveVerticalFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
				break;
			}
			case ProjectionType::Orthographic:
			{
				float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
				float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
				float orthoBottom = -m_OrthographicSize * 0.5f;
				float orthoTop = m_OrthographicSize * 0.5f;

				m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);

				break;
			}
		}
		
	}
}