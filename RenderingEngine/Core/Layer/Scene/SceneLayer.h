#pragma once

#include "Core/Layer/Layer.h"
#include "Renderer/Camera.h"
#include "Renderer/VertexArray.h"

namespace RenderingEngine
{
    class SceneLayer : public Layer
    {
    public:
        SceneLayer();

        void EveryUpdate() override;

    private:
        std::shared_ptr<Shader> m_TestShader;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<VertexArray> m_VertexArray;

        Camera m_Camera;
        bool m_FirstClick = true;
    };
}
