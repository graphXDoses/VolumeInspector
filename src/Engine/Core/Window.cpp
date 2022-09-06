#include "ngnpch.h"
#include "Window.h"
#include "../WindowControl/WindowControl.h"

namespace VolumeEngine
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowControl>(props);
	}

}