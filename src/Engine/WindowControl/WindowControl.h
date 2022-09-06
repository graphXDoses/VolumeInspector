#pragma once
#include "../Core/Window.h"
#include "../Graphics/Interfaces/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace VolumeEngine
{

	class WindowControl : public Window
	{
	public:
		WindowControl(const WindowProps& props);
		virtual ~WindowControl();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		void SetMousePosition(const float& X, const float& Y) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}