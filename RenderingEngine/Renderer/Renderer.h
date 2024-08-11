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

        explicit RenderData(const glm::mat4& projViewMat = glm::mat4(1.0f))
        {
            ProjViewMat = projViewMat;
        }
    };

    class Renderer
    {
    public:
        static void CreateWorld(Camera& camera);

        static void Clear(glm::vec4 color);

        static void RenderIndexed(const Ref<VertexArray>& vertices,
                                  const Ref<Shader>& shader,
                                  const glm::mat4& trsMatrix = glm::mat4(1.0f));

    private:
        static RenderData s_RenderData;
    };
}
