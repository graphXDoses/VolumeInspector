#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/VertexArray.h"
#include "Engine/Graphics/OpenGL/OpenGLVertexArray.h"

namespace MobiusEngine
{

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}

}