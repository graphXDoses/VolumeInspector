#include "ngnpch.h"
#include "Framebuffer.h"
#include "../OpenGL/OpenGLFramebuffer.h"

namespace VolumeEngine
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		return CreateRef<OpenGLFramebuffer>(spec);
	}

}