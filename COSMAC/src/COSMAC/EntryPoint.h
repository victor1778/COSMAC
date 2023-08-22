#pragma once

#ifdef COSMAC_PLATFORM_WINDOWS

extern COSMAC::Application* COSMAC::CreateApplication();

int main(int argc, char** argv)
{
	printf("COSMAC emulator v0.1\n");
	auto app = COSMAC::CreateApplication();
	app->Run();
	delete app;
}

#endif // COSMAC_PLATFORM_WINDOWS
