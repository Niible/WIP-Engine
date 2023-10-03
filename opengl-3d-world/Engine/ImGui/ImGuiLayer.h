#pragma once
#include "../Core/Event/Event.h"
#include "../Core/Layer/Layer.h"

namespace Engine
{
		
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(const bool block) { m_blockEvents = block; }

		void setDarkThemeColors();

		uint32_t getActiveWidgetId() const;
	private:
		bool m_blockEvents = true;
		bool m_show_demo = true;
	};

}
