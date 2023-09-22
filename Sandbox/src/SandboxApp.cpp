#include <COSMAC.h>
#include <COSMAC/Core/EntryPoint.h>

#include "EmulationLayer.h"

class Sandbox : public COSMAC::Application
{
public:
	Sandbox()
	{
		PushLayer(new EmulationLayer());
	}

	~Sandbox()
	{
	}
};

COSMAC::Application *COSMAC::CreateApplication()
{
	return new Sandbox();
}