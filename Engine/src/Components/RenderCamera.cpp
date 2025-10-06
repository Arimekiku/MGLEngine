#include "RenderCamera.h"

#include <glm/gtx/vector_angle.hpp>

namespace Engine
{
    RenderCamera::RenderCamera(const int width, const int height)
    {
		auto colorAttachment = FramebufferAttachment(RGBA8);
		auto depthAttachment = FramebufferAttachment(DEPTH24_STENCIL8);
		auto intAttachment = FramebufferAttachment(INTEGER);
        m_Framebuffer.ConfigureAttachments({ colorAttachment, depthAttachment, intAttachment });

        Resize(width, height);
    }

    void RenderCamera::Resize(const int width, const int height)
    {
        m_Aspect = (float)width / (float)height;

        m_Proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_MinRenderDist, m_MaxRenderDist);
        m_Framebuffer.Resize(width, height);
    }

    glm::mat4& RenderCamera::SetPerspective(const float fov, const float minDist, const float maxDist)
    {
        m_Fov = fov;
        m_MinRenderDist = minDist;
        m_MaxRenderDist = maxDist;

        m_Proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_MinRenderDist, m_MaxRenderDist);

        return m_Proj;
    }
}
