#pragma once

#include "Core.h"
#include "Mesh.h"
#include "RenderCamera.h"
#include "Shader.h"
#include "Texture.h"

namespace Engine
{
    class Renderer
    {
    public:
        static void Initialize();

        static void UpdateCameraMatrix(const glm::mat4& cameraProjView);
        static void Clear(glm::vec4 color);

        static void RenderMesh(const Ptr<Mesh>& mesh, const Ptr<Shader>& shader, const glm::mat4& trsMatrix);
		static void RenderCubeMap(const Ptr<Mesh>& mesh, const Ptr<Shader>& shader, const Ptr<Cubemap>& cubemap, const glm::mat4& trsMatrix);

        static void SetViewport(int16_t x, int16_t y, int16_t width, int16_t height);

    private:
        static glm::mat4 m_ProjViewMat;
    };
}
