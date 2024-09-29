#include "mxpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    static GLenum ShaderDataTypeToGLenum(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Bool: return GL_BOOL;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3x3: return GL_FLOAT;
            case ShaderDataType::Mat4x4: return GL_FLOAT;
            default: ;
        }

        LOG_CORE_ASSERT(false, "Can't get shader data type")
        return 0;
    }

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::SetVertexBuffer(const Ref<VertexBuffer>& buffer)
    {
        LOG_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Layout was not setted properly!")

        glBindVertexArray(m_RendererID);
        buffer->Bind();

        uint32_t index = 0;
        const auto& layout = buffer->GetLayout();
        for (const auto& attr : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  attr.GetElementCount(),
                                  ShaderDataTypeToGLenum(attr.Type),
                                  attr.Normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  reinterpret_cast<const void*>(attr.Offset));
            index++;
        }
        m_VertexBuffer = buffer;
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
    {
        glBindVertexArray(m_RendererID);

        m_IndexBuffer = buffer;
    }
}
