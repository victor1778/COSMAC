#pragma once

namespace COSMAC
{

	class Input
	{
	public:
		virtual ~Input() = default;

		Input(const Input&) = delete;
		Input& operator = (const Input&) = delete;

		inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return s_instance->GetMousePosImpl(); }
		inline static bool GetMouseX() { return s_instance->GetMouseXImpl(); }
		inline static bool GetMouseY() { return s_instance->GetMouseYImpl(); }

	protected:
		Input() = default;

		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_instance;
	};

}