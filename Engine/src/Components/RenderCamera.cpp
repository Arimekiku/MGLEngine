#include "RenderCamera.h"

#include <glm/gtx/vector_angle.hpp>

namespace Engine
{
    RenderCamera::RenderCamera(const int width, const int height)
    {
		auto colorLayout = ImageLayout(width, height, 4);
		colorLayout.Format = RGBA8;
		colorLayout.Target = TEXTURE_2D;
        colorLayout.Name = "Color";
		const auto colorAttachment = Ptr<Texture>::Create(colorLayout);

        auto depthLayout = ImageLayout(width, height, 1);
        depthLayout.Format = DEPTH24_STENCIL8;
        depthLayout.Target = TEXTURE_2D;
        depthLayout.Name = "Depth";
		auto depthAttachment = Ptr<Texture>::Create(depthLayout);

        auto intLayout = ImageLayout(width, height, 1);
        intLayout.Format = INTEGER;
        intLayout.Target = TEXTURE_2D;
        intLayout.Name = "Int";
		auto intAttachment = Ptr<Texture>::Create(intLayout);

    	auto attachments = FramebufferLayout(width, height);
        attachments.Attachments = { colorAttachment, depthAttachment, intAttachment };

        m_Framebuffer = Ptr<Framebuffer>::Create(attachments);

        Resize(width, height);
    }

    void RenderCamera::Resize(const int width, const int height)
    {
        m_Aspect = (float)width / (float)height;

        m_Proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_MinRenderDist, m_MaxRenderDist);
        m_Framebuffer->Resize(width, height);
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
