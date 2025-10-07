#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Engine
{
    enum ImageTarget
    {
	    TEXTURE_2D,
		CUBEMAP
    };

    enum ImageFormat
    {
        INTEGER,
        VECTOR2,
        RGB,
        RGBA8,
        DEPTH24_STENCIL8,
    };

    struct ImageLayout
    {
        ImageLayout() = default;
		ImageLayout(int x, int y) : Width(x), Height(y) { }
		ImageLayout(int x, int y, int channels) : Width(x), Height(y), Channels(channels) {}

		ImageTarget Target = TEXTURE_2D;
		ImageFormat Format = RGBA8;

		int Width = 800, Height = 600;
        int Channels = 4;

        std::string Name = std::string();
    };

    class Image
    {
    public:
        Image() = delete;
        Image(const std::string& filePath);
        Image(const ImageLayout& layout);
        ~Image();

		ImageLayout& GetLayout() { return m_Layout; }
        const ImageLayout& GetLayout() const { return m_Layout; }

        unsigned char* GetPixels() const;
        void WritePixels(unsigned char* buffer);

    private:
        ImageLayout m_Layout;

        unsigned char* m_Buffer;
    };

    class Texture
    {
    public:
        Texture(const Image& image);
        Texture(const std::string& left, const std::string& right,
            const std::string& bottom, const std::string& top,
            const std::string& back, const std::string& front);
    	~Texture();

    	void Bind(uint8_t slot = 0) const;
    	void Unbind() const;

		const ImageLayout& GetLayout() const { return m_Layout; }
		ImageLayout GetLayout() { return m_Layout; }

        uint32_t GetRendererId() const { return m_RendererId; }

    private:
        ImageLayout m_Layout;

        uint32_t m_RendererId = 0;
    };
}
