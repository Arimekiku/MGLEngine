#pragma once

#include <cstdint>
#include <vector>

namespace Engine 
{
    enum FramebufferAttachmentFormat
    {
        RGBA8,
        DEPTH24_STENCIL8,
	};

    struct FramebufferAttachment
    {
        FramebufferAttachment() = default;
		FramebufferAttachment(const FramebufferAttachmentFormat format) : Format(format) { }

		FramebufferAttachmentFormat Format;
    };

    class Framebuffer 
    {
    public:
        Framebuffer();
        Framebuffer(int width, int height);
        ~Framebuffer();

        void ConfigureAttachments(std::vector<FramebufferAttachment> attachments);
        uint32_t GetAttachment(FramebufferAttachmentFormat format) const;

        void Bind() const;
        void Unbind() const;

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        void Resize(int width, int height);

    private:
        void Create();
        void Clear();

        int m_Width = 800, m_Height = 600;
        uint32_t m_RendererId = 0;

		std::vector<FramebufferAttachment> m_Attachments;
        std::vector<uint32_t> m_AttachmentIds;
    };
}
