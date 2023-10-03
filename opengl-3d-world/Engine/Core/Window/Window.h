#pragma once
#include <functional>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Event/Event.h"

namespace Engine
{
	

enum KeyCode;
enum MouseCode;

using EventCallbackFn = std::function<void(Event&)>;

struct WindowData
{
	WindowData(const int width, const int height, const char* title): width(width), height(height), title(title) {}

	int width;
	int height;
	const char* title;
	bool v_sync = true;

	EventCallbackFn event_callback;

};

class Window
{
public:
	Window(int width, int height, const char* title);
	explicit Window(WindowData data);
	~Window();

private:
	void init();

public:
	void onUpdate() const;

	void setVSync(bool enabled);
	bool isKeyPressed(KeyCode key) const;
	bool isMouseButtonPressed(MouseCode button) const;
	glm::vec2 getMousePosition() const;
	float getMouseX() const;
	float getMouseY() const;

	void setEventCallback(const EventCallbackFn& callback) { m_data.event_callback = callback; }

	[[nodiscard]] int getWidth() const { return m_data.width; }
	[[nodiscard]] int getHeight() const { return m_data.height; }

	[[nodiscard]] GLFWwindow* getNativeWindow() const { return m_window; }

private:
	GLFWwindow* m_window;
	WindowData m_data;
};

}
