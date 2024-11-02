#pragma once

namespace RenderingEngine 
{
    enum class FramebufferType
    {
        Viewport,
        DepthBuffer
    };

    class Framebuffer 
    {
    public:
        Framebuffer() { Resize(m_Width, m_Height); };
        Framebuffer(int16_t width, int16_t height, FramebufferType type);
        ~Framebuffer();

        void Bind() const;
        void Unbind() const;

        uint32_t GetAttachment(int16_t index);
        int16_t GetWidth() const { return m_Width; }
        int16_t GetHeight() const { return m_Height; }

        void Resize(int16_t width, int16_t height);

    private:
        void CreateAsViewport();
        void CreateAsDepthBuffer();
        void Clear();

        int16_t m_Width = 800, m_Height = 600;
        uint32_t m_RendererID = 0;
        std::vector<uint32_t> m_Attachments;
        FramebufferType m_Type = FramebufferType::Viewport;
    };
}
