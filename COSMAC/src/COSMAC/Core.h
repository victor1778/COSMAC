#pragma once

#ifdef COSMAC_PLATFORM_WINDOWS
	#ifdef COSMAC_BUILD_DLL
		#define COSMAC_API __declspec(dllexport)
	#else
		#define COSMAC_API __declspec(dllexport)
	#endif
#else
	#error COSMAC only supports Windows
#endif
