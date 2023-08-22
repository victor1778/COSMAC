#pragma once

#include "Core.h"

namespace COSMAC {
	class COSMAC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:

	};

	Application* CreateApplication();
}
