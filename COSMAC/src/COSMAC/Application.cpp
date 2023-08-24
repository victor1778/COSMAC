#include "cpch.h"
#include "Application.h"

#include "COSMAC/Events/ApplicationEvent.h"
#include "COSMAC/Log.h"

namespace COSMAC
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			COSMAC_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			COSMAC_TRACE(e);
		}

		while (true);
	}
}