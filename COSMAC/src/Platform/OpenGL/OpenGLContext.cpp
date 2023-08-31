#include "cpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace COSMAC {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		COSMAC_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		COSMAC_CORE_ASSERT(status, "Failed to initialize Glad!");

		COSMAC_CORE_INFO("OpenGL Info:");
		COSMAC_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		COSMAC_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		COSMAC_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}