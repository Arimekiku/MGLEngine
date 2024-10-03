#pragma once

#include "Transform.h"
#include "RendererEngine/Core.h"

namespace RenderingEngine
{
    class AreaLighting
    {
    public:
        AreaLighting(glm::vec3 position) : Position(position) { }
        ~AreaLighting() = default;

        glm::vec3 Color = glm::vec3(1, 1, 1);
        glm::vec3 Position = glm::vec3(0, 0, 0);
    };
}
