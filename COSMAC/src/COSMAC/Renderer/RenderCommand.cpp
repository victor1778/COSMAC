#include "cpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace COSMAC
{

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}