#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/RenderCommand.h"

namespace MobiusEngine
{

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}