#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

#include <memory>

namespace RenderingEngine
{
    RenderData Renderer::s_RenderData = RenderData();

    void Renderer::Initialize()
    {
        //-------------------- QUAD --------------------
        s_RenderData.QuadMesh = std::make_shared<Mesh>();

        constexpr float quadVertex[4 * 9] = {
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
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

        constexpr float cubeVertex[8 * 9] = {
            -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f
        };
        s_RenderData.CubeMesh->SetVertices(cubeVertex, sizeof(cubeVertex));

        constexpr uint32_t cubeIndices[6 * 6] =
        {
            0, 1, 2, 2, 3, 0, //FRONT
            1, 5, 6, 6, 2, 1, //RIGHT
            5, 4, 7, 7, 6, 5, //BACK
            4, 0, 3, 3, 7, 4, //LEFT
            4, 5, 1, 1, 0, 4, //BOTTOM
            3, 2, 6, 6, 7, 3, //TOP
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

    void Renderer::RenderQuad(const Ref<Shader>& shader, const glm::mat4& trsMatrix)
    {
        RenderIndexed(s_RenderData.QuadMesh->GetVertexArray(), shader, trsMatrix);
    }

    void Renderer::RenderCube(const Ref<Shader>& shader, const glm::mat4& trsMatrix)
    {
        RenderIndexed(s_RenderData.CubeMesh->GetVertexArray(), shader, trsMatrix);
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
