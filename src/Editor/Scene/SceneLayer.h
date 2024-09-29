#pragma once

#include "RendererEngine.h"

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
        Ref<Model> m_Pyramid;
        Ref<Material> m_DefaultMat;
        Ref<Material> m_PyramidMat;
        Ref<AreaLighting> m_Light;

        Framebuffer m_Framebuffer;
        Camera m_Camera;
        bool m_FirstClick = true;
        Time m_LastTime = Time(0);
    };
}
