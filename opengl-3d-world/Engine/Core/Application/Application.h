#pragma once
#include "../Window/Window.h"
#include "../Layer/Layer.h"
#include "../Layer/LayerStack.h"

namespace Engine
{
	class Application
	{
	public:
		Application();

		~Application() = default;

		void run();

		void pushLayer(Layer* layer);

		void pushOverlay(Layer* layer);

		void onEvent(Event& e);

		static Application& get();

		Window& getWindow() { return m_window; }

	private:
		static Application* s_instance;

		Window m_window;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
		bool m_isRunning = true;
	};

}

