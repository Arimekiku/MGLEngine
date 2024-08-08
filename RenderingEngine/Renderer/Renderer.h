#pragma once

#include "VertexArray.h"

#include <glm/vec4.hpp>

namespace RenderingEngine
{
    class Renderer
    {
    public:
        static void Clear(glm::vec4 color);
        static void RenderIndexed(const std::shared_ptr<VertexArray>& vertices);
    };
}
