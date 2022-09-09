#include "MobiusEngine.h"
#include "Engine/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace MobiusEngine
{

	class VolumeInspector : public Application
	{
	public:
		VolumeInspector(const ConfigurationProtocol& config)
			:Application(config)
		{
			//Application::GetWindow().SetVSync(false);
			PushLayer(new EditorLayer());
		}

		~VolumeInspector() {}

	};

	Application* CreateApplication()
	{
		ConfigurationProtocol config;
		config.AppName = "VolumeInspector";

		return new VolumeInspector(config);
	}

}