#include "mxpch.h"
#include "Framebuffer.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine 
{
    Framebuffer::Framebuffer(int16_t width, int16_t height, FramebufferType type)
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
        if (m_RendererID != 0)
        {
            glDeleteFramebuffers(1, &m_RendererID);

            for (const uint32_t index : m_Attachments)
            {
                glDeleteTextures(1, &index);
            }

            m_Attachments.clear();
        }
    }

    void Framebuffer::CreateAsViewport()
    {
        glViewport(0, 0, m_Width, m_Height);
        Clear();

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        uint32_t textureMapID;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureMapID);
        glBindTexture(GL_TEXTURE_2D, textureMapID);
        glTextureStorage2D(textureMapID, 1, GL_RGBA8, m_Width, m_Height);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureMapID, 0);
        m_Attachments.push_back(textureMapID);

        uint32_t depthMapID;
        glCreateTextures(GL_TEXTURE_2D, 1, &depthMapID);
        glBindTexture(GL_TEXTURE_2D, depthMapID);
        glTextureStorage2D(depthMapID, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
        m_Attachments.push_back(depthMapID);

        LOG_RENDERER_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::CreateAsDepthBuffer()
    {
        glViewport(0, 0, m_Width, m_Height);
        Clear();

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        uint32_t depthMapID;
        glCreateTextures(GL_TEXTURE_2D, 1, &depthMapID);
        glBindTexture(GL_TEXTURE_2D, depthMapID);
        glTextureStorage2D(depthMapID, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        constexpr float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
        m_Attachments.push_back(depthMapID);

        glDrawBuffer(GL_NONE);

        LOG_RENDERER_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t Framebuffer::GetAttachment(int16_t index)
    {
        if (index > m_Attachments.capacity())
        {
            LOG_RENDERER_WARN("Can't get attachment from FBO with index: %d", index);
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
