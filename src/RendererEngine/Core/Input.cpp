#include "Input.h"
#include "Bootstrapper.h"

namespace RenderingEngine
{
    bool Input::Key::Pressed(const KeyCode code)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
        const auto pressed = glfwGetKey(window, code);
        return pressed == GLFW_PRESS;
    }

    bool Input::Key::Held(const KeyCode code)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
        const auto pressed = glfwGetKey(window, code);
        return pressed == GLFW_REPEAT;
    }

    bool Input::Key::Released(const KeyCode code)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
        const auto pressed = glfwGetKey(window, code);
        return pressed == GLFW_RELEASE;
    }

    int Input::Key::GetAxisValue(const KeyCode positiveKey, const KeyCode negativeKey)
    {
        return Pressed(positiveKey) - Pressed(negativeKey);
    }

    bool Input::Mouse::ButtonPressed(const int code)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
        const auto pressed = glfwGetMouseButton(window, code);
          return pressed == GLFW_PRESS;
    }

    bool Input::Mouse::ButtonReleased(const int code)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();
        const auto pressed = glfwGetMouseButton(window, code);
        return pressed == GLFW_RELEASE;
    }

    void Input::Mouse::SetInputMode(const MouseMode mode)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

        glfwSetInputMode(window, GLFW_CURSOR, mode);
    }

    glm::vec2 Input::Mouse::GetCursorPosition()
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        return {mouseX, mouseY};
    }

    glm::vec2 Input::Mouse::GetNormalizedCursor()
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();
        const glm::vec2 mousePos = GetCursorPosition();

        const float width = window.GetWidth();
        const float height = window.GetHeight();

        return {
                10.0f * (mousePos.y - height / 2) / width,
                10.0f * (mousePos.x - width / 2) / width
        };
    }

    void Input::Mouse::SetCursorPosition(const int x, const int y)
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

        glfwSetCursorPos(window, x, y);
    }

    float Input::Mouse::GetMouseX()
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return static_cast<float>(xPos);
    }

    float Input::Mouse::GetMouseY()
    {
        auto& window = Bootstrapper::GetInstance().GetWindow().GetNativeWindow();

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return static_cast<float>(yPos);
    }

    void Input::Mouse::SetCursorInCenterOfWindow()
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();

        SetCursorPosition((window.GetWidth() / 2), (window.GetHeight() / 2));
    }
}
