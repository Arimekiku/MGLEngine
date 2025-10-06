#include "Mesh.h"

namespace Engine
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        SetVertices(&vertices[0].Position.x, vertices.size() * sizeof(Vertex));
        SetIndices(indices.data(), indices.size());
    }

    void Mesh::SetVertices(const float* vertices, size_t size) const
    {
        const auto& vertexBuffer = Ptr<VertexBuffer>::Create(vertices, size);

        const RenderBufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normals"},
            {ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Int, "a_ID"},
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->SetVertexBuffer(vertexBuffer);
    }

    void Mesh::SetIndices(const uint32_t* indices, uint32_t count) const
    {
        const auto& indexBuffer = Ptr<IndexBuffer>::Create(indices, count);
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    Mesh::~Mesh()
    {
        m_VertexArray->Unbind();
    }
}
