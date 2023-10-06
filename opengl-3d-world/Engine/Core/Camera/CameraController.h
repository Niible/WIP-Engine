#pragma once
#include "Camera.h"

namespace Engine
{
	class MouseMovedEvent;
	class WindowResizeEvent;
	class MouseScrolledEvent;
	class Event;

	struct CameraControllerProperties
	{
		float fov;
		float aspect_ratio;
		float m_near;
		float m_far;
	};

	class CameraController
	{
	public:
		CameraController(float fov, float aspect_ratio, float near, float far);
		CameraController(const CameraControllerProperties& properties);

		void onUpdate(float ts);
		void onEvent(Event& event);

		void onResize(float width, float height);

		bool onMouseMoved(const MouseMovedEvent& e);

		[[nodiscard]] Camera& getCamera() { return m_camera; }

		[[nodiscard]] float getCameraTranslationSpeed() const { return m_cameraTranslationSpeed; }
		void setCameraTranslationSpeed(float speed);

	private:
		bool onMouseScrolled(const MouseScrolledEvent& e);
		bool onWindowResized(const WindowResizeEvent& e);

		Camera m_camera;

		float m_cameraTranslationSpeed = 100.f;
		float m_cameraRotationSpeed = 2.f;
	};

}
