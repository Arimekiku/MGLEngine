#include "Renderer.h"

#include <glad/glad.h>

namespace Engine
{
    glm::mat4 Renderer::m_ProjViewMat;

    void Renderer::Initialize()
    {

    }

    void Renderer::UpdateCameraMatrix(const glm::mat4& cameraProjView)
    {
        m_ProjViewMat = cameraProjView;
    }

    void Renderer::Clear(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderMesh(const Ptr<Mesh>& mesh, const Ptr<Shader>& shader, const glm::mat4& trsMatrix)
    {
        shader->Bind();
        shader->BindUniformMat4("u_camMatrix", m_ProjViewMat);
        shader->BindUniformMat4("u_trsMatrix", trsMatrix);

        const auto& vertexArray = mesh->GetVertexArray();
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();

        shader->Unbind();
    }

    void Renderer::RenderCubeMap(const Ptr<Mesh>& mesh, const Ptr<Shader>& shader, const Ptr<Cubemap>& cubemap, const glm::mat4& trsMatrix)
    {
        glCullFace(GL_FRONT);
        shader->Bind();
        shader->BindUniformMat4("u_camMatrix", m_ProjViewMat * trsMatrix);

        cubemap->Bind();
        shader->BindUniformInt1("u_Skybox", 0);

        const auto& vertexArray = mesh->GetVertexArray();
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
        vertexArray->Unbind();

        shader->Unbind();
        glCullFace(GL_BACK);
    }

    void Renderer::SetViewport(const int16_t x, const int16_t y, const int16_t width, const int16_t height)
    {
        glViewport(x, y, width, height);
    }
}
