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
        static void CreateWorld(const std::shared_ptr<Camera>& camera);

        static void Clear(glm::vec4 color);
        static void RenderIndexed(const std::shared_ptr<VertexArray>& vertices, const std::shared_ptr<Shader>& shader);

    private:
        static RenderData s_RenderData;
    };
}
