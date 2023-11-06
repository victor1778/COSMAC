#include "cpch.h"

#include "COSMAC/Renderer/Buffer.h"
#include "COSMAC/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace COSMAC
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

}