#pragma once

#include "RendererEngine.h"
#include "Scene.h"
#include "GuiRenderer.h"

namespace RenderingEngine
{
    class SceneLayer final : public Layer
    {
    public:
        SceneLayer();

        void OnEveryUpdate(Time deltaTime) override;
        void OnGuiUpdate() override;

        void OnEvent(Event& event) override;

    private:
        Ref<Scene> m_Scene = std::make_shared<Scene>();
        bool m_FirstClick = true;
        Time m_LastTime = Time(0);
        Ref<Camera> m_Camera = std::make_shared<Camera>(glm::vec3(0, 0, 10));

        GuiRenderer m_GuiRenderer = GuiRenderer();
    };
}
