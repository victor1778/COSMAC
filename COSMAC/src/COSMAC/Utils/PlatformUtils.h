#pragma once

#include <string>

namespace COSMAC
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
	};
}