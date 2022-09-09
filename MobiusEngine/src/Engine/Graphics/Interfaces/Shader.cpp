#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/Shader.h"
#include "Engine/Graphics/OpenGL/OpenGLShader.h"

namespace MobiusEngine
{
	/*
	Ref<Shader> Create(const std::string& filepath)
	{
		return CreateRef<OpenGLShader>(filepath);
	}
	*/

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
	}

}