#pragma once

#include "Component.h"
#include "Foundation/Events/Event.h"

namespace Foundation
{
	class Event;
	class KeyPressedEvent;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class MouseButtonPressedEvent;

	class InputComponent : public Component
	{
		FD_REFLECT()

		template<typename T>
		using InputEventFunc = std::function<bool(T&)>;

	public:
		InputComponent();
		explicit InputComponent(const InputComponent&) = default;
		~InputComponent();

		template<typename T>
		bool BindInputFunction(InputEventFunc<T> func);

		virtual void OnEvent(Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		std::vector<std::function<bool(KeyPressedEvent&)>>			m_KeyPressedFunctions;
		std::vector<std::function<bool(MouseMovedEvent&)>>			m_MouseMovedFunctions;
		std::vector<std::function<bool(MouseScrolledEvent&)>>		m_MouseScrolledFunctions;
		std::vector<std::function<bool(MouseButtonPressedEvent&)>>	m_MouseButtonFunctions;
	};
}