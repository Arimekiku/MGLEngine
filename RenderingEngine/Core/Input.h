#pragma once

namespace RenderingEngine {
    class Input
    {
    public:
        static bool KeyPressed(int keycode);
        static bool KeyHeld(int keycode);
        static bool KeyReleased(int keycode);

        static bool MouseButtonPressed(int code);
        static bool MouseButtonReleased(int code);

        static float GetMouseX();
        static float GetMouseY();
    };
}