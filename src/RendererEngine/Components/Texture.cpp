#include "Texture.h"
#include "RendererEngine/Core/Logger/Log.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace RenderingEngine
{
    Cubemap::Cubemap(std::vector<std::string>& faces)
	{
        m_Textures = faces;

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 1024, 1024);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                LOG_ERROR("Cubemap tex failed to load at path: {0}", faces[i]);
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

    Cubemap::~Cubemap()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Cubemap::Bind(const uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    }

    void Cubemap::Unbind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    Texture::Texture(const std::string& path)
    {
        m_FilePath = path;

        m_Buffer = stbi_load(path.c_str(), &m_Size.x, &m_Size.y, &m_BPP, 0);
        LOG_ASSERT(m_Buffer != nullptr, "Can't load texture from path");

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

    void Texture::Unbind() const
    {
        glBindTextureUnit(GL_TEXTURE_2D, 0);
    }
}
