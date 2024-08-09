#pragma once

#include "glm/fwd.hpp"

namespace RenderingEngine
{
    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        static void Unbind();
        void BindUniformMat4(const std::string& name, const glm::mat4& mat) const;

        const uint32_t& GetRendererID() const { return m_RendererID; }

    private:
        uint32_t m_RendererID;
    };
}
