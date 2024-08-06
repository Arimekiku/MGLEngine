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
}
