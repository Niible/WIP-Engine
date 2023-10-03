#pragma once
#include <iostream>
#include <gl/glew.h>


namespace Engine {
	class IndexBuffer
	{
	public:
		IndexBuffer() = delete;
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(const uint32_t* indices, const uint32_t count) : m_count(count)
		{
			load(indices, count);
		}

		void load(const uint32_t* indices, const uint32_t count)
		{
			glCreateBuffers(1, &m_rendererId);
			// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
			// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
			glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		~IndexBuffer()
		{
			glDeleteBuffers(1, &m_rendererId);
		}

		void bind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
		}

		void unbind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void setData(const uint32_t indices[], const uint32_t count)
		{
			m_count = count;
			glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		uint32_t getCount() const { return m_count; }

	private:
		uint32_t m_rendererId;
		uint32_t m_count;
	};

}