#pragma once
#include "Engine/Graphics/Interfaces/RenderCommand.h"
#include "Engine/Graphics/Interfaces/PerspectiveCamera.h"
#include "Engine/Graphics/Interfaces/Shader.h"

namespace MobiusEngine
{

	class Renderer
	{
	public:
		static void Init(const bool& culling=false);
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		struct SceneData
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}