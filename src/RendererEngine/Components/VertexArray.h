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
        static void Unbind();

        void SetVertexBuffer(const Ref<VertexBuffer>& buffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);

        [[nodiscard]] const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        [[nodiscard]] const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        uint32_t m_RendererID = 0;

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}
