#pragma once
#include <filesystem>
#include "MobiusEngine.h"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace MobiusEngine
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		glm::vec3& rotatePlaneNormal(bool* lock);
		void ResetCameraOrientation();
		bool ResetCameraOrientation(KeyPressedEvent& e);
		bool SetPlaneDepth(MouseScrolledEvent& e);
		void ImportImage();
		void ImportImage(const std::filesystem::path& path);
		inline glm::mat4 BBConfig(const float& angle, const glm::vec3& axis) { return glm::rotate(glm::mat4(1), glm::radians(angle), axis); }
		glm::vec3 PitchYawToDirection();
	private:
		std::filesystem::path m_path = std::filesystem::current_path();
		std::string m_lastPath = "";

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_SquareVA;
		PerspectiveCamera m_EditorCamera;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_Texture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_viewportPanelSize = { 0.0f, 0.0f };

		float lat = 0.0f;
		float m_scl = 1.963f;
		float m_opacity = 100.0f;
		float m_clipPlaneDepth = 0.0f;
		float m_isolateMin = 0.0f;
		float m_isolateMax = 100.0f;
		glm::vec3 m_clipPlaneNormal;
		bool m_colorizeIntensity = false;
		bool m_clip = false;
		bool m_lockTrack = false;
		bool m_flipClipPlane = false;

		bool m_clearCacheDialogOpen = false;

		bool m_showLegend = false;
		float m_clipPlanePitch = 0.0f, m_clipPlaneYaw = 0.0f;
	};

}