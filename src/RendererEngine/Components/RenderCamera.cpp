#include "RenderCamera.h"
#include "RendererEngine.h"

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/vector_angle.hpp>

namespace RenderingEngine
{
    RenderCamera::RenderCamera()
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();
        Resize(window.GetWidth(), window.GetHeight());
    }

    void RenderCamera::Resize(const float width, const float height)
    {
        m_Aspect = width / height;

        m_Proj = glm::perspective(glm::radians(m_FOV), m_Aspect, m_MinRenderDist, m_MaxRenderDist);
    }

    void RenderCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_FUNC(OnWindowResizeEvent));
    }

    glm::mat4& RenderCamera::SetPerspective(float fov, float minDist, float maxDist)
    {
        m_FOV = fov;
        m_MinRenderDist = minDist;
        m_MaxRenderDist = maxDist;

        m_Proj = glm::perspective(glm::radians(m_FOV), m_Aspect, m_MinRenderDist, m_MaxRenderDist);

        return m_Proj;
    }

    bool RenderCamera::OnWindowResizeEvent(const WindowResizeEvent& e)
    {
        const auto& window = Bootstrapper::GetInstance().GetWindow();
        Resize(window.GetWidth(), window.GetHeight());

        return true;
    }
}
