#include "Texture.h"

#include <array>
#include <stb_image.h>
#include <glad/glad.h>

#include "Core/Logger/Log.h"

namespace Engine
{
    GLenum FromImageTargetToGLTarget(const ImageTarget target)
    {
        switch (target)
        {
	        case TEXTURE_2D: return GL_TEXTURE_2D;
	        case CUBEMAP: return GL_TEXTURE_CUBE_MAP;
			default: break;
        }

        LOG_CRITICAL("Requested bad Image Target");
    }

    GLenum FromImageFormatToGLInternalFormat(const ImageFormat format)
    {
        switch (format)
        {
	        case INTEGER: return GL_R32I;
	        //case VECTOR2: return GL_RG;
	        case RGB: return GL_RGB;
	        case RGBA8: return GL_RGBA8;
	        case DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
	        default: break;
        }

        LOG_CRITICAL("Requested bad Image Format");
    }

    GLenum FromImageFormatToGLFormat(const ImageFormat format)
    {
       switch (format)
       {
	       case INTEGER: return GL_RED_INTEGER;
	       case VECTOR2: return GL_RG;
	       case RGB: return GL_RGB;
	       case RGBA8: return GL_RGBA;
	       case DEPTH24_STENCIL8: return GL_DEPTH_STENCIL;
	       default: break;
       }

       LOG_CRITICAL("Requested bad Image Format");
	}

    GLenum FromImageFormatToGLPixelType(const ImageFormat format)
    {
        switch (format)
        {
        case INTEGER: return GL_UNSIGNED_BYTE;
        case VECTOR2: return GL_UNSIGNED_BYTE;
        case RGB: return GL_UNSIGNED_BYTE;
        case RGBA8: return GL_UNSIGNED_BYTE;
        case DEPTH24_STENCIL8: return GL_UNSIGNED_INT_24_8;
        default: break;
        }

        LOG_CRITICAL("Requested bad Image Format");
    }

    Image::Image(const std::string& filePath)
    {
        m_Buffer = stbi_load(filePath.c_str(), &m_Layout.Width, &m_Layout.Height, &m_Layout.Channels, 0);

        m_Layout.Name = filePath;
		m_Layout.Target = TEXTURE_2D;

        switch (m_Layout.Channels)
        {
			case 1: m_Layout.Format = INTEGER; break;
			case 2: m_Layout.Format = VECTOR2; break;
			case 3: m_Layout.Format = RGB; break;
			case 4: m_Layout.Format = RGBA8; break;
	        default: LOG_ASSERT(false, "Can't load image format properly");
        }

        LOG_ASSERT((m_Buffer != nullptr), "Can't load texture from path");
    }

    Image::Image(const ImageLayout& layout)
    {
        m_Layout = layout;
        m_Buffer = nullptr;
    }

    Image::~Image()
    {
		if (m_Buffer == nullptr)
            return;

		stbi_image_free(m_Buffer);
    }

    void Image::WritePixels(unsigned char* buffer)
    {
        if (m_Buffer != nullptr)
            LOG_WARN("Attempting to write pixels into existing buffer!");

        m_Buffer = buffer;
    }

    unsigned char* Image::GetPixels() const
    {
        return m_Buffer;
    }

    Texture::Texture(const Image& image)
    {
        const auto& layout = image.GetLayout();

        const auto target = FromImageTargetToGLTarget(layout.Target);
        const auto internalFormat = FromImageFormatToGLInternalFormat(layout.Format);
        const auto format = FromImageFormatToGLFormat(layout.Format);
        const auto pixelType = FromImageFormatToGLPixelType(layout.Format);

        glGenTextures(1, &m_RendererId);
        glBindTexture(target, m_RendererId);
        glTexImage2D(target, 0, internalFormat, layout.Width, layout.Height, 0, format, pixelType, image.GetPixels());

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(target, 0);

        m_Layout = layout;
    }

    Texture::Texture(const std::string& left, const std::string& right, const std::string& bottom,
        const std::string& top, const std::string& back, const std::string& front)
    {
        const std::array<Image, 6> faces = { right, left, top, bottom, front, back };

        glGenTextures(1, &m_RendererId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);

        for (GLuint i = 0; i < faces.size(); ++i)
        {
			const auto& layout = faces[i].GetLayout();

			const auto format = FromImageFormatToGLFormat(layout.Format);
            const auto internalFormat = FromImageFormatToGLInternalFormat(layout.Format);
            const auto pixelType = FromImageFormatToGLPixelType(layout.Format);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, layout.Width, layout.Height, 0, format, pixelType, faces[i].GetPixels());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        m_Layout = ImageLayout();
		m_Layout.Target = CUBEMAP;
		m_Layout.Width = faces[0].GetLayout().Width;
		m_Layout.Height = faces[0].GetLayout().Height;
		m_Layout.Channels = faces[0].GetLayout().Channels;
		m_Layout.Format = faces[0].GetLayout().Format;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererId);
    }

    void Texture::Bind(const uint8_t slot) const
    {
        const auto target = FromImageTargetToGLTarget(m_Layout.Target);

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(target, m_RendererId);
    }

    void Texture::Unbind() const
    {
        const auto target = FromImageTargetToGLTarget(m_Layout.Target);

        glBindTexture(target, 0);
    }
}
