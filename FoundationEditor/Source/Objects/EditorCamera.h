#pragma once

#include <Foundation/Objects/CameraObject.h>

namespace Foundation
{
	class Scene;
	class InputComponent;
	class KeyPressedEvent;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class MouseButtonPressedEvent;

	class EditorCamera : public CameraObject
	{
	public:
		EditorCamera();
		EditorCamera(Scene* pScene);
		~EditorCamera();

		virtual void Start() override;
		virtual void Update(float deltaTime) override;
		virtual void End() override;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		InputComponent* m_pInputComponent;
	};
}