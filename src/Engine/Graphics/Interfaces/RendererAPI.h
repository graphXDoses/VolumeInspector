#pragma once
#include <glm/ext/vector_float4.hpp>
#include "VertexArray.h"


namespace VolumeEngine
{

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void Init(const bool& culling=false) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		static Scope<RendererAPI> Create();
	};

}