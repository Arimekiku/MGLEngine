#pragma once

#include "Events/Event.h"

#include <glm/glm.hpp>

#include "Events/KeyEvent.h"
#include "Events/WindowEvent.h"

namespace RenderingEngine
{
    struct CameraAttributes
    {
        float FOV;
        float MinRenderDist;
        float MaxRenderDist;
        float Aspect;

        explicit CameraAttributes(const float fov = 45,
                                  const float minRenderDist = 0.1f,
                                  const float maxRenderDist = 100)
        {
            FOV = fov;
            MinRenderDist = minRenderDist;
            MaxRenderDist = maxRenderDist;
            Aspect = 16.0f / 9.0f;
        }
    };

    class Camera
    {
    public:
        explicit Camera(const glm::vec3& position, const CameraAttributes& attributes = CameraAttributes());
        ~Camera() = default;

        void EveryUpdate();
        void OnEvent(Event& e);

        const glm::mat4& GetProjViewMat();

        glm::vec3 Position{};

    private:
        bool OnWindowResizeEvent(const WindowResizeEvent& e);
        bool OnKeyPressedEvent(const KeyPressedEvent& e);
        void SetOrientation(float rotX, float rotY);

        CameraAttributes m_Attributes;

        glm::mat4 m_ProjView = glm::mat4(1.0f);
        glm::vec3 m_Orientation = glm::vec3(0.25f, 0.25f, 1.0f);
        glm::vec3 m_Up = glm::vec3(0, 1.0f, 0.0f);
        bool m_CameraEditorMode = true;
    };
}
