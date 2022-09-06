#include "ngnpch.h"
#include "GraphicsContext.h"
#include "../OpenGL/OpenGLContext.h"

namespace VolumeEngine
{

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}

}
