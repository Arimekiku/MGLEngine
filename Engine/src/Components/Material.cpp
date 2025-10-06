#include "Material.h"

#include <sstream>

namespace Engine
{
    Material::Material(const std::string& vertPath, const std::string& fragPath)
    {
        std::stringstream vertResult;
        vertResult << RESOURCES_PATH << vertPath;

        std::stringstream fragResult;
        fragResult << RESOURCES_PATH << fragPath;

        m_Shader = Ptr<Shader>::Create(vertResult.str(), fragResult.str());

        m_VertPath = vertPath;
        m_FragPath = fragPath;
    }

    Material::Material(const Ptr<Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::SetShader(const Ptr<Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::AddTexture(const Ptr<Texture>& texture)
    {
        m_Textures.push_back(texture);
    }

    void Material::Bind() const
    {
    	glCullFace(m_Uniforms.Mode == Back ? GL_BACK : GL_FRONT);

        m_Shader->Bind();

        for (auto& texture : m_Textures)
            texture->Bind();

        for (auto& [name, value] : m_Uniforms.FloatUniforms)
            m_Shader->BindUniformFloat1(name, value);

        for (auto& [name, value] : m_Uniforms.Vec3Uniforms)
            m_Shader->BindUniformFloat3(name, value);

        for (auto& [name, value] : m_Uniforms.Mat4Uniforms)
			m_Shader->BindUniformMat4(name, value);
    }

    void Material::Unbind() const
    {
        for (const auto& texture : m_Textures)
            texture->Unbind();

        m_Shader->Unbind();

        glCullFace(m_Uniforms.Mode != Back ? GL_BACK : GL_FRONT);
    }

    void Material::BindFloatUniform(const std::string& name, float value)
    {
        m_Uniforms.FloatUniforms[name] = value;
    }

    void Material::BindVec3Uniform(const std::string& name, glm::vec3 value)
    {
        m_Uniforms.Vec3Uniforms[name] = value;
    }

    void Material::BindMat4Uniform(const std::string& name, glm::mat4 value)
    {
        m_Uniforms.Mat4Uniforms[name] = value;
    }

    void Material::BindTextureSlot(const std::string& name, int slot)
    {
        m_Uniforms.TextureUniforms[name] = slot;
    }
}
