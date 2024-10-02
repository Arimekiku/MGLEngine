#pragma once

#include "RendererEngine/Core.h"
#include "Camera.h"
#include "Model.h"

namespace RenderingEngine
{
    class Renderer
    {
    public:
        static void OnEveryUpdate(Camera& camera);
        static void Clear(glm::vec4 color);

        static void RenderModel(const Ref<Model>& model);

        static void SetViewport(int16_t x, int16_t y, int16_t width, int16_t height);

    private:
        static glm::mat4 m_ProjViewMat;
    };
}
