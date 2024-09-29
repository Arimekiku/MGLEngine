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

        constexpr float quadVertex[4 * 9] = {
            -0.5f,  0.5f, 0.0f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,   1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f
        };
        s_RenderData.QuadMesh->SetVertices(quadVertex, sizeof(quadVertex));

        constexpr uint32_t quadIndices[6] =
        {
            0, 1, 2, 0, 2, 3,
        };
        s_RenderData.QuadMesh->SetIndices(quadIndices, 6);
        //----------------------------------------------
        //-------------------- CUBE --------------------
        s_RenderData.CubeMesh = std::make_shared<Mesh>();

        constexpr float cubeVertex[24 * 9] = {
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f, //FRONT
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f, //FRONT
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f, //FRONT
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f, //FRONT

            -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f, //BACK
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f, //BACK
             0.5f,  0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f, //BACK
            -0.5f,  0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f, //BACK

             0.5f, -0.5f,  0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, //RIGHT
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f, //RIGHT
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, //RIGHT
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, //RIGHT

            -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f, //LEFT
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f, //LEFT
            -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f, //LEFT
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f, //LEFT

            -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f, //BOTTOM
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f, //BOTTOM
             0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f, //BOTTOM
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f, //BOTTOM

            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f, //TOP
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f, //TOP
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f, //TOP
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f, //TOP
        };
        s_RenderData.CubeMesh->SetVertices(cubeVertex, sizeof(cubeVertex));

        constexpr uint32_t cubeIndices[6 * 6] =
        {
             0,  1,  2,  2,  3,  0, //FRONT
             5,  4,  7,  7,  6,  5, //BACK
             8,  9, 10, 10, 11,  8, //RIGHT
            12, 13, 14, 14, 15, 12, //LEFT
            16, 17, 18, 18, 19, 16, //BOTTOM
            20, 21, 22, 22, 23, 20, //TOP
        };
        s_RenderData.CubeMesh->SetIndices(cubeIndices, 6 * 6);
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

    void Renderer::RenderCube(const Ref<Material>& material, const glm::mat4& trsMatrix)
    {
        RenderIndexed(s_RenderData.CubeMesh->GetVertexArray(), material, trsMatrix);
    }

    void Renderer::RenderModel(const Ref<Model>& model)
    {
        RenderIndexed(
            model->GetMesh()->GetVertexArray(),
            model->GetMaterial(),
            model->GetTransform()->GetTRSMatrix());
    }

    void Renderer::RenderIndexed(const Ref<VertexArray>& vertices,
                                 const Ref<Material>& material,
                                 const glm::mat4& trsMatrix)
    {
        const auto shader = material->GetShader();
        shader->Bind();

        shader->BindUniformMat4("u_camMatrix", s_RenderData.ProjViewMat);
        shader->BindUniformMat4("u_trsMatrix", trsMatrix);

        material->Bind();
        vertices->Bind();
        glDrawElements(GL_TRIANGLES, vertices->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::SetViewport(const int16_t x, const int16_t y, const int16_t width, const int16_t height)
    {
        glViewport(x, y, width, height);
    }
}
