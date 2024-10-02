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
        Ref<Model> m_BaseballBat;
        Ref<Model> m_SphereModel;
        Ref<Model> m_CubeModel;
        Ref<Model> m_PlaneModel;
        Ref<Material> m_DefaultMat;
        Ref<AreaLighting> m_Light;

        Framebuffer m_Framebuffer;
        Camera m_Camera;
        bool m_FirstClick = true;
        Time m_LastTime = Time(0);
    };
}
