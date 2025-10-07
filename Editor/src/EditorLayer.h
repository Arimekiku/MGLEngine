#pragma once

#include "Scene/Scene.h"
#include "Scene/SceneRenderer.h"
#include "Scene/Panels/SceneRenderingPanel.h"

namespace Editor
{
    class EditorLayer final : public Layer
    {
    public:
        EditorLayer(const Ptr<Window>& window);

        void OnEveryUpdate(Time deltaTime) override;
        void OnGuiUpdate() override;

    private:
		Ptr<Window> m_Parent = nullptr;
        Ptr<Scene> m_Scene = nullptr;

        bool m_FirstClick = true;
        Time m_LastTime = Time(0);

        std::vector<Ptr<ScenePanel>> m_SceneRenderPanels;
        SceneRenderer m_SceneRenderer = SceneRenderer();

#ifdef DEBUG
        bool m_IsImGuiDemoPanel = false;
#endif
    };
}
