#include "mxpch.h"
#include "Material.h"

#include <imgui.h>

namespace RenderingEngine
{
    Material::Material(const std::string& vertPath, const std::string& fragPath)
    {
        std::stringstream vertResult;
        vertResult << RESOURCES_PATH << vertPath;

        std::stringstream fragResult;
        fragResult << RESOURCES_PATH << fragPath;

        m_Shader = std::make_shared<Shader>(vertResult.str(), fragResult.str());

        m_VertPath = vertPath;
        m_FragPath = fragPath;
    }

    Material::Material(const Ref<RenderingEngine::Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::SetShader(const Ref<Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::AddTexture(const Ref<Texture>& texture)
    {
        m_Textures.push_back(texture);
    }

    void Material::Bind() const
    {
        for (auto& texture : m_Textures)
        {
            texture->Bind();
        }

        m_Shader->Bind();

        for (auto& [name, value] : m_Uniforms.FloatUniforms)
        {
            m_Shader->BindUniformFloat1(name, value);
        }

        for (auto& [name, value] : m_Uniforms.Vec3Uniforms)
        {
            m_Shader->BindUniformFloat3(name, value);
        }

        for (auto& texture : m_Textures)
        {
            texture->Unbind();
        }
    }

    void Material::BindFloatUniform(std::string name, float value)
    {
        m_Uniforms.FloatUniforms[name] = value;
    }

    void Material::BindVec3Uniform(std::string name, glm::vec3 value)
    {
        m_Uniforms.Vec3Uniforms[name] = value;
    }

    void Material::BindMat4Uniform(std::string name, glm::mat4 value)
    {
        m_Uniforms.Mat4Uniforms[name] = value;
    }

    void Material::BindTextureSlot(std::string name, int slot)
    {
        m_Uniforms.TextureUniforms[name] = slot;
    }
}