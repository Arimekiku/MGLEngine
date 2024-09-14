#pragma once

#include <glm/fwd.hpp>

namespace RenderingEngine
{
    class Shader
    {
    public:
        Shader(const std::string& vertPath, const std::string& fragPath);
        ~Shader();

        void Bind() const;
        static void Unbind();

        void BindUniformMat4(const std::string& name, const glm::mat4& mat) const;
        void BindUniformMat3(const std::string& name, const glm::mat3& mat) const;
        void BindUniformFloat4(const std::string& name, const glm::f32vec4& vec) const;
        void BindUniformFloat3(const std::string& name, const glm::f32vec3& vec) const;
        void BindUniformFloat2(const std::string& name, const glm::f32vec2& vec) const;
        void BindUniformFloat1(const std::string& name, const float& val) const;
        void BindUniformInt4(const std::string& name, const glm::i32vec4& vec) const;
        void BindUniformInt3(const std::string& name, const glm::i32vec3& vec) const;
        void BindUniformInt2(const std::string& name, const glm::i32vec2& vec) const;
        void BindUniformInt1(const std::string& name, const int& val) const;

        [[nodiscard]] const uint32_t& GetRendererID() const { return m_RendererID; }

    private:
        std::string GetContent(const std::string& path);

        uint32_t m_RendererID;
    };
}
