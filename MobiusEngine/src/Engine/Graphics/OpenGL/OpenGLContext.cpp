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
		//NGN_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//NGN_CORE_ASSERT(status, "Failed to initialize Glad!");

		NGN_CORE_INFO("OpenGL Info:");
		NGN_CORE_INFO("  Vendor: {0}", (const char*)(glGetString(GL_VENDOR)));
		NGN_CORE_INFO("  Renderer: {0}", (const char*)(glGetString(GL_RENDERER)));
		NGN_CORE_INFO("  Version: {0}", (const char*)(glGetString(GL_VERSION)));

		//NGN_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "OpenGL version 4.5 or later, is required!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}