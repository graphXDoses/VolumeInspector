#pragma once
#include "Camera.h"
#include "../../Core/Timestep.h"
#include "../../Events/EventSchema.h"
#include "../../Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace VolumeEngine
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetProjectionMatrix() const { return m_Projection; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		void ResetView();
		//void SetMousePos(const glm::vec2& pos) { m_mousePos = pos; }
		//float SetPitch(const float& Pitch = 0.0f ) { m_Pitch = Pitch; UpdateView(); }
		//float SetYaw(const float& Yaw = 0.0f) { m_Yaw = Yaw; UpdateView(); }
	protected:
		void MouseZoom(float delta);
		void UpdateView();
	private: // Functions
		void UpdateProjection();

		bool OnMouseScrollPress(MouseButtonPressedEvent& e);
		bool OnMouseScrollRelease(MouseButtonReleasedEvent& e);
		virtual bool OnMouseScroll(MouseScrolledEvent& e);

		//void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);

		glm::vec3 CalculatePosition() const;

		//std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private: // Variables
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		//glm::vec2 m_mousePos = { 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 5.0f;
		float m_Pitch = glm::radians(45.0f), m_Yaw = glm::radians(-45.0f);

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
		bool m_ShouldRotate = false;
	};
}