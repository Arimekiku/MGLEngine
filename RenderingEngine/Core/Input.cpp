#include "Input.h"

#include "Bootstrapper.h"

namespace RenderingEngine
{
        bool Input::KeyPressed(const int keycode)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
                const auto pressed = glfwGetKey(window, keycode);
                return pressed == GLFW_PRESS;
        }

        bool Input::KeyHeld(const int keycode)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
                const auto pressed = glfwGetKey(window, keycode);
                return pressed == GLFW_REPEAT;
        }

        bool Input::KeyReleased(const int keycode)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
                const auto pressed = glfwGetKey(window, keycode);
                return pressed == GLFW_RELEASE;
        }

        bool Input::MouseButtonPressed(const int code)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
                const auto pressed = glfwGetMouseButton(window, code);
                return pressed == GLFW_PRESS;
        }

        bool Input::MouseButtonReleased(const int code)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
                const auto pressed = glfwGetMouseButton(window, code);
                return pressed == GLFW_RELEASE;
        }

        void Input::SetInputMode(int mode, int value)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

                glfwSetInputMode(window, mode, value);
        }

        glm::vec2 Input::GetCursorPosition()
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

                double mouseX;
                double mouseY;
                glfwGetCursorPos(window, &mouseX, &mouseY);

                return {mouseX, mouseY};
        }

        glm::vec2 Input::GetNormalizedCursor()
        {
                auto& window = Bootstrapper::GetInstance().GetWindow();
                glm::vec2 mousePos = GetCursorPosition();

                float width = window.GetWidth();
                float height = window.GetHeight();

                return {
                        10.0f * (mousePos.y - height / 2) / width,
                        10.0f * (mousePos.x - width / 2) / width
                };
        }

        void Input::SetCursorPosition(const int x, const int y)
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

                glfwSetCursorPos(window, x, y);
        }

        float Input::GetMouseX()
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

                double xPos, yPos;
                glfwGetCursorPos(window, &xPos, &yPos);
                return static_cast<float>(xPos);
        }

        float Input::GetMouseY()
        {
                auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

                double xPos, yPos;
                glfwGetCursorPos(window, &xPos, &yPos);
                return static_cast<float>(yPos);
        }

        void Input::SetCursorInCenterOfWindow()
        {
                const auto& window = Bootstrapper::GetInstance().GetWindow();

                SetCursorPosition((window.GetWidth() / 2), (window.GetHeight() / 2));
        }
}
