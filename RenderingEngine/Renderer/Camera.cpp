#include "Camera.h"

#include <backends/imgui_impl_opengl3_loader.h>

#include "Shader.h"
#include "Core/Bootstrapper.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace RenderingEngine
{
    Camera::Camera(glm::vec3 position)
    {
        m_Position = position;
    }

    void Camera::SetProjection(float FOV, float minRendering, std::shared_ptr<RendereringEngine::Shader>& shader,
                               float maxRendering,
                               const char* uniform)
    {
        auto view = glm::mat4(1.0f);
        auto proj = glm::mat4(1.0f);

        view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
        proj = glm::perspective(glm::radians(45.0f),
                                static_cast<float>(Bootstrapper::GetInstance().GetWindow().GetWidth() /
                                    Bootstrapper::GetInstance()
                                    .GetWindow().GetHeight()),
                                minRendering,
                                maxRendering);

        glUniformMatrix4fv(glGetUniformLocation(shader->GetRendererID(), uniform), 1, GL_FALSE,
                           glm::value_ptr(proj * view));
    }
}
