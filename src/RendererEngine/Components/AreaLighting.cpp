#include "mxpch.h"
#include "AreaLighting.h"

namespace RenderingEngine
{
    AreaLighting::AreaLighting(const glm::vec3 position)
    {
        m_Position->Position = position;
    }
}