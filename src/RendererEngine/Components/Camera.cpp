#include "Camera.h"
#include "RendererEngine.h"

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/vector_angle.hpp>

namespace RenderingEngine
{
    Camera::Camera(const glm::vec3& position, const CameraAttributes& attributes) : Position(position), m_Attributes(attributes)
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();
        Resize(window.GetWidth(), window.GetHeight());
    }

    void Camera::Resize(const float width, const float height)
    {
        m_Attributes.Aspect = width / height;

        m_View = glm::lookAt(Position, Position + m_Orientation, m_Up);
        m_Proj = glm::perspective(glm::radians(m_Attributes.FOV),
                                                m_Attributes.Aspect,
                                                m_Attributes.MinRenderDist,
                                                m_Attributes.MaxRenderDist);

        m_ProjView = glm::mat4(m_Proj * m_View);
    }

    void Camera::EveryUpdate(Time deltaTime)
    {
        if (m_CameraEditorMode == true)
            return;

        m_View = glm::lookAt(Position, Position + m_Orientation, m_Up);
        m_Proj = glm::perspective(glm::radians(m_Attributes.FOV),
                                                m_Attributes.Aspect,
                                                m_Attributes.MinRenderDist,
                                                m_Attributes.MaxRenderDist);

        m_ProjView = glm::mat4(m_Proj * m_View);

        Input::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        const glm::vec2 rotationVector = Input::GetNormalizedCursor();
        SetOrientation(rotationVector.x, rotationVector.y);
        Input::SetCursorInCenterOfWindow();

        if (Input::KeyPressed(GLFW_KEY_A))
            Position += deltaTime.GetSeconds() * -glm::normalize(glm::cross(m_Orientation, {0, 1, 0}));

        if (Input::KeyPressed(GLFW_KEY_D))
            Position += deltaTime.GetSeconds() * glm::normalize(glm::cross(m_Orientation, {0, 1, 0}));

        if (Input::KeyPressed(GLFW_KEY_W))
            Position += deltaTime.GetSeconds() * m_Orientation;

        if (Input::KeyPressed(GLFW_KEY_S))
            Position += deltaTime.GetSeconds() * -m_Orientation;
    }

    void Camera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_FUNC(OnWindowResizeEvent));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_FUNC(OnKeyPressedEvent));
    }

    void Camera::SetOrientation(const float rotX, const float rotY)
    {
        // Calculates upcoming vertical change in the Orientation
        const glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX),
                                                     glm::normalize(glm::cross(m_Orientation, m_Up)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (std::abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            m_Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
    }

    bool Camera::OnWindowResizeEvent(const WindowResizeEvent& e)
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();
        Resize(window.GetWidth(), window.GetHeight());

        return true;
    }

    bool Camera::OnKeyPressedEvent(const KeyPressedEvent& e)
    {
        if (e.GetRepeatCount() != 0)
            return true;

        if (e.GetKeyCode() == GLFW_KEY_C)
        {
            if (m_CameraEditorMode == false)
            {
                m_CameraEditorMode = true;
                Input::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);

                LOG_RENDERER_INFO("Switched to Editor Mode");
                return true;
            }

            const glm::vec2 rotationVector = Input::GetNormalizedCursor();
            SetOrientation(rotationVector.x, rotationVector.y);

            Input::SetCursorInCenterOfWindow();

            m_CameraEditorMode = false;
            LOG_RENDERER_INFO("Switched to Camera Mode");
        }

        return true;
    }
}
