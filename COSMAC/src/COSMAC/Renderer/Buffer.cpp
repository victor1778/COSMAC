#include "cpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace COSMAC
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None currently not supported.");
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

}