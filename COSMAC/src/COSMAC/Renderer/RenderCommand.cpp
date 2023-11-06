#include "cpch.h"

#include "COSMAC/Renderer/RenderCommand.h"

namespace COSMAC
{

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}