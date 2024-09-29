#pragma once

#include "Transform.h"
#include "RendererEngine/Core.h"

namespace RenderingEngine
{
    class AreaLighting
    {
    public:
        explicit AreaLighting(glm::vec3 position);
        ~AreaLighting() = default;

        Ref<Transform> GetTransform() { return m_Position; }

        glm::vec3 Color = glm::vec3(1, 1, 1);

    private:
        Ref<Transform> m_Position = std::make_shared<Transform>();
    };
}