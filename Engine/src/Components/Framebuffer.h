#pragma once

#include <cstdint>

namespace Engine 
{
    enum class FramebufferType
    {
        Viewport,
        DepthBuffer
    };

    class Framebuffer 
    {
    public:
        Framebuffer() { Resize(m_Width, m_Height); }
        Framebuffer(int width, int height, FramebufferType type);
        ~Framebuffer();

        void Bind() const;
        void Unbind() const;

        uint32_t GetColorAttachment() const { return m_ColorAttachment; }
		uint32_t GetDepthAttachment() const { return m_DepthAttachment; }

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        void Resize(int width, int height);

    private:
        void CreateAsViewport();
        void CreateAsDepthBuffer();
        void Clear();

        int m_Width = 800, m_Height = 600;
        uint32_t m_RendererId = 0;
        uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
        FramebufferType m_Type = FramebufferType::Viewport;
    };
}
