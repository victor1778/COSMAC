#pragma once

// Precompiled header
#include "cpch.h"

// Include necessary headers
#include "COSMAC/Core.h"
#include "COSMAC/Events/Event.h"

namespace COSMAC
{

	// Define a structure to hold window properties
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		// Constructor with default values for title, width, and height
		WindowProps(const std::string &title = "COSMAC Emulator", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Declare a class for managing windows
	class COSMAC_API Window
	{
	public:
		// Define a function signature for event callback
		using EventCallbackFn = std::function<void(Event &)>;

		// Destructor (to be implemented by derived classes)
		virtual ~Window() {}

		// Pure virtual function to be implemented by derived classes for updating the window
		virtual void OnUpdate() = 0;

		// Pure virtual functions to get the width and height of the window
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Pure virtual function to set the event callback function
		virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

		// Pure virtual functions to set and query vertical sync (VSync) state
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// Static function to create a window instance with specified properties
		static Window *Create(const WindowProps &props = WindowProps());
	};
}