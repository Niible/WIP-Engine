#include "Application.h"
#include "../Event/WindowEvent.h"
#include "../OpenGl/Renderer/Renderer.h"
#include "../Event/KeyEvent.h"
#include "../../Debug/Instrumentor.h"

using namespace Engine;
	
Application* Application::s_instance = nullptr;

Application::Application() : m_window ( Window(1280, 720, "OpenGl") )
{
	PROFILE_FUNC()
	if (s_instance)
	{
		throw std::runtime_error("Application already exists");
	}
	s_instance = this;

	m_window.setEventCallback([&](Event& e) { return Application::onEvent(e); });
}


void Application::run()
{
	while (m_isRunning)
	{
		PROFILE_FRAME("MainThread")

		const auto time = static_cast<float>(glfwGetTime());
		const float dt = time - m_lastFrameTime;
		m_lastFrameTime = time;

		for (Layer* layer : m_layerStack)
			layer->onUpdate(dt);

		m_window.onUpdate();
	}
}

void Application::pushLayer(Layer* layer)
{
	PROFILE_FUNC()

	m_layerStack.pushLayer(layer);
	layer->onAttach();
}

void Application::pushOverlay(Layer* layer)
{
	PROFILE_FUNC()

	m_layerStack.pushOverlay(layer);
	layer->onAttach();
}

void Application::onEvent(Event& e)
{
	PROFILE_FUNC();

	EventDispatcher dispatcher(e);

	dispatcher.dispatch<WindowResizeEvent>([](const WindowResizeEvent& event)
	{
		glViewport(0, 0, event.getWidth(), event.getHeight());
		return false;
	});

	dispatcher.dispatch<WindowCloseEvent>([this](const WindowCloseEvent& event)
	{
		m_isRunning = false;
		return true;
	});

	dispatcher.dispatch<KeyPressedEvent>([this](const KeyPressedEvent& event)
	{
		if (event.getKeyCode() == GLFW_KEY_ESCAPE) {
			m_isRunning = false;
			return true;
		}
		return false;
	});

	for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
	{
		(*--it)->onEvent(e);
		if (e.handled)
			break;
	}
}


Application& Application::get()
{
	return *s_instance;
}
