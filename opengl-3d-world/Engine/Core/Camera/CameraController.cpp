#include "CameraController.h"

#include <mutex>

#include "../Event/Event.h"
#include "../Event/MouseEvent.h"
#include "../Event/WindowEvent.h"
#include "../Window/Input.h"

using namespace Engine;

CameraController::CameraController(const float fov, const float aspect_ratio, const float near, const float far)
	: m_camera(fov, aspect_ratio, near, far)
{
}

CameraController::CameraController(const CameraControllerProperties& properties): m_camera(properties.fov, properties.aspect_ratio, properties.m_near, properties.m_far)
{
}

void CameraController::onUpdate(const float ts)
{
	auto translation_speed = m_cameraTranslationSpeed * ts;
	glm::vec3 current_position = m_camera.getPosition();

	if (Input::isKeyPressed(KeyCode::LeftControl))
		translation_speed *= 10;

	if (Input::isKeyPressed(KeyCode::A))
		current_position -= glm::normalize(glm::cross(m_camera.getFront(), m_camera.getUp())) * translation_speed;
	if (Input::isKeyPressed(KeyCode::D))
		current_position += glm::normalize(glm::cross(m_camera.getFront(), m_camera.getUp())) * translation_speed;
	if (Input::isKeyPressed(KeyCode::W))
		current_position += translation_speed * m_camera.getFront();
	if (Input::isKeyPressed(KeyCode::S))
		current_position -= translation_speed * m_camera.getFront();
	if (Input::isKeyPressed(KeyCode::Space))
		current_position += translation_speed * m_camera.getUp();
	if (Input::isKeyPressed(KeyCode::LeftShift))
		current_position -= translation_speed * m_camera.getUp();

	m_camera.setPosition(current_position);

	if (Input::isKeyPressed(KeyCode::Q))
		m_camera.setYaw(m_camera.getYaw() + m_cameraRotationSpeed * ts);
	if (Input::isKeyPressed(KeyCode::E))
		m_camera.setYaw(m_camera.getYaw() - m_cameraRotationSpeed * ts);
}

void CameraController::onEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseScrolledEvent>([this](const MouseScrolledEvent& e){ return CameraController::onMouseScrolled(e); });
	dispatcher.dispatch<WindowResizeEvent>([this](const WindowResizeEvent& e) { return CameraController::onWindowResized(e); });
	dispatcher.dispatch<MouseMovedEvent>([this](const MouseMovedEvent& e) { return CameraController::onMouseMoved(e); });
}

void CameraController::onResize(const float width, const float height)
{
	m_camera.setViewportSize(width, height);
}

bool CameraController::onMouseMoved(const MouseMovedEvent& e)
{
	static float last_x;
	static float last_y;

	static std::once_flag flag;
	std::call_once(flag, [&]()
		{
			last_x = e.getX();
			last_y = e.getY();
		});

	float x_offset = e.getX() - last_x;
	float y_offset = last_y - e.getY();

	last_x = e.getX();
	last_y = e.getY();

	if (Input::isMouseButtonPressed(ButtonLeft)) {
		constexpr float sensitivity = 0.1f;
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		m_camera.setYaw(m_camera.getYaw() + x_offset);
		m_camera.setPitch(m_camera.getPitch() + y_offset);
	}

	// could change translation speed here
	return false;
}

void CameraController::setCameraTranslationSpeed(const float speed)
{
	m_cameraTranslationSpeed = speed;
}

bool CameraController::onMouseScrolled(const MouseScrolledEvent& e)
{
	auto fov = m_camera.getFov();
	fov -= e.getYOffset();
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	m_camera.setFov(fov);
	// could change translation speed here
	return false;
}

bool CameraController::onWindowResized(const WindowResizeEvent& e)
{
	onResize(static_cast<float>(e.getWidth()), static_cast<float>(e.getHeight()));
	return false;
}