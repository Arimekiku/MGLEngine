#pragma once

#include "Camera.h"
#include "VertexArray.h"

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
        static void RenderIndexed(const std::shared_ptr<VertexArray>& vertices, const std::shared_ptr<Shader>& shader,
                                  const glm::mat4& trsMatrix = glm::mat4(1.0f));

    private:
        static RenderData s_RenderData;
    };
}
