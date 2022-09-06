#include "ngnpch.h"
#include "UniformBuffer.h"

#include "Engine/Graphics/OpenGL/OpenGLUniformBuffer.h"

namespace VolumeEngine
{



	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		return CreateRef<OpenGLUniformBuffer>(size, binding);
	}


}