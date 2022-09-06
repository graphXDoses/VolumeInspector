#include "ngnpch.h"
#include "VolumeEngine.h"

#include "EditorLayer.h"

namespace VolumeEngine
{

	class VolumeApp : public Application
	{
	public:
		VolumeApp(const std::string& name)
			:Application(name)
		{
			//Application::GetWindow().SetVSync(false);
			PushLayer(new EditorLayer());
		}

		~VolumeApp() {}

	};

	Application* CreateApplication(const std::string& name)
	{
		return new VolumeApp(name);
	}

}