#pragma once

#include <glm/glm.hpp>

namespace RenderingEngine
{
    class Transform
    {
    public:
        explicit Transform(const glm::vec3& initPos = glm::vec3(0),
                           const glm::vec3& initRot = glm::vec3(0),
                           const glm::vec3& initScale = glm::vec3(1));

        glm::vec3 Position = glm::vec3(0, 0, 0);
        glm::vec3 Rotation = glm::vec3(0, 0, 0);
        glm::vec3 Scale = glm::vec3(1, 1, 1);

        glm::mat4& GetTRSMatrix();

    private:
        glm::mat4 m_TRSMatrix;
    };
}
