#pragma once

#include "COSMAC/Core/Core.h"

#include "COSMAC/Core/Window.h"
#include "COSMAC/Core/LayerStack.h"
#include "COSMAC/Events/Event.h"
#include "COSMAC/Events/ApplicationEvent.h"

#include "COSMAC/Core/Timestep.h"

#include "COSMAC/ImGui/ImGuiLayer.h"

#ifdef COSMAC_DEBUG
int main(int argc, char** argv);
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow);
#endif

namespace COSMAC
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);

		inline Window &GetWindow() { return *m_Window; }

		inline void Close() { m_Running = false; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application &Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent &e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application *s_Instance;

		#ifdef COSMAC_DEBUG
			friend int::main(int argc, char** argv);
		#else
			friend int WINAPI::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
		#endif
	};

	Application *CreateApplication();
}