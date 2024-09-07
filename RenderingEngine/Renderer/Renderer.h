#pragma once

#include "Core.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

namespace RenderingEngine
{
    struct RenderData
    {
        glm::mat4 ProjViewMat;

        Ref<IndexBuffer> QuadIndexBuffer;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<VertexArray> QuadVertexArray;

        Ref<IndexBuffer> CubeIndexBuffer;
        Ref<VertexBuffer> CubeVertexBuffer;
        Ref<VertexArray> CubeVertexArray;
    };

    class Renderer
    {
    public:
        static void Initialize();
        static void CreateWorld(Camera& camera);

        static void Clear(glm::vec4 color);

        static void RenderQuad(const Ref<Shader>& shader, const glm::mat4& trsMatrix = glm::mat4(1.0f));

        static void RenderCube(const Ref<Shader>& shader, const glm::mat4& trsMatrix = glm::mat4(1.0f));

        static void RenderIndexed(const Ref<VertexArray>& vertices,
                                  const Ref<Shader>& shader,
                                  const glm::mat4& trsMatrix = glm::mat4(1.0f));

    private:
        static RenderData s_RenderData;
    };
}
