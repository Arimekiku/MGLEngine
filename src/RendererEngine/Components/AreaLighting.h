#pragma once

#include "Transform.h"
#include "RendererEngine.h"

namespace RenderingEngine
{
    class AreaLighting
    {
    public:
        AreaLighting() = default;
        AreaLighting(glm::vec3 position, const Ref<Model>& model);
        ~AreaLighting() = default;

        void OnEveryUpdate() const;

        glm::vec3 Color = glm::vec3(1, 1, 1);
        glm::vec3 Position = glm::vec3(0, 0, 0);

    private:
        Ref<Model> m_Model;
    };
}
