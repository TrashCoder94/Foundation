#include "fdpch.h"
#include "InputComponent.h"
#include "Foundation/Events/Event.h"
#include "Foundation/Events/KeyEvent.h"
#include "Foundation/Events/MouseEvent.h"

namespace Foundation
{
	InputComponent::InputComponent() : Component()
	{}

	InputComponent::~InputComponent()
	{}

	template<typename T>
	bool InputComponent::BindInputFunction(InputEventFunc<T> func)
	{
		FD_CORE_ASSERT(false, "Please implement the template function for whatever input event you are using");
		return false;
	}

	template<>
	bool InputComponent::BindInputFunction<KeyPressedEvent>(InputEventFunc<KeyPressedEvent> func)
	{
		m_KeyPressedFunctions.push_back(func);
		return true;
	}

	template<>
	bool InputComponent::BindInputFunction<MouseMovedEvent>(InputEventFunc<MouseMovedEvent> func)
	{
		m_MouseMovedFunctions.push_back(func);
		return true;
	}

	template<>
	bool InputComponent::BindInputFunction<MouseScrolledEvent>(InputEventFunc<MouseScrolledEvent> func)
	{
		m_MouseScrolledFunctions.push_back(func);
		return true;
	}

	template<>
	bool InputComponent::BindInputFunction<MouseButtonPressedEvent>(InputEventFunc<MouseButtonPressedEvent> func)
	{
		m_MouseButtonFunctions.push_back(func);
		return true;
	}

	void InputComponent::OnEvent(Event& event)
	{
		Component::OnEvent(event);

		FD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(FD_BIND_EVENT_FN(InputComponent::OnKeyPressed));
		dispatcher.Dispatch<MouseMovedEvent>(FD_BIND_EVENT_FN(InputComponent::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(FD_BIND_EVENT_FN(InputComponent::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(FD_BIND_EVENT_FN(InputComponent::OnMouseButtonPressed));
	}

	bool InputComponent::OnKeyPressed(KeyPressedEvent& event)
	{
		for (const std::function<bool(KeyPressedEvent&)>& func : m_KeyPressedFunctions)
			func(event);

		return false;
	}

	bool InputComponent::OnMouseMoved(MouseMovedEvent& event)
	{
		for (const std::function<bool(MouseMovedEvent&)>& func : m_MouseMovedFunctions)
			func(event);

		return false;
	}

	bool InputComponent::OnMouseScrolled(MouseScrolledEvent& event)
	{
		for (const std::function<bool(MouseScrolledEvent&)>& func : m_MouseScrolledFunctions)
			func(event);

		return false;
	}

	bool InputComponent::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		for (const std::function<bool(MouseButtonPressedEvent&)>& func : m_MouseButtonFunctions)
			func(event);

		return false;
	}
}