#include <COSMAC.h>

class Sandbox : public COSMAC::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

COSMAC::Application* COSMAC::CreateApplication()
{
	return new Sandbox(); 
}