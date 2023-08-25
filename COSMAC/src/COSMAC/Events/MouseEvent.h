#pragma once

#include "Event.h"

namespace COSMAC {
    /// <summary>
    ///  Mouse event class for move event and scroll event
    /// </summary>
    class COSMAC_API MouseEvent : public Event
    {
    public:
        inline float GetX() const { return m_X; }
        inline float GetY() const { return m_Y; }
           
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseEvent(float x, float y)
            : m_X(x), m_Y(y) {}

        float m_X, m_Y;
    };

    class COSMAC_API MouseMovedEvent : public MouseEvent
    {
    public:
        MouseMovedEvent(float x, float y)
            : MouseEvent(x, y) {}
        inline float GetX() const { return m_X; }
        inline float GetY() const { return m_Y; }


        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_X << ", " << m_Y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
    private:
        float m_X, m_Y;
    };

    class COSMAC_API MouseScrolledEvent : public MouseEvent
    {
    public:
        MouseScrolledEvent(float offsetX, float offsetY)
            : MouseEvent(offsetX, offsetY) {}
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_X << ", " << m_Y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
    };

    class COSMAC_API MouseButtonEvent : public Event
	{
    public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};
	/// <summary>
	///  Mouse class that handles mouse button pressed event
	/// </summary>
	class COSMAC_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class COSMAC_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};




}