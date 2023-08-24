#pragma once

#ifdef COSMAC_PLATFORM_WINDOWS
	#ifdef COSMAC_BUILD_DLL
		#define COSMAC_API __declspec(dllexport)
	#else
		#define COSMAC_API __declspec(dllimport)
	#endif //COSMAC_BUILD_DLL
#else
	#error COSMAC only supports Windows!
#endif //COSMAC_PLATFORM_WINDOWS

#define BIT(x) (1 << x)