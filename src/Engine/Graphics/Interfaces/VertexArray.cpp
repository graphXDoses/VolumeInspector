#include "ngnpch.h"
#include "VertexArray.h"
#include "Engine/Graphics/OpenGL/OpenGLVertexArray.h"

namespace VolumeEngine
{

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}

}