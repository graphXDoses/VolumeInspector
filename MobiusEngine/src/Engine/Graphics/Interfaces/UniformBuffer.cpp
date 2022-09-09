#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/UniformBuffer.h"

#include "Engine/Graphics/OpenGL/OpenGLUniformBuffer.h"

namespace MobiusEngine
{



	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		return CreateRef<OpenGLUniformBuffer>(size, binding);
	}


}