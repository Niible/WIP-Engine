#pragma once
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include "../Engine/Core/OpenGl/Shader/Shader.h"
#include "../Engine/Core/OpenGl/Texture/Texture.h"

#include "Chunk.h"

#include "../Engine/Core/Event/DataEvent.h"

using namespace Engine;

struct TerrainThresholds
{
	float sand_threshold = 30.f;
	float grass_threshold = 50.f;
	float rock_threshold = 70.f;
};

class TerrainGenerator
{
public:
	TerrainGenerator(glm::vec3 position, glm::ivec2 chunk_size);

	void generateNewChunk(glm::ivec2 chunk_pos);

	void onUpdate() const;

	void onEvent(Event& e);

private:
	void setTerrainThresholds();

	void generateInitialChunks();
	void deleteAllChunks();

public:
	glm::ivec3 position;
	glm::ivec2 chunk_size;
	std::unordered_map<std::string, Ref<Chunk>> chunks;
	Ref<Shader> shader;

	TerrainThresholds terrain_thresholds;
	HeightMapNoiseSettings height_map_noise_settings;
	HeightMapSettings height_map_settings;
private:

	std::vector<Scope<Texture2D>> m_textures;


};
