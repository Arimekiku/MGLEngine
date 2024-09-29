#pragma once

namespace RenderingEngine {
    class Framebuffer {
    public:
        Framebuffer(int16_t width, int16_t height);
        ~Framebuffer();

        void Bind() const;
        static void Unbind();

        [[nodiscard]] uint32_t GetTextureAttachment() const { return m_TextureAttachment; }
        [[nodiscard]] int16_t GetWidth() const { return m_Width; }
        [[nodiscard]] int16_t GetHeight() const { return m_Height; }

        void Resize(int16_t width, int16_t height);

    private:
        int16_t m_Width, m_Height;
        uint32_t m_RendererID = 0;
        uint32_t m_TextureAttachment = 0;
        uint32_t m_DepthAttachment = 0;
    };
}
