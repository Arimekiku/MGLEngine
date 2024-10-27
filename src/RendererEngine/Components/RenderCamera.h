#pragma once

#include "RendererEngine/Events/Event.h"
#include "RendererEngine/Events/KeyEvent.h"
#include "RendererEngine/Events/WindowEvent.h"

#include "RendererEngine/Core/Time.h"

#include <glm/glm.hpp>

namespace RenderingEngine
{
    class RenderCamera
    {
    public:
        RenderCamera();
        ~RenderCamera() = default;

        void Resize(float width, float height);

        void OnEvent(Event& e);

        glm::mat4& SetPerspective(float fov, float minDist, float maxDist);

        const glm::mat4& GetProjMat() { return m_Proj; }

    private:
        bool OnWindowResizeEvent(const WindowResizeEvent& e);

        float m_FOV = 45;
        float m_MinRenderDist = 0.1f;
        float m_MaxRenderDist = 100.0f;
        float m_Aspect = 1.0f;

        glm::mat4 m_Proj = glm::mat4(1.0f);
    };
}
