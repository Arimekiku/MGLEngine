#pragma once

#include "VertexArray.h"

namespace RenderingEngine
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh();

        void SetVertices(const float* vertices, int size);
        void SetIndices(const uint32_t* indices, int count);

        Ref<VertexArray> GetVertexArray() { return m_VertexArray; }

    private:
        Ref<VertexArray> m_VertexArray;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<VertexBuffer> m_VertexBuffer;
    };
}
