#include "mxpch.h"
#include "Mesh.h"

#include <memory>

namespace RenderingEngine
{
    Mesh::Mesh()
    {
        m_VertexArray = std::make_shared<VertexArray>();
    }

    void Mesh::SetVertices(const std::vector<Vertex>& vertices)
    {
        m_Vertices = vertices;
        m_VertexBuffer = std::make_shared<VertexBuffer>(vertices);

        const RenderBufferLayout cubeLayout = {
            {ShaderDataType::Float3, "a_Position"},
	        {ShaderDataType::Float3, "a_Normals"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };
        m_VertexBuffer->SetLayout(cubeLayout);
        m_VertexArray->SetVertexBuffer(m_VertexBuffer);
    }

    void Mesh::SetIndices(const std::vector<uint32_t>& indices)
    {
        m_Indices = indices;

        m_IndexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    Mesh::~Mesh()
    {
        m_IndexBuffer->Unbind();
        m_VertexBuffer->Unbind();
        m_VertexArray->Unbind();
    }
}
