#include "ngnpch.h"
#include "Engine/Core/Window.h"
#include "Engine/WindowControl/WindowControl.h"

namespace MobiusEngine
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowControl>(props);
	}

}