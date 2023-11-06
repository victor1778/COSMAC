#pragma once

#include "COSMAC/Core/Core.h"

#ifdef COSMAC_PLATFORM_WINDOWS

extern COSMAC::Application *COSMAC::CreateApplication();


#ifdef COSMAC_DEBUG
int main(int argc, char** argv)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
#endif
{
	COSMAC::Log::Init();

	COSMAC_CORE_WARN("Logger initialized");
	COSMAC_INFO("COSMAC Emulator v0.1.0-beta");
	
	#ifdef COSMAC_DEBUG
		COSMAC_PROFILE_BEGIN_SESSION("Startup", "COSMACProfile-Startup.json");
		auto app = COSMAC::CreateApplication();
		COSMAC_PROFILE_END_SESSION();

		COSMAC_PROFILE_BEGIN_SESSION("Runtime", "COSMACProfile-Runtime.json");
		app->Run();
		COSMAC_PROFILE_END_SESSION();

		COSMAC_PROFILE_BEGIN_SESSION("Shutdown", "COSMACProfile-Shutdown.json");
		delete app;
		COSMAC_PROFILE_END_SESSION();
	#else
		auto app = COSMAC::CreateApplication();
		app->Run();
		delete app;
	#endif
}

#endif // COSMAC_PLATFORM_WINDOWS