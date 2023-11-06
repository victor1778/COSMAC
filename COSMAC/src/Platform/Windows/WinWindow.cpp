#include "cpch.h"

#include "Platform/Windows/WinWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "COSMAC/Events/ApplicationEvent.h"
#include "COSMAC/Events/KeyEvent.h"
#include "COSMAC/Events/MouseEvent.h"
#include "COSMAC/Renderer/Renderer.h"

namespace COSMAC
{
	float Window::s_UIScaleFactor = 1.0f;

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char *description)
	{
		COSMAC_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps &props)
	{
		return CreateScope<WinWindow>(props);
	}

	WinWindow::WinWindow(const WindowProps &props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::Init(const WindowProps &props)
	{
		COSMAC_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		COSMAC_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			COSMAC_PROFILE_SCOPE("glfwInit")
			COSMAC_CORE_INFO("Initializing GLFW.");

			int success = glfwInit();
			COSMAC_CORE_ASSERT(success, "Could not initialize GLFW!")
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			COSMAC_PROFILE_SCOPE("glfwCreateWindow");

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			float xscale, yscale;
			glfwGetMonitorContentScale(monitor, &xscale, &yscale);
			if (xscale > 1.0f || yscale > 1.0f)
			{
				s_UIScaleFactor = yscale;
				glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
			}

			#ifdef COSMAC_DEBUG
				if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
					glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = CreateScope<OpenGLContext>(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW event callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event); 
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event); 
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		/// <summary>
		/// Implements the mouse button callback function from openGL to the engine
		/// </summary>
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
		{
				/// <summary>
				/// Gets the window data from the window
				/// </summary>
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				/// <summary>
				/// Switches the action
				/// </summary>
				switch (action)
				{
					/// <summary>
					/// If the action is pressed
					/// </summary>
					case GLFW_PRESS:
					{
						/// <summary>
						/// Creates a mouse button pressed event
						/// </summary>
						MouseButtonPressedEvent event(button);
						/// <summary>
						/// Calls the event callback
						/// </summary>
						data.EventCallback(event);
						break;
					}
					/// <summary>
					/// If the action is released
					/// </summary>
					case GLFW_RELEASE:
					{
						/// <summary>
						/// Creates a mouse button released event
						/// </summary>
						MouseButtonReleasedEvent event(button);
						/// <summary>
						/// Calls the event callback
						/// </summary>
						data.EventCallback(event);
						break;
					}
					/// <summary>
					/// Mouse moved event callback
					/// </summary>
					case GLFW_REPEAT:
					{
						/// <summary>
						/// Creates a mouse button pressed event
						/// </summary>
						MouseButtonPressedEvent event(button);
						/// <summary>		
						/// Calls the event callback
						/// </summary>
						data.EventCallback(event);
						break;
					}
				}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
		
		glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
		{
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
	}

	void WinWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);

		s_GLFWWindowCount -= 1;

		if (s_GLFWWindowCount == 0)
		{
			COSMAC_CORE_INFO("Terminating GLFW.");
			glfwTerminate();
		}
	}

	void WinWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WinWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = true;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}