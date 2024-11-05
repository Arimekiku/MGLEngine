#pragma once

#include "RendererEngine/Core.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderCamera.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    struct RendererData
    {
        GLuint vbo;
        GLuint vao;
    };

    class Renderer
    {
    public:
        static void Initialize();

        static void UpdateCameraMatrix(const glm::mat4& cameraProjView);
        static void Clear(glm::vec4 color);

        static void RenderSkybox(const Cubemap& cubemap);
        static void RenderMesh(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& TRSMatrix);

        static void SetViewport(int16_t x, int16_t y, int16_t width, int16_t height);

    private:
        static glm::mat4 m_ProjViewMat;
        static RendererData m_Data;
        static Shader m_SkyboxShader;
    };
}
