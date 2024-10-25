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
        Scene* m_Scene = new Scene();
        bool m_FirstClick = true;
        Time m_LastTime = Time(0);
        Camera m_Camera;

        GuiRenderer m_GuiRenderer = GuiRenderer();
    };
}
