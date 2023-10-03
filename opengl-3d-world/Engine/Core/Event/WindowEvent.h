#pragma once
#include <sstream>
#include "Event.h"
#include "../Window/Window.h"

namespace Engine {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const int width, const int height) : m_width(width), m_height(height) {}

		[[nodiscard]] int getWidth() const { return m_width; }
		[[nodiscard]] int getHeight() const { return m_height; }

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_NAME(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int m_width, m_height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_NAME(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};


	inline void setWindowSizeCallbackFunction(GLFWwindow* window, const int width, const int height)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		data.width = width;
		data.height = height;
		WindowResizeEvent event(width, height);
		data.event_callback(event);
	}

	inline void setWindowCloseCallbackFunctions(GLFWwindow* window)
	{
		const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event;
		data.event_callback(event);
	}

}