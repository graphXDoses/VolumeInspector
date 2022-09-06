#include "ngnpch.h"
#include "PerspectiveCamera.h"

#include "../../Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace VolumeEngine
{

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void PerspectiveCamera::OnUpdate(Timestep ts)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.006f;
		m_InitialMousePosition = mouse;

		if (m_ShouldRotate)
		{
			MouseRotate(delta);
		}

		UpdateView();
	}

	void PerspectiveCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(NGN_BIND_EVENT_FNC(PerspectiveCamera::OnMouseScroll));
		dispatcher.Dispatch<MouseButtonPressedEvent>(NGN_BIND_EVENT_FNC(PerspectiveCamera::OnMouseScrollPress));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(NGN_BIND_EVENT_FNC(PerspectiveCamera::OnMouseScrollRelease));
	}

	glm::vec3 PerspectiveCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 PerspectiveCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat PerspectiveCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void PerspectiveCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void PerspectiveCamera::ResetView()
	{
		m_Distance = 5.0f;
		m_Pitch = glm::radians(45.0f), m_Yaw = glm::radians(-45.0f);
		UpdateView();
	}

	void PerspectiveCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool PerspectiveCamera::OnMouseScrollPress(MouseButtonPressedEvent& e)
	{
		if ( (e.GetMouseButton() == Mouse::ButtonMiddle) )
		{
			m_ShouldRotate = true;
		}
		return false;
	}

	bool PerspectiveCamera::OnMouseScrollRelease(MouseButtonReleasedEvent& e)
	{
		if ((e.GetMouseButton() == Mouse::ButtonMiddle))
		{
			m_ShouldRotate = false;
		}
		return false;
	}

	bool PerspectiveCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		if (!(Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl)))
		{
			float delta = e.GetYOffset() * 0.1f;
			MouseZoom(delta);
			UpdateView();
		}
		return false;
	}

	void PerspectiveCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void PerspectiveCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 PerspectiveCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	float PerspectiveCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float PerspectiveCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.4f;
		distance = std::max(distance, 0.1f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}


}