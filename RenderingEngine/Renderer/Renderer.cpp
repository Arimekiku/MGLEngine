#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    RenderData Renderer::s_RenderData = RenderData();

    void Renderer::Initialize()
    {
        s_RenderData.QuadVertexArray = std::make_shared<VertexArray>();

        constexpr float ver[4 * 7] = {
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };
        s_RenderData.QuadVertexBuffer = std::make_shared<VertexBuffer>(ver, sizeof(ver));

        const RenderBufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        s_RenderData.QuadVertexBuffer->SetLayout(layout);
        s_RenderData.QuadVertexArray->SetVertexBuffer(s_RenderData.QuadVertexBuffer);

        constexpr int count = 6;
        constexpr uint32_t indices[count] =
        {
            0, 1, 2,
            0, 2, 3,
        };
        s_RenderData.QuadIndexBuffer = std::make_shared<IndexBuffer>(indices, count);
        s_RenderData.QuadVertexArray->SetIndexBuffer(s_RenderData.QuadIndexBuffer);
    }

    void Renderer::CreateWorld(Camera& camera)
    {
        s_RenderData.ProjViewMat = camera.GetProjViewMat();
    }

    void Renderer::Clear(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderQuad(const Ref<Shader>& shader, const glm::mat4& trsMatrix)
    {
        RenderIndexed(s_RenderData.QuadVertexArray, shader, trsMatrix);
    }

    void Renderer::RenderIndexed(const Ref<VertexArray>& vertices,
                                 const Ref<Shader>& shader,
                                 const glm::mat4& trsMatrix)
    {
        shader->Bind();
        shader->BindUniformMat4("u_camMatrix", s_RenderData.ProjViewMat);
        shader->BindUniformMat4("u_trsMatrix", trsMatrix);

        vertices->Bind();
        glDrawElements(GL_TRIANGLES, vertices->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }
}
