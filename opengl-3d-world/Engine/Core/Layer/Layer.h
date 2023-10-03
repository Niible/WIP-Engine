#pragma once
#include <string>

#include "../Event/Event.h"

namespace Engine
{
	
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_debugName(name) {}
		virtual ~Layer() = default;

		virtual void onAttach() {}

		virtual void onDetach() {}

		virtual void onUpdate(float dt) {}

		virtual void onEvent(Event& event) {}

		virtual void onDataEvent(Event& event) {}

		const std::string& getName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};

}
