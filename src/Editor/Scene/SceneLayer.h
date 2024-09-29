#pragma once

#include "Renderer/Core.h"
#include "Renderer/Components/Model.h"
#include "Renderer/Components/AreaLighting.h"
#include "Renderer/Core/Layer/Layer.h"
#include "Renderer/Components/Camera.h"
#include "Renderer/Components/Framebuffer.h"
#include "Renderer/Components/Texture.h"
#include "Renderer/Events/Event.h"

namespace RenderingEngine
{
    class SceneLayer : public Layer
    {
    public:
        SceneLayer();

        void OnEveryUpdate() override;
        void OnGuiUpdate() override;

        void OnEvent(Event& event) override;

    private:
        Ref<Framebuffer> m_Framebuffer;
        Ref<Model> m_Pyramid;
        Ref<Material> m_DefaultMat;
        Ref<Material> m_PyramidMat;
        Ref<AreaLighting> m_Light;

        Camera m_Camera;
        bool m_FirstClick = true;
    };
}
