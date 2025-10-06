#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Engine
{
    class Image
    {
    public:
        Image(const std::string& fileName);
        ~Image();

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        int GetChannels() const { return m_Channels; }

        unsigned char* GetBuffer() const { return m_Buffer; }

    private:
        std::string m_FilePath;

        int m_Width;
        int m_Height;
        int m_Channels;

        unsigned char* m_Buffer;
    };

    class Texture
    {
    public:
        Texture() = default;
        Texture(const std::string& path);
        virtual ~Texture();

        virtual void Bind(uint8_t slot = 0) const;
        virtual void Unbind() const;

        uint32_t GetRendererId() const { return m_RendererId; }

    protected:
        uint32_t m_RendererId = 0;
    };

    class Cubemap final : public Texture
    {
    public:
        Cubemap(const std::string& left, const std::string& right,
            const std::string& bottom, const std::string& top,
            const std::string& back, const std::string& front);
        ~Cubemap() override;

        void Bind(uint8_t slot = 0) const override;
        void Unbind() const override;
    };
}
