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

#ifdef HZ_ENABLE_ASSERTS
	#define COSMAC_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define COSMAC_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define COSMAC_ASSERT(x, ...)
	#define COSMAC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)