#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/Events/Event.h"
#include "Foundation/Core/UUID.h"

namespace Foundation
{
	class Event;
	class KeyPressedEvent;
	class KeyReleasedEvent;
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
		bool BindInputFunction(BaseObject* pFunctionOwner, InputEventFunc<T> func);

		void EnableInput();
		void DisableInput();

		virtual void OnEvent(Event& event) override;

		FVARIABLE(VariableFlags::Edit)
			UUID m_UUID;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		std::map<BaseObject*, std::function<bool(KeyPressedEvent&)>>			m_KeyPressedFunctions;
		std::map<BaseObject*, std::function<bool(KeyReleasedEvent&)>>			m_KeyReleasedFunctions;
		std::map<BaseObject*, std::function<bool(MouseMovedEvent&)>>			m_MouseMovedFunctions;
		std::map<BaseObject*, std::function<bool(MouseScrolledEvent&)>>			m_MouseScrolledFunctions;
		std::map<BaseObject*, std::function<bool(MouseButtonPressedEvent&)>>	m_MouseButtonFunctions;

		bool m_InputEnabled;
	};
}