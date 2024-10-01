#include "mxpch.h"
#include "Mesh.h"

#include <memory>

namespace RenderingEngine
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        SetVertices(&vertices[0].Position.x, vertices.size() * sizeof(Vertex));
        SetIndices(indices.data(), indices.size());
    }

    void Mesh::SetVertices(const float* vertices, size_t size)
    {
        const auto& m_VertexBuffer = std::make_shared<VertexBuffer>(vertices, size);

        const RenderBufferLayout cubeLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normals"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };
        m_VertexBuffer->SetLayout(cubeLayout);
        m_VertexArray->SetVertexBuffer(m_VertexBuffer);
    }

    void Mesh::SetIndices(const uint32_t* indices, uint32_t count)
    {
        const auto& m_IndexBuffer = std::make_shared<IndexBuffer>(indices, count);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    Mesh::~Mesh()
    {
        m_VertexArray->Unbind();
    }
}
