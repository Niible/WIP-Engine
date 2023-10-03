#pragma once

#include "../Engine/Core/Camera/CameraController.h"
#include "../Engine/Core/Layer/Layer.h"
#include "../Engine/Core/OpenGl/Renderer/Renderer.h"
#include "../Engine/Core/OpenGl/Renderer/RendererAPI.h"
#include "../Engine/Core/Log/Log.h"
#include "TerrainGenerator.h"


using namespace Engine;

class MainLayer : public Layer
{
public:
	MainLayer(int width, int height) : Layer("MainLayer"), m_cameraController(45.f, (float)width / float(height), 0.1f, 1000.f), m_terrainGenerator({ 0,0,0 }, { 16,16 })
	{
		RendererAPI::get()->setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	}

	void onEvent(Event& e) override
	{
		m_cameraController.onEvent(e);
		m_terrainGenerator.onEvent(e);
	}

	void onUpdate(float dt) override
	{
		m_cameraController.onUpdate(dt);

		RendererAPI::get()->clear();

		Renderer::beginScene(m_cameraController.getCamera());

		m_terrainGenerator.onUpdate();
		//const auto waterShader = m_shaderLibrary.get("WaterShader");
		//Renderer::submit(waterShader, m_water.GetVertexArray(), m_textures, model);

		Renderer::endScene();
	}

private:
	CameraController m_cameraController;
	TerrainGenerator m_terrainGenerator;

};