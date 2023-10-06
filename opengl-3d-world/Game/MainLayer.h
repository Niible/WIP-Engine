#pragma once

#include "../Engine/Core/Camera/CameraController.h"
#include "../Engine/Core/Layer/Layer.h"
#include "../Engine/Core/OpenGl/Renderer/RendererAPI.h"
#include "World/World.h"


using namespace Engine;

class MainLayer : public Layer
{
public:
	MainLayer(CameraControllerProperties& camera_controller_properties) : Layer("MainLayer"),
		m_world(camera_controller_properties)
	{
		RendererAPI::get()->setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	}

	void onEvent(Event& e) override
	{
		m_world.onEvent(e);
	}

	void onUpdate(const float dt) override
	{
		m_world.onUpdate(dt);
	}

private:
	World m_world;

};