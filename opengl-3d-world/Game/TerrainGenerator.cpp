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
	generateInitialChunks();
}

void TerrainGenerator::generateNewChunk(glm::ivec2 chunk_pos)
{
	const std::string key = glm::to_string(chunk_pos);
	if (!chunks.contains(key))
	{
		auto hms = height_map_settings;
		hms.position = chunk_pos;
		chunks.emplace(key, createRef<Chunk>(hms, height_map_noise_settings));
	}
}

void TerrainGenerator::onUpdate() const
{
	const glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f));
	for (auto& [key, chunk] : chunks)
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
			generateInitialChunks();
			return true;
		});

	dispatcher.dispatch<DataEvent<HeightMapSettings>>([this](DataEvent<HeightMapSettings>& ev)
		{
			height_map_settings = ev.getValue();
			deleteAllChunks();
			generateInitialChunks();
			return true;
		});

}

void TerrainGenerator::setTerrainThresholds()
{
	shader->bind();
	shader->setFloat("sandThreshold", terrain_thresholds.sand_threshold);
	shader->setFloat("grassThreshold", terrain_thresholds.grass_threshold);
	shader->setFloat("rockThreshold", terrain_thresholds.rock_threshold);
}

void TerrainGenerator::generateInitialChunks()
{
	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			const glm::ivec2 pos = { position.x + x, position.y + y };
			generateNewChunk(pos);
		}
	}
	
}

void TerrainGenerator::deleteAllChunks()
{
	chunks.clear();
}
