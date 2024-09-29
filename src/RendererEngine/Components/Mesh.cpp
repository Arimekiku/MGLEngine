#include "mxpch.h"
#include "Mesh.h"

namespace RenderingEngine
{
    Mesh::Mesh()
    {
        m_VertexArray = std::make_shared<VertexArray>();
    }

    void Mesh::SetVertices(const float* vertices, int size)
    {
        m_VertexBuffer = std::make_shared<VertexBuffer>(vertices, size);

        const RenderBufferLayout cubeLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float3, "a_Normals"}
        };
        m_VertexBuffer->SetLayout(cubeLayout);
        m_VertexArray->SetVertexBuffer(m_VertexBuffer);
    }

    void Mesh::SetIndices(const uint32_t* indices, int count)
    {
        m_IndexBuffer = std::make_shared<IndexBuffer>(indices, count);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    Mesh::~Mesh()
    {
        m_IndexBuffer->Unbind();
        m_VertexBuffer->Unbind();
        m_VertexArray->Unbind();
    }
}
