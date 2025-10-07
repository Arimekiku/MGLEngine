#include "Framebuffer.h"

#include <utility>
#include <glad/glad.h>

#include "Core/Logger/Log.h"

namespace Engine 
{
    std::string ImageFormatToString(ImageFormat format)
    {
	    switch (format)
	    {
		    case RGBA8: return "RGBA8";
		    case DEPTH24_STENCIL8: return "DEPTH24_STENCIL8";
			default: break;
	    }

        LOG_ERROR("Invalid attachment format parsed");
    }

    Framebuffer::Framebuffer(const FramebufferLayout& layout)
    {
        m_Layout = layout;

		Create();
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

    	auto& attachments = m_Layout.Attachments;
        if (attachments.empty() == false)
        {
            int offset = 0;

            for (auto& attachment : attachments)
            {
            	auto newLayout = ImageLayout();
                newLayout.Width = m_Layout.Width;
				newLayout.Height = m_Layout.Height;
				newLayout.Format = attachment->GetLayout().Format;
				newLayout.Target = attachment->GetLayout().Target;
				newLayout.Channels = attachment->GetLayout().Channels;
				newLayout.Name = attachment->GetLayout().Name;

                attachment = Ptr<Texture>::Create(newLayout);
                const uint32_t attachmentId = attachment->GetRendererId();

                glBindTexture(GL_TEXTURE_2D, attachmentId);
                switch (attachment->GetLayout().Format)
                {
	                case INTEGER:
	                case VECTOR2:
	                case RGB:
	                case RGBA8:
	                {
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + offset, GL_TEXTURE_2D, attachmentId, 0);
	                    break;
	                }
	                case DEPTH24_STENCIL8:
	                {
	                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachmentId, 0);

                        offset--;
	                	break;
	                }
	                default:
	                {
	                    LOG_ERROR("Unsupported framebuffer attachment format: {0}", ImageFormatToString(attachment->GetLayout().Format));
	                    break;
	                }
                }

                offset++;
            }
        }

        if (attachments.size() > 1)
        {
	        constexpr uint32_t buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(attachments.size(), buffers);
        }
        else if (attachments.size() == 1 && attachments[0]->GetLayout().Format == DEPTH24_STENCIL8)
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

		const auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        LOG_ASSERT((result == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is invalid!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::ConfigureAttachments(std::vector<Ptr<Texture>> attachments)
    {
        m_Layout.Attachments = std::vector(std::move(attachments));

        Create();
    }

    void Framebuffer::ClearAttachment(ImageFormat format, int clearValue) const
    {
        const auto attachment = GetAttachment(format);

        glClearTexImage(attachment, 0, GL_RED_INTEGER, GL_INT, &clearValue);
    }

    uint32_t Framebuffer::GetAttachment(ImageFormat format) const
    {
        for (const auto& attachment : m_Layout.Attachments)
        {
            if (attachment->GetLayout().Format != format)
                continue;

            return attachment->GetRendererId();
		}

        LOG_ERROR("Couldn't retrieve requested framebuffer attachment: {0}", ImageFormatToString(format));
        return -1;
    }

    void Framebuffer::Clear()
    {
        if (m_RendererId == 0)
            return;

        glDeleteFramebuffers(1, &m_RendererId);
    }

    void Framebuffer::Bind() const
    {
        glViewport(0, 0, m_Layout.Width, m_Layout.Height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int Framebuffer::GetPixel(const uint32_t index, int x, int y) const
    {
        LOG_ASSERT(index < m_Layout.Attachments.size(), "Framebuffer. Invalid attachment index!");

        int pixel;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);

        return pixel;
    }

    void Framebuffer::Resize(const int width, const int height)
    {
        m_Layout.Width = width;
    	m_Layout.Height = height;

        Create();
    }
}
