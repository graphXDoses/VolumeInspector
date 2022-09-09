#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/Framebuffer.h"
#include "Engine/Graphics/OpenGL/OpenGLFramebuffer.h"

namespace MobiusEngine
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		return CreateRef<OpenGLFramebuffer>(spec);
	}

}