#pragma once

#include "Scene/Scene.h"
#include "Scene/SceneRenderer.h"

namespace RenderingEngine
{
    class EditorLayer final : public Layer
    {
    public:
        EditorLayer();

        void OnEveryUpdate(Time deltaTime) override;
        void OnGuiUpdate() override;

        void OnEvent(Event& event) override;

    private:
        Ref<Scene> m_Scene = std::make_shared<Scene>();
        bool m_IsScenePanel = true, m_IsViewportPanel = true, m_IsInspectorPanel = true, m_FirstClick = true;
        Time m_LastTime = Time(0);

        SceneRenderer m_SceneRenderer = SceneRenderer();

#ifdef DEBUG
        bool m_IsImGuiDemoPanel = false;
#endif
    };
}
