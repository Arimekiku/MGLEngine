#pragma once

#include "Renderer/Core.h"
#include "Renderer/Core/Layer/Layer.h"
#include "Renderer/Components/Camera.h"
#include "Renderer/Components/Mesh.h"
#include "Renderer/Components/Shader.h"
#include "Renderer/Components/Texture.h"
#include "Renderer/Components/Transform.h"
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
        Ref<Shader> m_QuadShader;
        Ref<Shader> m_TestShader;
        Ref<Texture> m_TestTexture;
        Ref<Transform> m_TestTransform;
        Ref<Mesh> m_PyramidMesh;

        Camera m_Camera;
        bool m_FirstClick = true;
    };
}
