#pragma once

#include "Core.h"
#include "RenderBuffer.h"

namespace Engine
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void SetVertexBuffer(const Ptr<VertexBuffer>& buffer);
        void SetIndexBuffer(const Ptr<IndexBuffer>& buffer);

        const Ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        const Ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        uint32_t m_RendererId = 0;

        Ptr<VertexBuffer> m_VertexBuffer = nullptr;
        Ptr<IndexBuffer> m_IndexBuffer = nullptr;
    };
}
