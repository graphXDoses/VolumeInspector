#include "ngnpch.h"
#include "Engine/Graphics/OpenGL/OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine/Core/Log.h"

namespace MobiusEngine
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void OpenGLContext::Init()
	{

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		NGN_CORE_INFO("OpenGL Info:");
		NGN_CORE_INFO("  Vendor: {0}", (const char*)(glGetString(GL_VENDOR)));
		NGN_CORE_INFO("  Renderer: {0}", (const char*)(glGetString(GL_RENDERER)));
		NGN_CORE_INFO("  Version: {0}", (const char*)(glGetString(GL_VERSION)));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}