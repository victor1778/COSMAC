#include "cpch.h"

#include "COSMAC/Renderer/GraphicsContext.h"
#include "COSMAC/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace COSMAC
{

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}