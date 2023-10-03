#pragma once
#include <memory>
#include <GL/glew.h>


namespace Engine
{
	
class UniformBuffer
{
public:
	UniformBuffer(const uint32_t size, const uint32_t binding)
	{
		glCreateBuffers(1, &m_rendererId);
		glNamedBufferData(m_rendererId, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_rendererId);
	}

	~UniformBuffer()
	{
		glDeleteBuffers(1, &m_rendererId);
	}

	void setData(const void* data, const uint32_t size, const uint32_t offset = 0) const
	{
		glNamedBufferSubData(m_rendererId, offset, size, data);
	}

private:
	uint32_t m_rendererId = 0;
};

}
