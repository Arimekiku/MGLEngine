#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace RenderingEngine
{
	class Mesh
    {
    public:
        Mesh() = default;
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh();

        void SetVertices(const float* vertices, size_t size);
        void SetIndices(const uint32_t* indices, uint32_t count);

        void SetPath(const std::string& path) { m_Path = path; }
        const std::string& GetPath() { return m_Path; }

        inline const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

    private:
        std::string m_Path = std::string();

        Ref<VertexArray> m_VertexArray = std::make_shared<VertexArray>();
    };
}
