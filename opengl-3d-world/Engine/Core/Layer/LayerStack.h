#pragma once
#include <vector>
#include "Layer.h"

namespace Engine
{

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack()
		{
			for (Layer* layer : m_layers)
			{
				layer->onDetach();
				delete layer;
			}
		}

		void pushLayer(Layer* layer) {
			m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
			m_layerInsertIndex++;
		}

		void pushOverlay(Layer* overlay)
		{
			m_layers.emplace_back(overlay);
		}

		void popLayer(Layer* layer)
		{
			auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
			if (it != m_layers.begin() + m_layerInsertIndex)
			{
				layer->onDetach();
				m_layers.erase(it);
				m_layerInsertIndex--;
			}
		}

		void popOverlay(Layer* overlay)
		{
			auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
			if (it != m_layers.end())
			{
				overlay->onDetach();
				m_layers.erase(it);
			}
		}

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};

}