#include "mxpch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace RenderingEngine
{
    void Renderer::Clear(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderIndexed(const std::shared_ptr<VertexArray>& vertices)
    {
        vertices->Bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices->GetIndexBuffer()->GetIndexCount());
    }
}
