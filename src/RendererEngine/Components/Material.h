#pragma once

#include "Shader.h"
#include "Texture.h"
#include "RendererEngine/Core.h"

namespace RenderingEngine
{
    struct MaterialProperties
    {
        std::unordered_map<std::string, int> TextureUniforms;
        std::unordered_map<std::string, float> FloatUniforms;
        std::unordered_map<std::string, glm::vec3> Vec3Uniforms;
        std::unordered_map<std::string, glm::mat4> Mat4Uniforms;
    };

    class Material
    {
    public:
        Material(const std::string& vertPath, const std::string& fragPath);
        Material(const Ref<Shader>& shader);

        const MaterialProperties& GetProperties() { return m_Uniforms; }

        void Bind() const;
        void BindFloatUniform(std::string name, float value);
        void BindVec3Uniform(std::string name, glm::vec3 value);
        void BindMat4Uniform(std::string name, glm::mat4 value);
        void BindTextureSlot(std::string name, int slot);

        void SetName(const std::string& name) { m_Name = name; }
        std::string& GetName() { return m_Name; }

        void SetShader(const Ref<Shader>& shader);
        const Ref<Shader>& GetShader() { return m_Shader; }

        void AddTexture(const Ref<Texture>& texture);
        std::string& GetVertPath() { return m_VertPath; }
        std::string& GetFragPath() { return m_FragPath; }

    private:
        std::string m_VertPath, m_FragPath, m_Name;
        Ref<Shader> m_Shader;

        MaterialProperties m_Uniforms;

        std::vector<Ref<Texture>> m_Textures;
    };
}
