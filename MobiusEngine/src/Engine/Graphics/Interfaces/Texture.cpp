#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/Texture.h"

#include "Engine/Graphics/OpenGL/OpenGLTexture.h"

namespace MobiusEngine
{

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		return CreateRef<OpenGLTexture2D>(width, height);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		return CreateRef<OpenGLTexture2D>(path);
	}

}