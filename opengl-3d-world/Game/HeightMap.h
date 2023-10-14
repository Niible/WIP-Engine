#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <FastNoise/FastNoise.h>
#include "../Engine/Utils/MapRange.h"


struct HeightMapNoiseSettings
{
	int octave = 5;
	float lacunarity = 2.f;
	float gain = 0.5f;
	float frequency = 0.02f;
	int seed = 1337;
};

struct HeightMapSettings {
	glm::ivec2 position = { 0, 0 };
	glm::ivec2 size = { 16, 16 };
	int lod = 1;
	std::vector<float> map_points;
	std::vector<float> map_values;
};


class HeightMap : public std::vector<float>
{
public:
	//GLuint textureId = 0;

	HeightMap(const HeightMapSettings& map_settings, const HeightMapNoiseSettings& noise_settings): m_map_settings(map_settings)
	{
		compute(noise_settings);
	}

	void compute(const HeightMapNoiseSettings& noise_settings) {
		resize(m_map_settings.size.x * m_map_settings.size.y * m_map_settings.lod * m_map_settings.lod);

		const auto start_x = m_map_settings.position.x * m_map_settings.size.x + m_map_settings.position.x * -1;
		const auto start_y = m_map_settings.position.y * m_map_settings.size.y + m_map_settings.position.y * -1;

		const auto simplex = FastNoise::New<FastNoise::Simplex>();
		const auto fractal = FastNoise::New<FastNoise::FractalFBm>();
		fractal->SetSource(simplex);
		fractal->SetOctaveCount(noise_settings.octave);
		fractal->SetLacunarity(noise_settings.lacunarity);
		fractal->SetGain(noise_settings.gain);

		fractal->GenUniformGrid2D(this->data(), start_x * m_map_settings.lod, start_y * m_map_settings.lod, m_map_settings.size.x * m_map_settings.lod, m_map_settings.size.y * m_map_settings.lod , noise_settings.frequency / m_map_settings.lod, noise_settings.seed);

		// Apply custom range to map value from 0 - 1 to x - y
		if (m_map_settings.map_points.size() > 1) {
			for (int i = 0; i < this->size(); ++i)
			{
				float& value = this->data()[i];
				for (int p = 1; p < m_map_settings.map_points.size(); ++p)
				{
					const auto min = m_map_settings.map_points[p-1];
					const auto max = m_map_settings.map_points[p];
					if (value > max) continue;
					const auto min_v = m_map_settings.map_values[p - 1];
					const auto max_v = m_map_settings.map_values[p];
					value = mapRange(value, min, max, min_v, max_v);
				}
			}
		}
	}

	HeightMapSettings m_map_settings;
};