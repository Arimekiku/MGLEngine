#include "mxpch.h"
#include "Material.h"

#include <imgui.h>

namespace RenderingEngine
{
    Material::Material(const Ref<RenderingEngine::Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::Bind() const
    {
        //m_Texture->Bind();

        m_Shader->Bind();
        m_Shader->BindUniformFloat3("u_Albedo", m_Properties.Albedo);
        m_Shader->BindUniformFloat1("u_Roughness", m_Properties.Roughness);
        m_Shader->BindUniformFloat1("u_Metallic", m_Properties.Metallic);
        m_Shader->BindUniformFloat1("u_AO", m_Properties.AO);

        //m_Texture->Unbind();
    }

    void Material::SetShader(const Ref<RenderingEngine::Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::SetTextureMap(const Ref<Texture>& newTexture)
    {
        m_Texture = newTexture;

        m_Properties.AlbedoID = newTexture->GetRendererID();
    }

    void Material::SetAlbedo(const glm::vec3 value)
    {
        m_Properties.Albedo = value;
    }

    void Material::SetRoughness(const float value)
    {
        m_Properties.Roughness = std::clamp(value, 0.0f, 1.0f);
    }

    void Material::SetMetallic(const float value)
    {
        m_Properties.Metallic = std::clamp(value, 0.0f, 1.0f);
    }

    void Material::SetAO(const float value)
    {
        m_Properties.AO = std::clamp(value, 0.0f, 1.0f);
    }
}