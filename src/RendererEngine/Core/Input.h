#pragma once

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

namespace RenderingEngine
{
    namespace Input
    {
        class Mouse
        {
        public:
            enum MouseMode
            {
                VISIBLE = GLFW_CURSOR_NORMAL,
                HIDDEN = GLFW_CURSOR_HIDDEN,
            };

            static bool ButtonPressed(const int code);
            static bool ButtonReleased(const int code);

            static void SetInputMode(const MouseMode mode);

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
                LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
                RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
                LEFT_CTRL = GLFW_KEY_LEFT_CONTROL,
                RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
                SPACE = GLFW_KEY_SPACE,
            };

            static bool Pressed(const KeyCode code);
            static bool Held(const KeyCode code);
            static bool Released(const KeyCode code);

            static int GetAxisValue(const KeyCode positiveKey, const KeyCode negativeKey);
        };
    }
}
