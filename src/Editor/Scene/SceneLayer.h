#pragma once

#include "RendererEngine.h"
#include "Scene.h"
#include "SceneRenderer.h"

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

        SceneRenderer m_GuiRenderer = SceneRenderer();
    };
}
