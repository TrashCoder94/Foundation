#include "fdpch.h"
#include "Foundation/Core/Input.h"
#include "Foundation/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Foundation
{
	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		Window& window = Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		int state = glfwGetKey(glfwWindow, static_cast<int32_t>(keycode));
		return ((state == GLFW_PRESS) || (state == GLFW_REPEAT));
	}

	bool Input::IsKeyReleased(const KeyCode keycode)
	{
		Window& window = Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		int state = glfwGetKey(glfwWindow, static_cast<int32_t>(keycode));
		return (state == GLFW_RELEASE);
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		Window& window = Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		int state = glfwGetMouseButton(glfwWindow, static_cast<int32_t>(button));
		return ((state == GLFW_PRESS) || (state == GLFW_REPEAT));
	}

	bool Input::IsMouseButtonReleased(const MouseCode button)
	{
		Window& window = Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		int state = glfwGetMouseButton(glfwWindow, static_cast<int32_t>(button));
		return (state == GLFW_RELEASE);
	}

	glm::vec2 Input::GetMousePosition()
	{
		Window& window = Application::Get().GetWindow();
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		double mouseX, mouseY;
		glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
		return { (float)mouseX, (float)mouseY };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}
