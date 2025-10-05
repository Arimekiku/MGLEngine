#include "Texture.h"

#include <array>
#include <stb_image.h>
#include <glad/glad.h>

#include "Core/Logger/Log.h"

namespace Engine
{
    Texture::Texture(const std::string& path)
    {
		const auto image = Image(path);
		const auto size = glm::ivec2(image.GetWidth(), image.GetHeight());

        glGenTextures(1, &m_RendererId);
        glBindTexture(GL_TEXTURE_2D, m_RendererId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetBuffer());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererId);
    }

    void Texture::Bind(const uint8_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererId);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Cubemap::Cubemap(const std::string& left, const std::string& right, const std::string& bottom,
                     const std::string& top, const std::string& back, const std::string& front)
    {
        const std::array<Image, 6> faces = { right, left, top, bottom, front, back };

        glGenTextures(1, &m_RendererId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);

        for (GLuint i = 0; i < faces.size(); ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, faces[i].GetWidth(), faces[i].GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, faces[i].GetBuffer());

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    Cubemap::~Cubemap()
    {
		glDeleteTextures(1, &m_RendererId);
    }

    void Cubemap::Bind(const uint8_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);
    }

    void Cubemap::Unbind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    Image::Image(const std::string& fileName)
    {
        m_FilePath = fileName;

        m_Buffer = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_Channels, 0);
        LOG_ASSERT((m_Buffer != nullptr), "Can't load texture from path");
    }

    Image::~Image()
    {
		stbi_image_free(m_Buffer);
    }
}
