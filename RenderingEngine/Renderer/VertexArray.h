#pragma once

#include "RenderBuffer.h"

namespace RenderingEngine
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();

        void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer);
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer);

        const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        uint32_t m_RendererID;

        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}
