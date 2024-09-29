#pragma once

#include "RendererEngine/Core.h"
#include "RendererEngine/Components/Model.h"
#include "RendererEngine/Components/AreaLighting.h"
#include "RendererEngine/Core/Layer/Layer.h"
#include "RendererEngine/Components/Camera.h"
#include "RendererEngine/Components/Framebuffer.h"
#include "RendererEngine/Components/Texture.h"
#include "RendererEngine/Events/Event.h"

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
