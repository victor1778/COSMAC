#pragma once

#include "COSMAC/Core/Window.h"
#include "COSMAC/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace COSMAC
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProps &props);
		virtual ~WinWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps &props);
		virtual void Shutdown();

		GLFWwindow *m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}