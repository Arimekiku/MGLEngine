#pragma once

#include "Framebuffer.h"
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

        void Resize(const uint16_t width, const uint16_t height);

        glm::mat4& SetPerspective(float fov, float minDist, float maxDist);

        const glm::vec2 GetSize() { return glm::vec2(m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight()); }
        const glm::mat4& GetProjMat() { return m_Proj; }
        
        Framebuffer& GetFramebuffer() { return m_Framebuffer; }
        const Framebuffer& GetFramebuffer() const { return m_Framebuffer; }

    private:
        float m_FOV = 45;
        float m_MinRenderDist = 0.1f;
        float m_MaxRenderDist = 100.0f;
        float m_Aspect = 1.0f;

        glm::mat4 m_Proj = glm::mat4(1.0f);
        Framebuffer m_Framebuffer;
    };
}
