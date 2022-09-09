#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/GraphicsContext.h"
#include "Engine/Graphics/OpenGL/OpenGLContext.h"

namespace MobiusEngine
{

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}

}
