#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Engine;

Camera::Camera(const float fov, const float aspect_ratio, const float near_clip, const float far_clip) :
	m_fov(fov), m_aspectRatio(aspect_ratio), m_near(near_clip), m_far(far_clip)
{
    recalculateFront(); // Calculate the front vector first.
    recalculateView(); // Then the view matrix since it depends on the front vector.
    recalculateProjection(); // Finally the projection.

}

void Camera::setViewportSize(const float width, const float height)
{
    if (height == 0.0f) return; // Avoid division by zero
    m_aspectRatio = width / height;
    recalculateProjection();
}

void Camera::setFov(const float fov)
{
    m_fov = fov;
    recalculateProjection();
}

void Camera::setNearClip(const float near_clip)
{
    m_near = near_clip;
    recalculateProjection();
}

void Camera::setFarClip(const float far_clip)
{
    m_far = far_clip;
    recalculateProjection();
}

void Camera::setPosition(const glm::vec3& position)
{
    m_position = position;
    recalculateView();
}

void Camera::setYaw(const float yaw)
{
    m_yaw = yaw;
    recalculateFront();
    recalculateView();
}

void Camera::setPitch(float pitch)
{
    if (m_pitch == pitch) return;

    m_pitch = pitch;

    // Clamp the pitch to avoid gimbal lock.
    if (m_pitch > MAX_PITCH)
        m_pitch = MAX_PITCH;
    else if (m_pitch < MIN_PITCH)
        m_pitch = MIN_PITCH;

    recalculateFront();
    recalculateView();
}

void Camera::recalculateProjection()
{
    m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
    m_view_projection = m_view * m_projection;
}

void Camera::recalculateView()
{
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
    m_view_projection = m_view * m_projection;
}

void Camera::recalculateFront()
{
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);
}