#pragma once

#ifdef COSMAC_PLATFORM_WINDOWS

extern COSMAC::Application *COSMAC::CreateApplication();

int main()
{
	COSMAC::Log::Init();

	COSMAC_CORE_WARN("Logger initialized");
	COSMAC_INFO("COSMAC Emulator v0.3.0-alpha");

	auto app = COSMAC::CreateApplication();
	app->Run();
	delete app;
}

#endif // COSMAC_PLATFORM_WINDOWS