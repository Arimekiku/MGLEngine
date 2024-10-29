#include "RenderCamera.h"
#include "RendererEngine.h"

#include <glm/gtx/vector_angle.hpp>

namespace RenderingEngine
{
    RenderCamera::RenderCamera()
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();
        Resize(window.GetWidth(), window.GetHeight());
    }

    void RenderCamera::Resize(const uint16_t width, const uint16_t height)
    {
        m_Aspect = (float)width / (float)height;

        m_Proj = glm::perspective(glm::radians(m_FOV), m_Aspect, m_MinRenderDist, m_MaxRenderDist);
        m_Framebuffer.Resize(width, height);
    }

    glm::mat4& RenderCamera::SetPerspective(float fov, float minDist, float maxDist)
    {
        m_FOV = fov;
        m_MinRenderDist = minDist;
        m_MaxRenderDist = maxDist;

        m_Proj = glm::perspective(glm::radians(m_FOV), m_Aspect, m_MinRenderDist, m_MaxRenderDist);

        return m_Proj;
    }
}
