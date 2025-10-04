#include "Framebuffer.h"
#include "Renderer.h"
#include "Core/Bootstrapper.h"

#include <glad/glad.h>

namespace Engine 
{
    Framebuffer::Framebuffer(const int16_t width, const int16_t height, const FramebufferType type)
    {
        m_Type = type;

        Resize(width, height);
    }

    Framebuffer::~Framebuffer()
    {
        Clear();
    }

    void Framebuffer::Clear()
    {
        if (m_RendererId == 0)
            return;

        glDeleteFramebuffers(1, &m_RendererId);

        for (const uint32_t index : m_Attachments)
            glDeleteTextures(1, &index);

        m_Attachments.clear();
    }

    void Framebuffer::CreateAsViewport()
    {
        Clear();

        glCreateFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        uint32_t textureMapId;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureMapId);
        glBindTexture(GL_TEXTURE_2D, textureMapId);
        glTextureStorage2D(textureMapId, 1, GL_RGBA8, m_Width, m_Height);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureMapId, 0);
        m_Attachments.push_back(textureMapId);

        uint32_t depthMapId;
        glCreateTextures(GL_TEXTURE_2D, 1, &depthMapId);
        glBindTexture(GL_TEXTURE_2D, depthMapId);
        glTextureStorage2D(depthMapId, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMapId, 0);
        m_Attachments.push_back(depthMapId);

        LOG_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::CreateAsDepthBuffer()
    {
        Clear();

        glCreateFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        uint32_t depthMapId;
        glCreateTextures(GL_TEXTURE_2D, 1, &depthMapId);
        glBindTexture(GL_TEXTURE_2D, depthMapId);
        glTextureStorage2D(depthMapId, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        constexpr float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMapId, 0);
        m_Attachments.push_back(depthMapId);

        glDrawBuffer(GL_NONE);

        LOG_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind() const
    {
        if (m_Type == FramebufferType::DepthBuffer)
            glCullFace(GL_FRONT);

        glViewport(0, 0, m_Width, m_Height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (m_Type == FramebufferType::DepthBuffer)
        {
            glCullFace(GL_BACK);
        }
    }

    uint32_t Framebuffer::GetAttachment(int16_t index) const
    {
        if (index > m_Attachments.capacity())
        {
            LOG_ERROR("Can't get attachment from FBO with index: %d", index);
            return 0;
        }

        return m_Attachments[index];
    }

    void Framebuffer::Resize(const int16_t width, const int16_t height)
    {
        m_Width = width; m_Height = height;

        if (m_Type == FramebufferType::Viewport)
        {
            CreateAsViewport();
            return;
        }

        CreateAsDepthBuffer();
    }
}
