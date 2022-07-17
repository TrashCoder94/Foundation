#include "fdpch.h"
#include "InputComponent.h"
#include "Foundation/Events/Event.h"
#include "Foundation/Events/KeyEvent.h"
#include "Foundation/Events/MouseEvent.h"

namespace Foundation
{
	InputComponent::InputComponent() : Component(),
		m_UUID(),
		m_InputEnabled(true)
	{}

	InputComponent::~InputComponent()
	{}

	template<typename T>
	bool InputComponent::BindInputFunction(BaseObject* pFunctionOwner, InputEventFunc<T> func)
	{
		FD_CORE_ASSERT(false, "Please implement the template function for whatever input event you are using");
		return false;
	}

	template<>
	bool InputComponent::BindInputFunction<KeyPressedEvent>(BaseObject* pFunctionOwner, InputEventFunc<KeyPressedEvent> func)
	{
		if (m_KeyPressedFunctions.find(pFunctionOwner) != m_KeyPressedFunctions.end())
		{
			return false;
		}

		m_KeyPressedFunctions.emplace(pFunctionOwner, func);
		return true;
	}

	template<>
	bool InputComponent::BindInputFunction<MouseMovedEvent>(BaseObject* pFunctionOwner, InputEventFunc<MouseMovedEvent> func)
	{
		if (m_MouseMovedFunctions.find(pFunctionOwner) != m_MouseMovedFunctions.end())
		{
			return false;
		}

		m_MouseMovedFunctions.emplace(pFunctionOwner, func);
		return true;
	}

	template<>
	bool InputComponent::BindInputFunction<MouseScrolledEvent>(BaseObject* pFunctionOwner, InputEventFunc<MouseScrolledEvent> func)
	{
		if (m_MouseScrolledFunctions.find(pFunctionOwner) != m_MouseScrolledFunctions.end())
		{
			return false;
		}

		m_MouseScrolledFunctions.emplace(pFunctionOwner, func);
		return true;
	}

	template<>
	bool InputComponent::BindInputFunction<MouseButtonPressedEvent>(BaseObject* pFunctionOwner, InputEventFunc<MouseButtonPressedEvent> func)
	{
		if (m_MouseButtonFunctions.find(pFunctionOwner) != m_MouseButtonFunctions.end())
		{
			return false;
		}

		m_MouseButtonFunctions.emplace(pFunctionOwner, func);
		return true;
	}

	void InputComponent::EnableInput()
	{
		m_InputEnabled = true;
	}

	void InputComponent::DisableInput()
	{
		m_InputEnabled = false;
	}

	void InputComponent::OnEvent(Event& event)
	{
		Component::OnEvent(event);

		FD_PROFILE_FUNCTION();

		if (m_InputEnabled)
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<KeyPressedEvent>(FD_BIND_EVENT_FN(InputComponent::OnKeyPressed));
			dispatcher.Dispatch<MouseMovedEvent>(FD_BIND_EVENT_FN(InputComponent::OnMouseMoved));
			dispatcher.Dispatch<MouseScrolledEvent>(FD_BIND_EVENT_FN(InputComponent::OnMouseScrolled));
			dispatcher.Dispatch<MouseButtonPressedEvent>(FD_BIND_EVENT_FN(InputComponent::OnMouseButtonPressed));
		}
	}

	bool InputComponent::OnKeyPressed(KeyPressedEvent& event)
	{
		for (const std::pair<BaseObject*, std::function<bool(KeyPressedEvent&)>> funcMap : m_KeyPressedFunctions)
			funcMap.second(event);

		return false;
	}

	bool InputComponent::OnMouseMoved(MouseMovedEvent& event)
	{
		for (const std::pair<BaseObject*, std::function<bool(MouseMovedEvent&)>> funcMap : m_MouseMovedFunctions)
			funcMap.second(event);

		return false;
	}

	bool InputComponent::OnMouseScrolled(MouseScrolledEvent& event)
	{
		for (const std::pair<BaseObject*, std::function<bool(MouseScrolledEvent&)>> funcMap : m_MouseScrolledFunctions)
			funcMap.second(event);

		return false;
	}

	bool InputComponent::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		for (const std::pair<BaseObject*, std::function<bool(MouseButtonPressedEvent&)>> funcMap : m_MouseButtonFunctions)
			funcMap.second(event);
		
		return false;
	}
}