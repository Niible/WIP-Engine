
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

	application.pushLayer(new MainLayer(width, height));
	application.pushLayer(new UiLayer());
	application.run();
}
