#include "fdpch.h"
#include "CameraObject.h"

#include "Foundation/Components/CameraComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Events/ApplicationEvent.h"

namespace Foundation
{
	CameraObject::CameraObject() : Object(),
		m_pCameraComponent(nullptr),
		m_pTransformComponent(nullptr)
	{}

	CameraObject::CameraObject(Scene* pScene) : Object(pScene),
		m_pCameraComponent(nullptr),
		m_pTransformComponent(nullptr)
	{}

	CameraObject::~CameraObject()
	{
		m_pCameraComponent = nullptr;
		m_pTransformComponent = nullptr;
	}

	void CameraObject::Start()
	{
		Object::Start();

		m_pCameraComponent = AddComponent<CameraComponent>();
		m_pTransformComponent = GetComponent<TransformComponent>();
	}

	void CameraObject::Update(float deltaTime)
	{
		Object::Update(deltaTime);
	}

	void CameraObject::End()
	{
		Object::End();
	}

	void CameraObject::OnEvent(Event& e)
	{
		Object::OnEvent(e);

		FD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(FD_BIND_EVENT_FN(CameraObject::OnWindowResized));
	}

	void CameraObject::OnResize(float width, float height)
	{
		m_pCameraComponent->m_Camera.SetViewportSize((uint32_t)width, (uint32_t)height);
	}

	bool CameraObject::OnWindowResized(WindowResizeEvent& event)
	{
		FD_PROFILE_FUNCTION();

		OnResize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}
}