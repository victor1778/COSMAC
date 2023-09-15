#pragma once

#ifdef COSMAC_PLATFORM_WINDOWS

extern COSMAC::Application *COSMAC::CreateApplication();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	COSMAC::Log::Init();

	COSMAC_CORE_WARN("Initialized Log!");
	int a = 1;
	COSMAC_INFO("COSMAC Emulator v0.{0}", a);

	auto app = COSMAC::CreateApplication();
	app->Run();
	delete app;
}

#endif // COSMAC_PLATFORM_WINDOWS