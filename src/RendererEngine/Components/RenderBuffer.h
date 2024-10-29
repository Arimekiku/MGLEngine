#pragma once

#include "RendererEngine/Core/Logger/Log.h"

#include <glm/glm.hpp>

namespace RenderingEngine
{
    enum class ShaderDataType : uint8_t
    {
        None = 0,

        Bool,

        Int, 
        Int2,
        Int3, 
        Int4,

        Float, 
        Float2, 
        Float3, 
        Float4,

        Mat3x3, 
        Mat4x4,
    };

    static int32_t GetShaderDataTypeSize(const ShaderDataType dataType)
    {
        switch (dataType)
        {
            case ShaderDataType::Bool: return sizeof(bool);
            case ShaderDataType::Int: return sizeof(int);
            case ShaderDataType::Int2: return sizeof(int) * 2;
            case ShaderDataType::Int3: return sizeof(int) * 3;
            case ShaderDataType::Int4: return sizeof(int) * 4;
            case ShaderDataType::Float: return sizeof(float);
            case ShaderDataType::Float2: return sizeof(float) * 2;
            case ShaderDataType::Float3: return sizeof(float) * 3;
            case ShaderDataType::Float4: return sizeof(float) * 4;
            case ShaderDataType::Mat3x3: return sizeof(float) * 3 * 3;
            case ShaderDataType::Mat4x4: return sizeof(float) * 4 * 4;
            default: LOG_ASSERT(false, "None buffer type requested");
        }

        LOG_ASSERT(false, "Can't get shader data type");
        throw std::exception();
    }

    struct BufferAttribute
    {
        std::string Name;
        ShaderDataType Type = ShaderDataType::None;
        bool Normalized = false;
        int32_t Offset = 0;
        int32_t Size = 0;

        BufferAttribute() = default;
        BufferAttribute(const ShaderDataType type, const std::string& name, const bool normalized = false)
        {
            Name = name;
            Type = type;
            Normalized = normalized;
            Size = GetShaderDataTypeSize(type);
        }

        int32_t GetElementCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Bool: return 1;
                case ShaderDataType::Int: return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3x3: return 3 * 3;
                case ShaderDataType::Mat4x4: return 4 * 4;
                default: LOG_ASSERT(false, "None buffer type requested");
            }

            LOG_ASSERT(false, "Can't get shader data type");
            throw std::exception();
        }
    };

    class RenderBufferLayout
    {
    public:
        RenderBufferLayout() = default;

        RenderBufferLayout(const std::initializer_list<BufferAttribute>& attributes)
        {
            m_Elements = attributes;
            CalculateShaderDataOffset();
        }

        const std::vector<BufferAttribute>& GetElements() const { return m_Elements; }
        const int32_t& GetStride() const { return m_Stride; }

        std::vector<BufferAttribute>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferAttribute>::iterator end() { return m_Elements.end(); }
        std::vector<BufferAttribute>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferAttribute>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateShaderDataOffset()
        {
            int32_t offset = 0;
            for (auto& attr : m_Elements)
            {
                attr.Offset = offset;
                offset += attr.Size;
                m_Stride += attr.Size;
            }
        }

        std::vector<BufferAttribute> m_Elements;
        int32_t m_Stride = 0;
    };

    struct Vertex
    {
	    glm::vec3 Position;
	    glm::vec3 Normal;
	    glm::vec2 TexCoord;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer(const float* vertices, size_t size);
        ~VertexBuffer();

        const RenderBufferLayout& GetLayout() { return m_Layout; }
        void SetLayout(const RenderBufferLayout& layout) { m_Layout = layout; }

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_RendererID = 0;
        RenderBufferLayout m_Layout;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(const uint32_t* indices, int32_t count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        int32_t GetIndexCount() const { return m_IndexCount; }

    private:
        uint32_t m_RendererID = 0;
        int32_t m_IndexCount;
    };
}
