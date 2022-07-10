#include "fdpch.h"
#include "OrthographicCameraController.h"
#include "Foundation/Core/Input.h"
#include "Foundation/Core/KeyCodes.h"

namespace Foundation
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/) :
		m_RotationEnabled(rotation),
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(1.0f),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_CameraPosition(glm::vec3(0.0f)),
		m_CameraRotation(0.0f),
		m_CameraMoveSpeed(5.0f),
		m_CameraRotateSpeed(180.0f)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		FD_PROFILE_FUNCTION();

		// Camera Movement
		if (Input::IsKeyPressed(Key::Up) || Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.y += (m_CameraMoveSpeed * ts);
		}
		else if (Input::IsKeyPressed(Key::Down) || Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.y -= (m_CameraMoveSpeed * ts);
		}

		if (Input::IsKeyPressed(Key::Left) || Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.x -= (m_CameraMoveSpeed * ts);
		}
		else if (Input::IsKeyPressed(Key::Right) || Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += (m_CameraMoveSpeed * ts);
		}

		// Camera Rotation
		if (m_RotationEnabled)
		{
			if (Input::IsKeyPressed(Key::Q))
			{
				m_CameraRotation += (m_CameraRotateSpeed * ts);
			}
			else if (Input::IsKeyPressed(Key::E))
			{
				m_CameraRotation -= (m_CameraRotateSpeed * ts);
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraMoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		FD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FD_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(FD_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = (width / height);
		CalculateView();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		FD_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		FD_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Camera.m_Bounds.m_Left, m_Camera.m_Bounds.m_Right, m_Camera.m_Bounds.m_Bottom, m_Camera.m_Bounds.m_Top);
	}
}
