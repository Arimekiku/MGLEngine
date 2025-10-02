#pragma once

#include <string>
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

        uint32_t GetRendererId() const { return m_RendererId; }

    private:
        std::string m_FilePath;
        uint32_t m_RendererId = 0;
        unsigned char* m_Buffer = nullptr;
        glm::i32vec2 m_Size = glm::i32vec2(0, 0);
        int m_Bpp = 0;
    };
}
