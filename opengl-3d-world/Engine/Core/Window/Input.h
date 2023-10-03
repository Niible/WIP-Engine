#pragma once

#include "../Application/Application.h"
#include "../Event/KeyEvent.h"

namespace Engine {

	class Input
	{
	public:
		static bool isKeyPressed(const KeyCode key) {
			auto* window = Application::get().getWindow().getNativeWindow();
			const auto state = glfwGetKey(window, key);
			return state == GLFW_PRESS;
		}

		static bool isMouseButtonPressed(const MouseCode button)
		{
			auto* window = Application::get().getWindow().getNativeWindow();
			const auto state = glfwGetMouseButton(window, button);
			return state == GLFW_PRESS;
		}

		static glm::vec2 getMousePosition()
		{
			auto* window = Application::get().getWindow().getNativeWindow();
			double x_pos, y_pos;
			glfwGetCursorPos(window, &x_pos, &y_pos);

			return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
		}

		static float getMouseX()
		{
			return getMousePosition().x;
		}

		static float getMouseY()
		{
			return getMousePosition().y;
		}
	};

}