#pragma once

#include "Core.h"
#include "Material.h"
#include "Mesh.h"
#include "RenderCamera.h"
#include "Shader.h"

namespace Engine
{
    class Renderer
    {
    public:
        static void Initialize();

        static void UpdateCameraMatrix(const glm::mat4& cameraProjView);
        static void Clear(glm::vec4 color);

        static void RenderMesh(const Ptr<Mesh>& mesh, const Ptr<Shader>& shader, const glm::mat4& trsMatrix);
        static void RenderMesh(const Ptr<Mesh>& mesh, const Ptr<Material>& material, const glm::mat4& trsMatrix);

        static void RenderMeshEntity(const Ptr<Mesh>& mesh, const Ptr<Material>& material, const glm::mat4& trsMatrix, int entityId);

        static void SetViewport(int16_t x, int16_t y, int16_t width, int16_t height);

    private:
        static glm::mat4 m_ProjViewMat;
    };
}
