#pragma once

#include "Engine/Core/Base.h"

#include "Engine/Core/Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Events/EventSchema.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Engine/Core/Timestep.h"

#include "Engine/ImGui/ImGuiLayer.h"


namespace MobiusEngine
{

	struct ConfigurationProtocol
	{
		std::string AppName = "MobiusEngine Generic Application";
	};

	class Application
	{
	public:
		Application(const ConfigurationProtocol& config);//
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Run();
		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ConfigurationProtocol m_config;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	protected:
		float m_LastFrameTime = 0.0f;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}