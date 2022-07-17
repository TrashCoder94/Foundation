#include "EditorCamera.h"

#include <Foundation/Components/CameraComponent.h>
#include <Foundation/Components/InputComponent.h>
#include <Foundation/Components/TransformComponent.h>
#include <Foundation/Events/KeyEvent.h>
#include <Foundation/Events/MouseEvent.h>
#include <Foundation/Objects/CameraObject.h>
#include <Foundation/Scene/Scene.h>

namespace Foundation
{
	EditorCamera::EditorCamera() : CameraObject(),
		m_pInputComponent(nullptr)
	{}

	EditorCamera::EditorCamera(Scene* pScene) : CameraObject(pScene),
		m_pInputComponent(nullptr)
	{}

	EditorCamera::~EditorCamera()
	{}

	void EditorCamera::Create()
	{
		CameraObject::Create();

		m_pCameraComponent->m_Primary = true;

		m_pInputComponent = AddComponent<InputComponent>();
		m_pInputComponent->BindInputFunction<KeyPressedEvent>(this, FD_BIND_EVENT_FN(EditorCamera::OnKeyPressed));
		m_pInputComponent->BindInputFunction<MouseMovedEvent>(this, FD_BIND_EVENT_FN(EditorCamera::OnMouseMoved));
		m_pInputComponent->BindInputFunction<MouseScrolledEvent>(this, FD_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
		m_pInputComponent->BindInputFunction<MouseButtonPressedEvent>(this, FD_BIND_EVENT_FN(EditorCamera::OnMouseButtonPressed));
	}

	void EditorCamera::Start()
	{
		CameraObject::Start();
	}

	void EditorCamera::Update(float deltaTime)
	{
		CameraObject::Update(deltaTime);
	}

	void EditorCamera::End()
	{
		CameraObject::End();
	}

	bool EditorCamera::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == Key::Up || event.GetKeyCode() == Key::W)
			m_pTransformComponent->m_Position.z -= 0.5f;
		else if (event.GetKeyCode() == Key::Down || event.GetKeyCode() == Key::S)
			m_pTransformComponent->m_Position.z += 0.5f;

		if (event.GetKeyCode() == Key::Left || event.GetKeyCode() == Key::A)
			m_pTransformComponent->m_Position.x -= 0.5f;
		else if (event.GetKeyCode() == Key::Right || event.GetKeyCode() == Key::D)
			m_pTransformComponent->m_Position.x += 0.5f;

		return false;
	}

	bool EditorCamera::OnMouseMoved(MouseMovedEvent& event)
	{
		return false;
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& event)
	{
		return false;
	}

	bool EditorCamera::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		return false;
	}
}