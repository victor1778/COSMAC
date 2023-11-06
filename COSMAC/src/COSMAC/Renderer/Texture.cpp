#include "cpch.h"

#include "COSMAC/Renderer/Texture.h"
#include "COSMAC/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace COSMAC
{
	Ref<Texture> Texture::Create(uint32_t* data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			COSMAC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture>(data);
		}

		COSMAC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}