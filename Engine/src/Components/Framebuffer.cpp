#include "Framebuffer.h"

#include <utility>
#include <glad/glad.h>

#include "Core/Logger/Log.h"

namespace Engine 
{
    std::string FramebufferAttachmentFormatToString(FramebufferAttachmentFormat format)
    {
	    switch (format)
	    {
	    case RGBA8:
            return "RGBA8";
	    case DEPTH24_STENCIL8:
			return "DEPTH24_STENCIL8";
	    }

        LOG_ERROR("Invalid attachment format parsed");
    }

    Framebuffer::Framebuffer()
    {
        m_Attachments = { DEPTH24_STENCIL8 };

        Resize(m_Width, m_Height);
    }

    Framebuffer::Framebuffer(const int width, const int height)
    {
        m_Attachments = { DEPTH24_STENCIL8 };

        Resize(width, height);
    }

    Framebuffer::~Framebuffer()
    {
        Clear();
    }

    void Framebuffer::Create()
    {
        Clear();

        glGenFramebuffers(1, &m_RendererId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

        if (m_Attachments.empty() == false)
        {
            int offset = 0;

            m_AttachmentIds.resize(m_Attachments.size());
            glGenTextures(m_AttachmentIds.size(), m_AttachmentIds.data());
            for (size_t i = 0; i < m_Attachments.size(); i++)
            {
                const uint32_t attachmentId = m_AttachmentIds[i];
                glBindTexture(GL_TEXTURE_2D, attachmentId);

                const auto& attachment = m_Attachments[i];
                switch (attachment.Format)
                {
	                case RGBA8:
	                {
	                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + offset, GL_TEXTURE_2D, attachmentId, 0);
	                    break;
	                }
	                case DEPTH24_STENCIL8:
	                {
	                    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachmentId, 0);

                        offset--;
	                	break;
	                }
					case INTEGER:
	                {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Width, m_Height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + offset, GL_TEXTURE_2D, attachmentId, 0);
                        break;
	                }
	                default:
	                {
	                    LOG_ERROR("Unsupported framebuffer attachment format: {0}", static_cast<int>(attachment.Format));
	                    break;
	                }
                }

                offset++;
            }
        }

        if (m_Attachments.size() > 1)
        {
	        constexpr uint32_t buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_Attachments.size(), buffers);
        }
        else if (m_Attachments.size() == 1 && m_Attachments[0].Format == DEPTH24_STENCIL8)
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

		const auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        LOG_ASSERT((result == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::ConfigureAttachments(std::vector<FramebufferAttachment> attachments)
    {
        m_Attachments = std::vector(std::move(attachments));

        Create();
    }

    void Framebuffer::ClearAttachment(const FramebufferAttachmentFormat format, const int clearValue) const
    {
        const auto attachment = GetAttachment(format);

        glClearTexImage(attachment, 0, GL_RED_INTEGER, GL_INT, &clearValue);
    }

    uint32_t Framebuffer::GetAttachment(const FramebufferAttachmentFormat format) const
    {
        for (int i = 0; i < m_Attachments.size(); i++)
        {
            if (m_Attachments[i].Format != format)
                continue;

            return m_AttachmentIds[i];
		}

        LOG_ERROR("Couldn't retrieve requested framebuffer attachment: {0}", FramebufferAttachmentFormatToString(format));
        return -1;
    }

    void Framebuffer::Clear()
    {
        if (m_RendererId == 0)
            return;

        m_AttachmentIds.clear();

        glDeleteFramebuffers(1, &m_RendererId);
        glDeleteTextures(m_AttachmentIds.size(), m_AttachmentIds.data());
    }

    void Framebuffer::Bind() const
    {
        glViewport(0, 0, m_Width, m_Height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int Framebuffer::GetPixel(uint32_t index, int x, int y) const
    {
        LOG_ASSERT(index < m_Attachments.size(), "Framebuffer. Invalid attachment index!");

        int pixel;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);

        return pixel;
    }

    void Framebuffer::Resize(const int width, const int height)
    {
        m_Width = width; m_Height = height;

        Create();
    }
}
