#pragma once

#include <glm/glm.hpp>

namespace RenderingEngine
{
    class Texture
    {
    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind(uint8_t slot = 0) const;
        void Unbind() const;

        uint32_t GetRendererID() const { return m_RendererID; }

    private:
        std::string m_FilePath;
        uint32_t m_RendererID = 0;
        unsigned char* m_Buffer = nullptr;
        glm::i32vec2 m_Size = glm::i32vec2(0, 0);
        int m_BPP = 0;
    };
}
