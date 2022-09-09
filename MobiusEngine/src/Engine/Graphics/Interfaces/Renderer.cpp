#include "ngnpch.h"
#include "Engine/Graphics/Interfaces/Renderer.h"

namespace MobiusEngine
{

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init(const bool& culling)
	{
		RenderCommand::Init(culling);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		s_SceneData->ViewMatrix       = camera.GetViewMatrix();
		s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
	}

	void Renderer::Shutdown() {}
	void Renderer::EndScene() {}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("matProj",  s_SceneData->ProjectionMatrix);
		shader->SetMat4("matView",  s_SceneData->ViewMatrix);
		shader->SetMat4("matModel", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		//RenderCommand::DrawLines(vertexArray, vertexArray->GetVertexBuffers()[0]->GetSize());

	}


}