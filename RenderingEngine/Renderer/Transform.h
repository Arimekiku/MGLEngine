#pragma once

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace RenderingEngine
{
    class Transform
    {
    public:
        explicit Transform(const glm::vec3& initPos = glm::vec3(0),
                           const glm::vec3& initRot = glm::vec3(0),
                           const glm::vec3& initScale = glm::vec3(1));

        glm::vec3 Position{};
        glm::vec3 Rotation{};
        glm::vec3 Scale{};

        glm::mat4 GetTRSMatrix() const;

    private:
        static float DegToRad(const float& deg) { return glm::pi<float>() * deg / 180.0f; }
    };
}
