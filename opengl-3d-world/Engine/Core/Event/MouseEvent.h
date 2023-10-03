#pragma once
#include <sstream>

#include "Event.h"
#include "../Window/Window.h"

namespace Engine
{
	

enum MouseCode
{
	// From glfw3.h
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x, const float y) : m_mouseX(x), m_mouseY(y) {}

	[[nodiscard]] float getX() const { return m_mouseX; }
	[[nodiscard]] float getY() const { return m_mouseY; }

	[[nodiscard]] std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_NAME(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset)
		: m_xOffset(xOffset), m_yOffset(yOffset) {}

	[[nodiscard]] float getXOffset() const { return m_xOffset; }
	[[nodiscard]] float getYOffset() const { return m_yOffset; }

	[[nodiscard]] std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_NAME(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_xOffset, m_yOffset;
};

class MouseButtonEvent : public Event
{
public:
	[[nodiscard]] MouseCode getMouseButton() const { return m_button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
protected:
	explicit MouseButtonEvent(const MouseCode button)
		: m_button(button) {}

	MouseCode m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	explicit MouseButtonPressedEvent(const MouseCode button)
		: MouseButtonEvent(button) {}

	[[nodiscard]] std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
	EVENT_CLASS_NAME(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	explicit MouseButtonReleasedEvent(const MouseCode button)
		: MouseButtonEvent(button) {}

	[[nodiscard]] std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
	EVENT_CLASS_NAME(MouseButtonReleased)

};

inline void setMouseButtonCallbackFunction(GLFWwindow* window, int button, const int action, int mods)
{
	const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
	switch (action)
	{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(static_cast<MouseCode>(button));
			data.event_callback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
			data.event_callback(event);
			break;
		}
	default: 
		break;
	}
}

inline void setCursorPosCallbackFunction(GLFWwindow* window, const double x_pos, const double y_pos)
{
	const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
	MouseMovedEvent event(static_cast<float>(x_pos), static_cast<float>(y_pos));
	data.event_callback(event);
}

inline void setScrollCallbackFunction(GLFWwindow* window, const double x_offset, const double y_offset)
{
	const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
	MouseScrolledEvent event(static_cast<float>(x_offset), static_cast<float>(y_offset));
	data.event_callback(event);
}

}
