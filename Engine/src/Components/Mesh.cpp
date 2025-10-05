#include "Mesh.h"

#include <memory>

namespace Engine
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        SetVertices(&vertices[0].Position.x, vertices.size() * sizeof(Vertex));
        SetIndices(indices.data(), indices.size());
    }

    void Mesh::SetVertices(const float* vertices, size_t size)
    {
        const auto& vertexBuffer = std::make_shared<VertexBuffer>(vertices, size);

        const RenderBufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normals"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->SetVertexBuffer(vertexBuffer);
    }

    void Mesh::SetIndices(const uint32_t* indices, uint32_t count)
    {
        const auto& indexBuffer = std::make_shared<IndexBuffer>(indices, count);
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    Mesh::~Mesh()
    {
        m_VertexArray->Unbind();
    }
}
