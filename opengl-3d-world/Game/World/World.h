#pragma once
#include "../TerrainGenerator.h"
#include "../../Engine/Core/Camera/CameraController.h"
#include "../../Engine/Core/OpenGl/Renderer/Renderer.h"
#include "../../Engine/Core/OpenGl/Renderer/RendererAPI.h"

class World
{
public:
	explicit World(const CameraControllerProperties& camera_controller_properties): m_cameraController(camera_controller_properties), m_terrainGenerator({0,0,0}, {16,16})
	{
	}

	~World() = default;

	void update()
	{
		// Get the current chunk coordinates based on the camera's position
		int current_chunk_x = m_cameraController.getCamera().getPosition().x / (chunk_size - 1);
		int current_chunk_y = m_cameraController.getCamera().getPosition().z / (chunk_size - 1);

		for (int x = current_chunk_x - load_radius; x <= current_chunk_x + load_radius; x++) {
			for (int y = current_chunk_y - load_radius; y <= current_chunk_y + load_radius; y++) {
				if (!isChunkLoaded(x, y)) {
					loadOrGenerateChunk(x, y);
				}
			}
		}

		// Unload chunks outside the load radius
		for (int i = m_terrainGenerator.loaded_chunks.size() - 1; i >= 0; --i)
		{
			auto& chunk = m_terrainGenerator.loaded_chunks[i];
			if (abs(chunk->height_map_settings.position.x - current_chunk_x) > load_radius || abs(chunk->height_map_settings.position.y - current_chunk_y) > load_radius) {
				m_terrainGenerator.unloadChunk(i);
			}
		}
	}

	void onUpdate(float dt)
	{
		m_cameraController.onUpdate(dt);
		update();

		RendererAPI::get()->clear();

		Renderer::beginScene(m_cameraController.getCamera());

		m_terrainGenerator.onUpdate();

		Renderer::endScene();
	}

	void onEvent(Event& e)
	{
		m_cameraController.onEvent(e);
		m_terrainGenerator.onEvent(e);
	}

	bool isChunkLoaded(int x, int y) {
		// Check if a chunk at the given coordinates is in the activeChunks list
		return m_terrainGenerator.isChunkLoaded({ x, y });
	}

	void loadOrGenerateChunk(int x, int y) {
		m_terrainGenerator.loadOrGenerateChunk({ x, y });
	}

private:
	CameraController m_cameraController;
	TerrainGenerator m_terrainGenerator;
	std::vector<Chunk> m_active_chunks;

	inline static int chunk_size = 16;
	inline static int load_radius = 3;

};
