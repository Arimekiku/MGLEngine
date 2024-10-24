#pragma once

#include <glm/fwd.hpp>

namespace RenderingEngine 
{
    class Math 
    {
    public:
        static bool Decompose(glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
    };
}