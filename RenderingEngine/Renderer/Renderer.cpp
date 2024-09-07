#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    RenderData Renderer::s_RenderData = RenderData();

    void Renderer::Initialize()
    {
        //-------------------- QUAD --------------------
        s_RenderData.QuadVertexArray = std::make_shared<VertexArray>();

        constexpr float quadVertex[4 * 7] = {
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };
        s_RenderData.QuadVertexBuffer = std::make_shared<VertexBuffer>(quadVertex, sizeof(quadVertex));

        const RenderBufferLayout quadLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        s_RenderData.QuadVertexBuffer->SetLayout(quadLayout);
        s_RenderData.QuadVertexArray->SetVertexBuffer(s_RenderData.QuadVertexBuffer);

        constexpr int quadIndicesCount = 6;
        constexpr uint32_t quadIndices[quadIndicesCount] =
        {
            0, 1, 2,
            0, 2, 3,
        };
        s_RenderData.QuadIndexBuffer = std::make_shared<IndexBuffer>(quadIndices, quadIndicesCount);
        s_RenderData.QuadVertexArray->SetIndexBuffer(s_RenderData.QuadIndexBuffer);
        //----------------------------------------------
        //-------------------- CUBE --------------------
        s_RenderData.CubeVertexArray = std::make_shared<VertexArray>();

        constexpr float cubeVertex[8 * 7] = {
            -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f
        };
        s_RenderData.CubeVertexBuffer = std::make_shared<VertexBuffer>(cubeVertex, sizeof(cubeVertex));

        const RenderBufferLayout cubeLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        s_RenderData.CubeVertexBuffer->SetLayout(cubeLayout);
        s_RenderData.CubeVertexArray->SetVertexBuffer(s_RenderData.CubeVertexBuffer);

        constexpr int cubeIndicesCount = 6 * 6;
        constexpr uint32_t cubeIndices[cubeIndicesCount] =
        {
            0, 1, 2, 2, 3, 0, //FRONT
            1, 5, 6, 6, 2, 1, //RIGHT
            5, 4, 7, 7, 6, 5, //BACK
            4, 0, 3, 3, 7, 4, //LEFT
            4, 5, 1, 1, 0, 4, //BOTTOM
            3, 2, 6, 6, 7, 3, //TOP
        };
        s_RenderData.CubeIndexBuffer = std::make_shared<IndexBuffer>(cubeIndices, cubeIndicesCount);
        s_RenderData.CubeVertexArray->SetIndexBuffer(s_RenderData.CubeIndexBuffer);
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
        RenderIndexed(s_RenderData.QuadVertexArray, shader, trsMatrix);
    }

    void Renderer::RenderCube(const Ref<Shader>& shader, const glm::mat4& trsMatrix)
    {
        RenderIndexed(s_RenderData.CubeVertexArray, shader, trsMatrix);
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
