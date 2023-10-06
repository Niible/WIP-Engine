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

	void generateChunk(glm::ivec2 position);

	void unloadChunk(size_t index);

	void onUpdate() const;

	void onEvent(Event& e);

	bool isChunkLoaded(glm::ivec2 position);

	void loadOrGenerateChunk(glm::ivec2 position);

private:
	void setTerrainThresholds() const;

	void deleteAllChunks();

public:
	glm::ivec3 position;
	glm::ivec2 chunk_size;

	std::vector<Ref<Chunk>> loaded_chunks;
	std::vector<Ref<Chunk>> cached_chunks;

	Ref<Shader> shader;

	TerrainThresholds terrain_thresholds;
	HeightMapNoiseSettings height_map_noise_settings;
	HeightMapSettings height_map_settings;
private:

	std::vector<Scope<Texture2D>> m_textures;


};
