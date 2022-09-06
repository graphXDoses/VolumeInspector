#include "ngnpch.h"
#include "RenderCommand.h"

namespace VolumeEngine
{

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}