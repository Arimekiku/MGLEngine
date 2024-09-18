#pragma once

#include "Renderer/Core.h"
#include "Camera.h"
#include "Model.h"

namespace RenderingEngine
{
    struct RenderData
    {
        glm::mat4 ProjViewMat;

        Ref<Mesh> QuadMesh;
        Ref<Mesh> CubeMesh;
    };

    class Renderer
    {
    public:
        static void Initialize();
        static void CreateWorld(Camera& camera);

        static void Clear(glm::vec4 color);

        static void RenderQuad(const Ref<Shader>& shader, const glm::mat4& trsMatrix = glm::mat4(1.0f));

        static void RenderCube(const Ref<Shader>& shader, const glm::mat4& trsMatrix = glm::mat4(1.0f));

        static void RenderModel(const Ref<Model>& model);

        static void RenderIndexed(const Ref<VertexArray>& vertices,
                                  const Ref<Shader>& shader,
                                  const glm::mat4& trsMatrix = glm::mat4(1.0f));

    private:
        static RenderData s_RenderData;
    };
}
