#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	
	class Camera
	{
	public:
		explicit Camera(float fov = 45.0f, float aspect_ratio = 16.0f / 9.0f, float near_clip = 0.01f, float far_clip = 100.0f);

		virtual ~Camera() = default;

		void setViewportSize(float width, float height);

		void setFov(float fov);

		void setNearClip(float near_clip);

		void setFarClip(float far_clip);

		void setPosition(const glm::vec3& position);

		void setYaw(float yaw);

		void setPitch(float pitch);

		[[nodiscard]] const glm::mat4& getProjection() const { return m_projection; }

		[[nodiscard]] const glm::vec3& getPosition() const { return m_position; }

		[[nodiscard]] const glm::mat4& getView() const { return m_view; }

		[[nodiscard]] const glm::mat4& getViewProjection() const { return m_view_projection; }

		[[nodiscard]] float getYaw() const { return m_yaw; }

		[[nodiscard]] float getPitch() const { return m_pitch; }

		[[nodiscard]] float getFar() const { return m_far; }

		[[nodiscard]] float getNear() const { return m_near; }

		[[nodiscard]] float getFov() const { return m_fov; }

		[[nodiscard]] glm::vec3 getFront() const { return m_front; }

		[[nodiscard]] glm::vec3 getUp() const { return m_up; }

	private:
		void recalculateProjection();

		void recalculateView();

		void recalculateFront();

		float m_fov;
		float m_aspectRatio;
		float m_near, m_far;

		float m_yaw = -90.f;
		float m_pitch = 0.f;

		glm::mat4 m_projection = glm::mat4(1.0f);
		glm::vec3 m_position = glm::vec3(2.f, 1.f, 10.f);
		glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 m_view;
		glm::mat4 m_view_projection;

		static constexpr float MAX_PITCH = 89.0f;
		static constexpr float MIN_PITCH = -89.0f;
	};

}
