#pragma once

#include "RendererEngine.h"
#include "Scene.h"

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
        Scene m_Scene;
        bool m_FirstClick = true;
        Time m_LastTime = Time(0);
    };
}
