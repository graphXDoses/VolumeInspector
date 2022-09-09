#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/RendererAPI.h"

#include "Engine/Graphics/OpenGL/OpenGLRendererAPI.h"

namespace MobiusEngine
{

	Scope<RendererAPI> RendererAPI::Create()
	{
		return CreateScope<OpenGLRendererAPI>();
	}

}