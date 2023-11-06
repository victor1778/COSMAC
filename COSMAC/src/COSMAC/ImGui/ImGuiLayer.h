#pragma once

#include "COSMAC/Core/Layer.h"

#include "COSMAC/Events/ApplicationEvent.h"
#include "COSMAC/Events/KeyEvent.h"
#include "COSMAC/Events/MouseEvent.h"

namespace COSMAC
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}