#include "ngnpch.h"
#include "RendererAPI.h"

#include "Engine/Graphics/OpenGL/OpenGLRendererAPI.h"

namespace VolumeEngine
{

	Scope<RendererAPI> RendererAPI::Create()
	{
		return CreateScope<OpenGLRendererAPI>();
	}

}