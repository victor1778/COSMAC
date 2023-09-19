#pragma once

#include "Core.h"

#include "Window.h"
#include "COSMAC/LayerStack.h"
#include "COSMAC/Events/Event.h"
#include "COSMAC/Events/ApplicationEvent.h"

#include "COSMAC/ImGui/ImGuiLayer.h"

#include "COSMAC/Renderer/Shader.h"
#include "COSMAC/Renderer/Buffer.h"
#include "COSMAC/Renderer/VertexArray.h"

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

		inline Window &GetWindow() { return *m_Window; }
		inline static Application &Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application *s_Instance;
	};

	Application *CreateApplication();
}