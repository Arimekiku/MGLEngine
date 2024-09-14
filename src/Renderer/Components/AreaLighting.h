#pragma once

#include "Transform.h"
#include "Renderer/Core.h"

namespace RenderingEngine
{
    class AreaLighting
    {
    public:
        explicit AreaLighting(glm::vec3 position);
        ~AreaLighting() = default;

        Ref<Transform> GetTransform() { return m_Position; }

        glm::vec4 Color = glm::vec4(1, 1, 1, 1);

    private:
        Ref<Transform> m_Position = std::make_shared<Transform>();
    };
}
