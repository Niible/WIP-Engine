#include "TerrainGenerator.h"
#include "../Engine/Core/OpenGl/Renderer/Renderer.h"

TerrainGenerator::TerrainGenerator(glm::vec3 position, glm::ivec2 chunk_size)
	: position(position), chunk_size(chunk_size)
{
	m_textures.emplace_back(createScope<Texture2D>("grassTexture", "./assets/textures/Grass.jpg"));
	m_textures.emplace_back(createScope<Texture2D>("rockTexture", "./assets/textures/Rock.jpg"));
	m_textures.emplace_back(createScope<Texture2D>("sandTexture", "./assets/textures/sand.bmp"));
	m_textures.emplace_back(createScope<Texture2D>("snowTexture", "./assets/textures/Neige.png"));

	shader = createRef<Shader>("./assets/shaders/Map/vertexShader.glsl", "./assets/shaders/Map/fragmentShader.glsl");
	setTerrainThresholds();
}

void TerrainGenerator::generateChunk(glm::ivec2 position)
{
	auto hms = height_map_settings;
	hms.position = position;
	loaded_chunks.emplace_back(createRef<Chunk>(hms, height_map_noise_settings));
}

void TerrainGenerator::unloadChunk(const size_t index)
{
	// move the chunk to the cached chunks then delete it from the loaded.
	cached_chunks.push_back(std::move(loaded_chunks[index]));
	loaded_chunks.erase(loaded_chunks.begin() + index);
} 

bool TerrainGenerator::isChunkLoaded(glm::ivec2 position)
{
	return std::ranges::find_if(loaded_chunks, [position](const Ref<Chunk>& chunk) {
		return chunk->height_map_settings.position == position;
		}) != loaded_chunks.end();
}


void TerrainGenerator::loadOrGenerateChunk(const glm::ivec2 position)
{
	const auto it = std::ranges::find_if(cached_chunks, [position](const Ref<Chunk>& chunk) {
		return chunk->height_map_settings.position == position;
		});
	if (it != cached_chunks.end())
	{
		// move the chunk to the loaded chunks then delete it from the cache.
		loaded_chunks.push_back(std::move(*it));
		cached_chunks.erase(it);
	} else
	{
		generateChunk(position);
	}
}


void TerrainGenerator::onUpdate() const
{
	const glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f));
	for (auto& chunk : loaded_chunks)
	{
		Renderer::submit(shader, chunk->vertex_array, m_textures, model);
	}
}

void TerrainGenerator::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.dispatch<DataEvent<TerrainThresholds>>([this](DataEvent<TerrainThresholds>& ev)
		{
			terrain_thresholds = ev.getValue();
			setTerrainThresholds();
			return true;
		});

	dispatcher.dispatch<DataEvent<HeightMapNoiseSettings>>([this](DataEvent<HeightMapNoiseSettings>& ev)
		{
			height_map_noise_settings = ev.getValue();
			deleteAllChunks();
			return true;
		});

	dispatcher.dispatch<DataEvent<HeightMapSettings>>([this](DataEvent<HeightMapSettings>& ev)
		{
			height_map_settings = ev.getValue();
			deleteAllChunks();
			return true;
		});

}

void TerrainGenerator::setTerrainThresholds() const
{
	shader->bind();
	shader->setFloat("sandThreshold", terrain_thresholds.sand_threshold);
	shader->setFloat("grassThreshold", terrain_thresholds.grass_threshold);
	shader->setFloat("rockThreshold", terrain_thresholds.rock_threshold);
}

void TerrainGenerator::deleteAllChunks()
{
	loaded_chunks.clear();
	cached_chunks.clear();
}
