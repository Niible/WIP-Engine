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

		//for (int z = 0; z < size.y * lod; ++z)
		//{
		//	for (int x = 0; x < size.x * lod; ++x)
		//	{
		//		auto x1 = x / (float)lod + start_x;
		//		auto z1 = z / (float)lod + start_y;

		//		size_t index = x + z * size.x * lod;


				//float continentalnessNoise = continentalnessSettings.GetNoiseValue(continentalnessPerlin, x1 * f, z1 * f);

				//if (continentalnessSettings.splinePoints.size() > 1)
				//{
				//	auto previousSpline = continentalnessSettings.splinePoints[0];
				//	auto currentSpline = continentalnessSettings.splinePoints[0];
				//	for (int i = 0; i < continentalnessSettings.splinePoints.size(); ++i)
				//	{
				//		previousSpline = currentSpline;
				//		currentSpline = continentalnessSettings.splinePoints[i];
				//		if (continentalnessNoise < continentalnessSettings.splinePoints[i].value)
				//		{
				//			break;
				//		}
				//	}
				//	continentalnessNoise = Map(continentalnessNoise, previousSpline.value, currentSpline.value, previousSpline.height, currentSpline.height);
				//}


				//if (continentalnessSettings.ridgeNoise) {
				//	continentalnessNoise = Ridgenoise(continentalnessNoise);
				//}

				//if (continentalnessSettings.terraces) {
				//	continentalnessNoise = terraceNoise(continentalnessNoise, continentalnessSettings.terraceCount);
				//}

				//(*this)[index] = continentalnessNoise;
		//	}
		//}

	//[[nodiscard]] float Ridgenoise(const float h) const
	//{
	//	return 2 * (0.5 - abs(0.5 - h));
	//}

	//[[nodiscard]] float terraceNoise(const float h, const int terraceCount) const
	//{
	//	const float terraceHeight = 1.f / terraceCount;
	//	return floor(h / terraceHeight) * terraceHeight;
	//}

	//GLuint CreateHeightMapTexture() {
	//	std::vector<unsigned char> textureData(size.x * size.y);

	//	for (int y = 0; y < size.y; ++y) {
	//		for (int x = 0; x < size.x; ++x) {
	//			const size_t index = x + y * size.x;
	//			textureData[index] = static_cast<unsigned char>((*this)[index] * 255.0f);
	//		}
	//	}

	//	if (textureId != 0)
	//	{
	//		glDeleteTextures(1, &textureId);
	//	}

	//	glGenTextures(1, &textureId);
	//	glBindTexture(GL_TEXTURE_2D, textureId);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, textureData.data());

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);

	//	return textureId;
	//}


	//float BlendWithSubstractiveErosionNoise(float v1, float erosionNoise, float erosionFactor) {
	//	const float erosionValue = erosionNoise * erosionFactor;
	//	auto v = std::max(0.f, std::min(v1 - erosionValue, v1));
	//	return v;
	//}

};