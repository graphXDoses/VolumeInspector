#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/Buffer.h"

#include "Engine/Graphics/OpenGL/OpenGLBuffer.h"

namespace MobiusEngine
{

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, count);
	}


}