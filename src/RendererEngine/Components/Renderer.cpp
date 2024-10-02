#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    glm::mat4 Renderer::m_ProjViewMat;

    void Renderer::OnEveryUpdate(Camera& camera)
    {
        m_ProjViewMat = camera.GetProjViewMat();
    }

    void Renderer::Clear(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderModel(const Ref<Model>& model)
    {
        const auto& material = model->GetMaterial();
        material->Bind();

        material->GetShader()->BindUniformMat4("u_camMatrix", m_ProjViewMat);
        material->GetShader()->BindUniformMat4("u_trsMatrix", model->GetTRSMatrix());

        const auto& vertexArray = model->GetMesh()->GetVertexArray();
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::SetViewport(const int16_t x, const int16_t y, const int16_t width, const int16_t height)
    {
        glViewport(x, y, width, height);
    }
}
