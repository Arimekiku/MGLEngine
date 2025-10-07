#pragma once

#include "Core.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>

namespace Engine
{
    enum CullMode
    {
		Front = GL_FRONT,
        Back = GL_BACK
    };

    struct MaterialLayout
    {
        std::unordered_map<std::string, int> TextureUniforms;
        std::unordered_map<std::string, int> IntUniforms;
        std::unordered_map<std::string, float> FloatUniforms;
        std::unordered_map<std::string, glm::vec3> Vec3Uniforms;
        std::unordered_map<std::string, glm::mat4> Mat4Uniforms;

		CullMode Mode = Back;
    };

    class Material
    {
    public:
        Material(const std::string& vertPath, const std::string& fragPath);
        Material(const Ptr<Shader>& shader);

        const MaterialLayout& GetProperties() { return m_Uniforms; }

        void Bind() const;
        void BindFloatUniform(const std::string& name, float value);
        void BindVec3Uniform(const std::string& name, glm::vec3 value);
        void BindMat4Uniform(const std::string& name, glm::mat4 value);
		void BindIntUniform(const std::string& name, int value);
        void BindTextureSlot(const std::string& name, int slot);
        void Unbind() const;

		void SetCullMode(const CullMode mode) { m_Uniforms.Mode = mode; }

        void SetName(const std::string& name) { m_Name = name; }
        std::string& GetName() { return m_Name; }

        void SetShader(const Ptr<Shader>& shader);
        const Ptr<Shader>& GetShader() { return m_Shader; }

        void AddTexture(const Ptr<Texture>& texture);
        std::string& GetVertPath() { return m_VertPath; }
        std::string& GetFragPath() { return m_FragPath; }

    private:
        std::string m_VertPath, m_FragPath, m_Name;
        Ptr<Shader> m_Shader = nullptr;

        MaterialLayout m_Uniforms;

        std::vector<Ptr<Texture>> m_Textures;
    };
}
