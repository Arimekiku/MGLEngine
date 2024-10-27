#pragma once

#include <glm/glm.hpp>

namespace RenderingEngine 
{
    class Math 
    {
    public:
        static bool Decompose(glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
    };

    class Vector3
    {
    public:
        static glm::vec3 Zero() { return glm::vec3(0.0f); }
        static glm::vec3 Right() { return glm::vec3(1.0f, 0.0f, 0.0f); }
        static glm::vec3 Up() { return glm::vec3(0.0f, 1.0f, 0.0f); }
        static glm::vec3 Forward() { return glm::vec3(0.0f, 0.0f, 1.0f); }
        static glm::vec3 One() { return glm::vec3(1.0f); }
    };
}