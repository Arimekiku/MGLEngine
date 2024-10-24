#include "Texture.h"

#include "RendererEngine/Core/Logger/Log.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RenderingEngine
{
    Texture::Texture(const std::string& path)
    {
        m_FilePath = path;

        m_Buffer = stbi_load(path.c_str(), &m_Size.x, &m_Size.y, &m_BPP, 0);
        LOG_CORE_ASSERT(m_Buffer != nullptr, "Can't load texture from path");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Size.x, m_Size.y);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Size.x, m_Size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(m_Buffer);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::Bind(const uint8_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

    void Texture::Unbind()
    {
        glBindTextureUnit(GL_TEXTURE_2D, 0);
    }
}
