#pragma once
#include <glm/glm.hpp>
#include "../Buffer/VertexArray.h"

namespace Engine {

	class RendererAPI
	{
	public:
		RendererAPI()
		{
			init();
		}
		~RendererAPI() = default;

		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void setClearColor(const glm::vec4& color);
		void clear();
		void drawIndexed(const VertexArray& vertex_array, uint32_t index_count = 0);

		static RendererAPI* get()
		{
			if (!s_instance)
				s_instance = new RendererAPI();
			return s_instance;
		}

	private:
		void init();

		static RendererAPI* s_instance;
	};

}