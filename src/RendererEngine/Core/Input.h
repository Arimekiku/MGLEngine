#pragma once

#include <glm/vec2.hpp>

namespace RenderingEngine
{
    class Input
    {
    public:
        static bool KeyPressed(int keycode);
        static bool KeyHeld(int keycode);
        static bool KeyReleased(int keycode);

        static bool MouseButtonPressed(int code);
        static bool MouseButtonReleased(int code);

        static void SetInputMode(int mode, int value);

        static glm::vec2 GetCursorPosition();
        static glm::vec2 GetNormalizedCursor();

        static void SetCursorPosition(int x, int y);

        static float GetMouseX();
        static float GetMouseY();

        static void SetCursorInCenterOfWindow();
    };
}
