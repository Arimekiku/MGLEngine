#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    RenderData Renderer::s_RenderData = RenderData();

    void Renderer::CreateWorld(const std::shared_ptr<Camera>& camera)
    {
        s_RenderData.ProjViewMat = camera->GetProjViewMat();
    }

    void Renderer::Clear(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderIndexed(const std::shared_ptr<VertexArray>& vertices, const std::shared_ptr<Shader>& shader)
    {
        shader->Bind();
        shader->BindUniformMat4("u_camMatrix", s_RenderData.ProjViewMat);

        vertices->Bind();
        glDrawElements(GL_TRIANGLES, vertices->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }
}
