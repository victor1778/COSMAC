#include "cpch.h"

#include "COSMAC/Renderer/VertexArray.h"
#include "COSMAC/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace COSMAC
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}