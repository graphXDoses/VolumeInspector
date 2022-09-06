#pragma once
#include "../Interfaces/RendererAPI.h"

namespace VolumeEngine
{

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init(const bool& culling = false) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void SetLineWidth(float width) override;
	};

}