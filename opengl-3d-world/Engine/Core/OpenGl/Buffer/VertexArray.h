#pragma once
#include <memory>
#include <vector>
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "../../../Utils/Factory.h"

namespace Engine {
	static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		throw std::runtime_error("Unknown ShaderDataType!");
	}

	class VertexArray
	{
	public:
		VertexArray() : m_id(0)
		{
			glCreateVertexArrays(1, &m_id);
		}

		//VertexArray(const VertexArray& vertex_array) : m_id(vertex_array.m_id), m_vertexBufferIndex(vertex_array.m_vertexBufferIndex), m_vertexBuffers(vertex_array.getVertexBuffers()), m_indexBuffer(vertex_array.m_indexBuffer)
		//{
		//	glCreateVertexArrays(1, &m_id);
		//}

		~VertexArray()
		{
			glDeleteVertexArrays(1, &m_id);
		}

		void bind() const
		{
			glBindVertexArray(m_id);
		}

		void unbind() const
		{
			glBindVertexArray(0);
		}

		void addVertexBuffer(float* vertices, uint32_t size, const BufferLayout& layout)
		{
			m_vertexBuffers.emplace_back(vertices, size);
			auto& vertex_buffer = m_vertexBuffers[m_vertexBuffers.size() - 1];
			vertex_buffer.setLayout(layout);
			glBindVertexArray(m_id);
			vertex_buffer.bind();

			for (const auto& element : layout)
			{
				switch (element.type)
				{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_vertexBufferIndex);
					glVertexAttribPointer(m_vertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)element.offset);
					m_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_vertexBufferIndex);
					glVertexAttribIPointer(m_vertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						layout.getStride(),
						(const void*)element.offset);
					m_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_vertexBufferIndex);
						glVertexAttribPointer(m_vertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element.type),
							element.normalized ? GL_TRUE : GL_FALSE,
							layout.getStride(),
							(const void*)(element.offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_vertexBufferIndex, 1);
						m_vertexBufferIndex++;
					}
					break;
				}
				default:
					throw std::runtime_error("Unknown ShaderDataType!");
				}
			}

		}

		void setIndexBuffer(const uint32_t* indices, const uint32_t count) {

			m_indexBuffer = createRef<IndexBuffer>(indices, count);
			glBindVertexArray(m_id);
			m_indexBuffer->bind();
		}

		const std::vector<VertexBuffer>& getVertexBuffers() const { return m_vertexBuffers; }
		const Ref<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }

	private:
		GLuint m_id;
		uint32_t m_vertexBufferIndex = 0;
		std::vector<VertexBuffer> m_vertexBuffers;
		Ref<IndexBuffer> m_indexBuffer;

	};

}