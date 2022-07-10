#pragma once

#include <glm/glm.hpp>
#include "Foundation/Core/KeyCodes.h"
#include "Foundation/Core/MouseCodes.h"

namespace Foundation
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);
		
		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseButtonReleased(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
