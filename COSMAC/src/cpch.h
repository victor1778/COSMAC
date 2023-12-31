#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "COSMAC/Core/Log.h"

#include "COSMAC/Debug/Instrumentor.h"

#ifdef COSMAC_PLATFORM_WINDOWS
	#include <Windows.h>
#endif