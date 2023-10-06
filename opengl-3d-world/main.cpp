
#include "Engine/Core/Application/Application.h"
#include "Engine/Core/Layer/Layer.h"
#include "Engine/Core/Log/Log.h"
#include "Game/TerrainGenerator.h"
#include "Game/UiLayer.h"
#include "Game/MainLayer.h"


int main()
{
	Log::init();
	Application application;
	int width, height;
	glfwGetWindowSize(application.getWindow().getNativeWindow(), &width, &height);

	CameraControllerProperties camera_controller_properties = { 45.f, (float)width / float(height), 0.1f, 1000.f };
	application.pushLayer(new MainLayer(camera_controller_properties));
	application.pushLayer(new UiLayer());
	application.run();
}
