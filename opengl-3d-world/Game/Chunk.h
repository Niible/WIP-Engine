#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "HeightMap.h"
#include "../Engine/Core/OpenGl/Buffer/VertexArray.h"

using namespace Engine;

class Chunk
{
public:
	Chunk(const HeightMapSettings& height_map_settings, const HeightMapNoiseSettings& height_map_noise_settings): height_map_settings(height_map_settings)
	{
		init(height_map_noise_settings);
	}

private:

	void init(const HeightMapNoiseSettings& height_map_noise_settings)
	{
		const HeightMap height_map{ height_map_settings, height_map_noise_settings };
		createVertices(height_map);
		createIndices();

		const BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		};
		vertex_array.addVertexBuffer(m_vertices.data(), sizeof(float) * m_vertices.size(), layout);
		vertex_array.setIndexBuffer(m_indices.data(), m_indices.size());
	}

	void createVertices(const HeightMap& height_map)
	{
		const auto size = height_map_settings.size;
		const auto lod = height_map_settings.lod;
		const auto position = height_map_settings.position;

		m_vertices.clear();
		m_vertices.resize(size.x * size.y * lod * lod * 5);

		const auto start_x = position.x * size.x + position.x * -1;
		const auto start_y = position.y * size.y + position.y * -1;

		for (int y = 0; y < size.y * lod; ++y)
		{
			for (int x = 0; x < size.x * lod; ++x)
			{
				const size_t index = (x + y * size.x * lod) * 5;

				const float x1 = x / (float)lod + start_x;
				const float y1 = y / (float)lod + start_y;

				m_vertices[index] = x1;
				m_vertices[index + 1] = height_map[x + y * size.x * lod];
				m_vertices[index + 2] = y1;
				m_vertices[index + 3] = x1 / 10;
				m_vertices[index + 4] = y1 / 10;
			}
		}
	}

	void createIndices()
	{
		const auto size = height_map_settings.size;
		const auto lod = height_map_settings.lod;

		m_indices.clear();
		m_indices.resize((size.x * lod - 1 * lod) * (size.y * lod - 1 * lod) * 6);
		for (int y = 0; y < size.y * lod - 1 * lod; ++y)
		{
			for (int x = 0; x < size.x * lod - 1 * lod; ++x)
			{
				const size_t index = (x + y * (size.x * lod - 1 * lod)) * 6;
				m_indices[index] = x + y * size.x * lod;
				m_indices[index + 1] = x + (y + 1) * size.x * lod;
				m_indices[index + 2] = x + 1 + y * size.x * lod;
				m_indices[index + 3] = x + 1 + y * size.x * lod;
				m_indices[index + 4] = x + (y + 1) * size.x * lod;
				m_indices[index + 5] = x + 1 + (y + 1) * size.x * lod;
			}
		}
	}


	std::vector<float> m_vertices;
	std::vector<uint32_t> m_indices;


public:
	VertexArray vertex_array;
	HeightMapSettings height_map_settings;

};
