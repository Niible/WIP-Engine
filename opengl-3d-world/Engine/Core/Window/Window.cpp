#include "Window.h"
#include "../Event/WindowEvent.h"
#include "../Event/MouseEvent.h"
#include "../Event/KeyEvent.h"

#include <stdexcept>

using namespace Engine;

Window::Window(const int width, const int height, const char* title) : m_data(width, height, title)
{
    init();
}

Window::Window(WindowData data) : m_data(std::move(data))
{
    init();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::init()
{
    if (!glfwInit()) {
        throw std::runtime_error("glfw Init failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, NULL, NULL);

    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("create window failed");
    }

    glfwMakeContextCurrent(m_window);

    setVSync(m_data.v_sync);
    glfwSetWindowUserPointer(m_window, &m_data);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_window, setWindowSizeCallbackFunction);
    glfwSetWindowCloseCallback(m_window, setWindowCloseCallbackFunctions);
    glfwSetKeyCallback(m_window, setKeyCallbackFunction);
    glfwSetMouseButtonCallback(m_window, setMouseButtonCallbackFunction);
    glfwSetCursorPosCallback(m_window, setCursorPosCallbackFunction);
    glfwSetScrollCallback(m_window, setScrollCallbackFunction);

    // fucking lines of hell
    glewExperimental = GL_TRUE;
    if (glewInit())
        throw std::runtime_error("Error init glew");
}

void Window::onUpdate() const
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void Window::setVSync(const bool enabled)
{
    if (enabled) {
        glfwSwapInterval(1);
    }
    else {
        glfwSwapInterval(0);
    }
    m_data.v_sync = enabled;
}

bool Window::isKeyPressed(const KeyCode key) const
{
	const auto state = glfwGetKey(m_window, key);
    return state == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(const MouseCode button) const
{
	const auto state = glfwGetMouseButton(m_window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Window::getMousePosition() const
{
    double x_pos, y_pos;
    glfwGetCursorPos(m_window, &x_pos, &y_pos);

    return { static_cast<float>(x_pos), static_cast<float>(y_pos) };
}

float Window::getMouseX() const
{
    return getMousePosition().x;
}

float Window::getMouseY() const
{
    return getMousePosition().y;
}
