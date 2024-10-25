#pragma once

#include "RendererEngine/Events/Event.h"
#include "RendererEngine/Events/KeyEvent.h"
#include "RendererEngine/Events/WindowEvent.h"

#include "RendererEngine/Core/Time.h"

#include <glm/glm.hpp>

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
        Camera(const glm::vec3& position, const CameraAttributes& attributes = CameraAttributes());
        ~Camera() = default;

        void Resize(float width, float height);

        void EveryUpdate(Time deltaTime);
        void OnEvent(Event& e);

        const glm::mat4& GetProjViewMat() { return m_ProjView; }
        const glm::mat4& GetViewMat() { return m_View; }
        const glm::mat4& GetProjMat() { return m_Proj; }

        glm::vec3 Position;

    private:
        bool OnWindowResizeEvent(const WindowResizeEvent& e);
        bool OnKeyPressedEvent(const KeyPressedEvent& e);
        void SetOrientation(float rotX, float rotY);

        CameraAttributes m_Attributes;

        glm::mat4 m_Proj = glm::mat4(1.0f);
        glm::mat4 m_View = glm::mat4(1.0f);
        glm::mat4 m_ProjView = glm::mat4(1.0f);

        glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        bool m_CameraEditorMode = true;
    };
}
