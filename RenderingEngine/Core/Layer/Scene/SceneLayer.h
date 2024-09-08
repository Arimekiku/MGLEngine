#pragma once

#include "Core/Layer/Layer.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Transform.h"
#include "Renderer/VertexArray.h"

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
