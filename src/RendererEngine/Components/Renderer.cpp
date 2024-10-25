#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    glm::mat4 Renderer::m_ProjViewMat;

    void Renderer::OnEveryUpdate(Ref<Camera>& camera)
    {
        m_ProjViewMat = camera->GetProjViewMat();
    }

    void Renderer::Clear(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderMesh(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& TRSMatrix)
    {
        shader->Bind();
        shader->BindUniformMat4("u_camMatrix", m_ProjViewMat);
        shader->BindUniformMat4("u_trsMatrix", TRSMatrix);

        const auto& vertexArray = mesh->GetVertexArray();
        vertexArray->Bind();

        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::SetViewport(const int16_t x, const int16_t y, const int16_t width, const int16_t height)
    {
        glViewport(x, y, width, height);
    }
}
