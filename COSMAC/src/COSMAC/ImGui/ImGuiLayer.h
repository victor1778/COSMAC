#pragma once

#include "COSMAC/Layer.h"

#include "COSMAC/Events/ApplicationEvent.h"
#include "COSMAC/Events/KeyEvent.h"
#include "COSMAC/Events/MouseEvent.h"

namespace COSMAC
{

	class COSMAC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}