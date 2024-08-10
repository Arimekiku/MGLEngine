#include "Camera.h"

#include "Shader.h"
#include "Core/Bootstrapper.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/vector_angle.hpp>

namespace RenderingEngine
{
    Camera::Camera(const glm::vec3& position, const CameraAttributes& attributes)
    {
        m_Position = position;
        m_Attributes = attributes;
    }

    void Camera::SetOrientation(float rotX, float rotY)
    {
        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX),
                                               glm::normalize(glm::cross(m_Orientation, m_Up)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            m_Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
    }

    const glm::mat4& Camera::GetProjViewMat()
    {
        auto view = glm::mat4(1.0f);
        auto proj = glm::mat4(1.0f);

        const Window& window = Bootstrapper::GetInstance().GetWindow();
        const float aspect = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());

        view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
        proj = glm::perspective(glm::radians(m_Attributes.FOV),
                                aspect,
                                m_Attributes.MinRenderDist,
                                m_Attributes.MaxRenderDist);

        m_ProjView = glm::mat4(proj * view);
        return m_ProjView;
    }
}
