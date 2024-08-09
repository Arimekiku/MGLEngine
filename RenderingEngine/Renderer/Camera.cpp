#include "Camera.h"

#include "Shader.h"
#include "Core/Bootstrapper.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace RenderingEngine
{
    Camera::Camera(const glm::vec3& position, const CameraAttributes& attributes)
    {
        m_Position = position;
        m_Attributes = attributes;

        auto view = glm::mat4(1.0f);
        auto proj = glm::mat4(1.0f);

        const Window& window = Bootstrapper::GetInstance().GetWindow();
        const float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());

        view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
        proj = glm::perspective(glm::radians(m_Attributes.FOV),
                                aspect,
                                m_Attributes.MinRenderDist,
                                m_Attributes.MaxRenderDist);

        m_ProjViewMat = proj * view;
    }
}
