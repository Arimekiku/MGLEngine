#pragma once

#include <cstdint>
#include <vector>

#include "Core.h"
#include "Texture.h"

namespace Engine 
{
    struct FramebufferLayout
    {
		FramebufferLayout() = default;
        FramebufferLayout(int width, int height) : Width(width), Height(height) { }
		FramebufferLayout(const std::initializer_list<Ptr<Texture>>& buffers) : Attachments(buffers) { }

        int Width = 800, Height = 600;
        std::vector<Ptr<Texture>> Attachments;

		std::string Name = "Framebuffer";
    };

    class Framebuffer 
    {
    public:
        Framebuffer(const FramebufferLayout& layout);
        ~Framebuffer();

        void ConfigureAttachments(std::vector<Ptr<Texture>> attachments);
        void ClearAttachment(ImageFormat format, int clearValue) const;
        uint32_t GetAttachment(ImageFormat format) const;

        void Bind() const;
        void Unbind() const;

        int GetWidth() const { return m_Layout.Width; }
        int GetHeight() const { return m_Layout.Height; }
        int GetPixel(uint32_t index, int x, int y) const;

        void Resize(int width, int height);

    private:
        void Create();
        void Clear();

        FramebufferLayout m_Layout;

        uint32_t m_RendererId = 0;
    };
}
