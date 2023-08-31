#include <COSMAC.h>
#include "imgui/imgui.h"

class ExampleLayer : public COSMAC::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (COSMAC::Input::IsKeyPressed(COSMAC_KEY_TAB))
		{
			COSMAC_TRACE("Tab key is pressed (poll)!");
		}
	}

	virtual void OnImGuiRender() override
	{
		/*ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();*/
	}

	void OnEvent(COSMAC::Event &event) override
	{
		if (event.GetEventType() == COSMAC::EventType::KeyPressed)
		{
			COSMAC::KeyPressedEvent& e = (COSMAC::KeyPressedEvent&)event;
			if (e.GetKeyCode() == COSMAC_KEY_TAB)
				COSMAC_TRACE("Tab key is pressed (event)!");
			COSMAC_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public COSMAC::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

COSMAC::Application *COSMAC::CreateApplication()
{
	return new Sandbox();
}