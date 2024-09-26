#pragma once

#include "Renderer/Core.h"
#include "Renderer/Components/Model.h"
#include "Renderer/Components/AreaLighting.h"
#include "Renderer/Core/Layer/Layer.h"
#include "Renderer/Components/Camera.h"
#include "Renderer/Components/Texture.h"
#include "Renderer/Events/Event.h"

namespace RenderingEngine
{
    class SceneLayer final : public Layer
    {
    public:
        SceneLayer();

        void EveryUpdate() override;
        void OnEvent(Event& event) override;

    private:
        Ref<Model> m_Pyramid;
        Ref<Material> m_DefaultMat;
        Ref<Material> m_PyramidMat;
        Ref<AreaLighting> m_Light;

        Camera m_Camera;
        bool m_FirstClick = true;
    };
}
