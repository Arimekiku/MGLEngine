#pragma once

#include "RendererEngine/Core.h"
#include "RenderBuffer.h"

namespace RenderingEngine
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void SetVertexBuffer(const Ref<VertexBuffer>& buffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);

        const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        uint32_t m_RendererID = 0;

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}
