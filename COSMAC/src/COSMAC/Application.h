#pragma once

#include "Core.h"

#include "Window.h"
#include "COSMAC/LayerStack.h"
#include "COSMAC/Events/Event.h"
#include "COSMAC/Events/ApplicationEvent.h"

namespace COSMAC
{
	class COSMAC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);

	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application *CreateApplication();
}