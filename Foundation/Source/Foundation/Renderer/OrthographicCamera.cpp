#include "fdpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Foundation
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane /* = -1.0f */, float farPlane /* = 1.0f */) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, nearPlane, farPlane)),
		m_ViewMatrix(1.0f),
		m_ViewProjectionMatrix(1.0f),
		m_Bounds({ left, right, bottom, top }),
		m_Position({ 0.0f, 0.0f, 0.0f }),
		m_Rotation(0.0f)
	{
		FD_PROFILE_FUNCTION();

		// OpenGL is column major, so is projection * view, not view * projection.
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float nearPlane /* = -1.0f */, float farPlane /* = 1.0f */)
	{
		FD_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		CalculateViewMatrix();
	}

	const glm::vec3& OrthographicCamera::GetPosition() const
	{
		return m_Position;
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		CalculateViewMatrix();
	}

	const float& OrthographicCamera::GetRotation() const
	{
		return m_Rotation;
	}

	const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	const glm::mat4& OrthographicCamera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const glm::mat4& OrthographicCamera::GetViewProjectionMatrix() const
	{
		return m_ViewProjectionMatrix;
	}

	void OrthographicCamera::CalculateViewMatrix()
	{
		FD_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * // Translate
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));	// Rotate around Z axis for 2D

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}