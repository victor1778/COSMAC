#include "cpch.h"
#include "WinWindow.h"

#include "COSMAC/Events/ApplicationEvent.h"
#include "COSMAC/Events/KeyEvent.h"
#include "COSMAC/Events/MouseEvent.h"

#include "glad/glad.h"

namespace COSMAC
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char *description)
	{
		COSMAC_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window *Window::Create(const WindowProps &props)
	{
		return new WinWindow(props);
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
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		COSMAC_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			COSMAC_CORE_ASSERT(success, "Could not initialize GLFW!")

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		COSMAC_CORE_ASSERT(status, "Failed to initialize Glad!");

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
	}

	void WinWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WinWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}