#include "Framebuffer.h"
#include "Renderer.h"
#include "Core/Bootstrapper.h"

#include <glad/glad.h>

namespace Engine 
{
    Framebuffer::Framebuffer(const int width, const int height, const FramebufferType type)
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

        if (m_ColorAttachment) glDeleteTextures(1, &m_ColorAttachment);
        if (m_DepthAttachment) glDeleteRenderbuffers(1, &m_DepthAttachment);
    }

    void Framebuffer::CreateAsViewport()
    {
        Clear();

        glGenFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        uint32_t textureMapId;
        glGenTextures(1, &textureMapId);
        glBindTexture(GL_TEXTURE_2D, textureMapId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureMapId, 0);
		m_ColorAttachment = textureMapId;

        uint32_t depthMapId;
        glGenRenderbuffers(1, &depthMapId);
        glBindRenderbuffer(GL_RENDERBUFFER, depthMapId);
    	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthMapId);
		m_DepthAttachment = depthMapId;

        LOG_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::CreateAsDepthBuffer()
    {
        Clear();

        glGenFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        uint32_t depthMapId;
        glGenTextures(1, &depthMapId);
        glBindTexture(GL_TEXTURE_2D, depthMapId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        constexpr float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapId, 0);
		m_ColorAttachment = depthMapId;

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

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
            glCullFace(GL_BACK);
    }

    void Framebuffer::Resize(const int width, const int height)
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
