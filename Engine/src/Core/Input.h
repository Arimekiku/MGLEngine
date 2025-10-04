#pragma once

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

namespace Engine::Input
{
	class Mouse
	{
	public:
		enum MouseMode
		{
			Visible = GLFW_CURSOR_NORMAL,
			Hidden = GLFW_CURSOR_HIDDEN,
		};

		static bool ButtonPressed(int code);
		static bool ButtonReleased(int code);

		static void SetInputMode(MouseMode mode);

		static glm::vec2 GetCursorPosition();
		static glm::vec2 GetNormalizedCursor();

		static void SetCursorPosition(int x, int y);

		static float GetMouseX();
		static float GetMouseY();

		static void SetCursorInCenterOfWindow();
	};

	class Key
	{
	public:
		enum KeyCode
		{
			W = GLFW_KEY_W,
			O = GLFW_KEY_O,
			A = GLFW_KEY_A,
			S = GLFW_KEY_S,
			D = GLFW_KEY_D,
			C = GLFW_KEY_C,
			N = GLFW_KEY_N,
			F1 = GLFW_KEY_F1,
			F2 = GLFW_KEY_F2,
			F3 = GLFW_KEY_F3,
			LeftShift = GLFW_KEY_LEFT_SHIFT,
			RightShift = GLFW_KEY_RIGHT_SHIFT,
			LeftCtrl = GLFW_KEY_LEFT_CONTROL,
			RightCtrl = GLFW_KEY_RIGHT_CONTROL,
			Space = GLFW_KEY_SPACE,
		};

		static bool Pressed(KeyCode code);
		static bool Held(KeyCode code);
		static bool Released(KeyCode code);

		static int GetAxisValue(KeyCode positiveKey, KeyCode negativeKey);
	};
}
