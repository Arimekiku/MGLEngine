#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    RenderData Renderer::s_RenderData = RenderData();

    void Renderer::Initialize()
    {
        //-------------------- QUAD --------------------
        s_RenderData.QuadMesh = std::make_shared<Mesh>();

        std::vector<Vertex> quadVertex;
        quadVertex.resize(4);
        quadVertex[0] = { glm::vec3(-0.5, 0.5, 0), glm::vec3(0, 0, 1), glm::vec2(1, 0) };
        quadVertex[1] = { glm::vec3(-0.5, -0.5, 0), glm::vec3(0, 0, 1), glm::vec2(1, 1) };
        quadVertex[2] = { glm::vec3(0.5, -0.5, 0), glm::vec3(0, 0, 1), glm::vec2(0, 1) };
        quadVertex[3] = { glm::vec3(0.5, 0.5, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0) };
        s_RenderData.QuadMesh->SetVertices(quadVertex);

        const std::vector<uint32_t> quadIndices = { 0, 1, 2, 0, 2, 3, };
        s_RenderData.QuadMesh->SetIndices(quadIndices);
        //----------------------------------------------
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

    void Renderer::RenderQuad(const Ref<Material>& material, const glm::mat4& trsMatrix)
    {
        RenderIndexed(s_RenderData.QuadMesh->GetVertexArray(), material, trsMatrix);
    }

    void Renderer::RenderModel(const Ref<Model>& model)
    {
	    RenderIndexed(
            model->GetMesh()->GetVertexArray(),
            model->GetMaterial(),
            model->GetTRSMatrix());
    }

    void Renderer::RenderIndexed(const Ref<VertexArray>& vertices,
                                 const Ref<Material>& material,
                                 const glm::mat4& trsMatrix)
    {
        const auto& shader = material->GetShader();
        material->Bind();

        shader->BindUniformMat4("u_camMatrix", s_RenderData.ProjViewMat);
        shader->BindUniformMat4("u_trsMatrix", trsMatrix);

        vertices->Bind();
        glDrawElements(GL_TRIANGLES, vertices->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::SetViewport(const int16_t x, const int16_t y, const int16_t width, const int16_t height)
    {
        glViewport(x, y, width, height);
    }
}
