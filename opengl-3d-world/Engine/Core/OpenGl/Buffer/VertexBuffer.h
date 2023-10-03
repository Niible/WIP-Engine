#pragma once
#include <cstdint>
#include <gl/glew.h>
#include "Buffer.h"

namespace Engine
{
	
class VertexBuffer
{
public:
	VertexBuffer() = default;
	explicit VertexBuffer(uint32_t size)
	{
		load(size);
	}

	VertexBuffer(float* vertices, uint32_t size)
	{
        load(size, vertices);
	}

    void load(const uint32_t size, const float* vertices = nullptr)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

	~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setData(const void* data, const uint32_t size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	const BufferLayout& getLayout() const { return m_layout; }

	void setLayout(const BufferLayout& layout) { m_layout = layout; }

private:
	GLuint m_id;
	BufferLayout m_layout;
};

}
