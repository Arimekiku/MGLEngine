#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace RenderingEngine
{
	class Mesh
    {
    public:
        Mesh();
        ~Mesh();

        void SetVertices(const std::vector<Vertex>& vertices);
        void SetIndices(const std::vector<uint32_t>& indices);

        const std::vector<Vertex>& GetVertices() { return m_Vertices; }
        const std::vector<uint32_t>& GetIndices() { return m_Indices; }

        Ref<VertexArray> GetVertexArray() { return m_VertexArray; }

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;

        Ref<VertexArray> m_VertexArray;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<VertexBuffer> m_VertexBuffer;
    };
}
