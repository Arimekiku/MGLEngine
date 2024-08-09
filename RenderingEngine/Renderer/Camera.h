#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

namespace RenderingEngine
{
    struct CameraAttributes
    {
        float FOV;
        float MinRenderDist;
        float MaxRenderDist;

        explicit CameraAttributes(const float fov = 45,
                                  const float minRenderDist = 0.1f,
                                  const float maxRenderDist = 100)
        {
            FOV = fov;
            MinRenderDist = minRenderDist;
            MaxRenderDist = maxRenderDist;
        }
    };

    class Camera
    {
    public:
        explicit Camera(const glm::vec3& position, const CameraAttributes& attributes = CameraAttributes());
        ~Camera() = default;

        const glm::mat4& GetProjViewMat() const { return m_ProjViewMat; }

    private:
        CameraAttributes m_Attributes;

        glm::mat4 m_ProjViewMat{};

        glm::vec3 m_Position{};
        glm::vec3 m_Orientation = glm::vec3(0.25f, 0.25f, 1.0f);
        glm::vec3 m_Up = glm::vec3(0, 1.0f, 0.0f);
    };
}
