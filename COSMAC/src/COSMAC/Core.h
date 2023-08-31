#pragma once

#ifdef COSMAC_PLATFORM_WINDOWS
#if COSMAC_DYNAMIC_LINK
	#ifdef COSMAC_BUILD_DLL
		#define COSMAC_API __declspec(dllexport)
	#else
		#define COSMAC_API __declspec(dllimport)
	#endif //COSMAC_BUILD_DLL
#else
	#define COSMAC_API
#endif //COSMAC_DYNAMIC_LINK
#else
	#error COSMAC only supports Windows!
#endif //COSMAC_PLATFORM_WINDOWS

#ifdef COSMAC_DEBUG
	#define COSMAC_ENABLE_ASSERTS
#endif

#ifdef COSMAC_ENABLE_ASSERTS
	#define COSMAC_ASSERT(x, ...) { if(!(x)) { COSMAC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define COSMAC_CORE_ASSERT(x, ...) { if(!(x)) { COSMAC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define COSMAC_ASSERT(x, ...)
	#define COSMAC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define COSMAC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)