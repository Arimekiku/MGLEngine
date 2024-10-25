#pragma once

#include "RendererEngine/Core.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

namespace RenderingEngine
{
    class Renderer
    {
    public:
        static void OnEveryUpdate(Ref<Camera>& camera);
        static void Clear(glm::vec4 color);

        static void RenderMesh(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& TRSMatrix);

        static void SetViewport(int16_t x, int16_t y, int16_t width, int16_t height);

    private:
        static glm::mat4 m_ProjViewMat;
    };
}
