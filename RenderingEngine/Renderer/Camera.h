#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

namespace RenderingEngine
{
    class Camera
    {
    public:
        Camera(glm::vec3 position);
        ~Camera() = default;

        void SetProjection(float FOV, float minRendering, std::shared_ptr<RendereringEngine::Shader>& shader,
                           float maxRendering,
                           const char* uniform);

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Orientation = glm::vec3(0.0f, 0.25f, 1.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    };
}
