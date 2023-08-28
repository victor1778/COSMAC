#include <COSMAC.h>

class ExampleLayer : public COSMAC::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		COSMAC_INFO("ExampleLayer::Update");
	}

	void OnEvent(COSMAC::Event &event) override
	{
		COSMAC_TRACE("{0}", event);
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