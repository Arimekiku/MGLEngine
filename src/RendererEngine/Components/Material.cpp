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
        m_Texture->Bind();

        m_Shader->Bind();
        m_Shader->BindUniformFloat3("u_Albedo", m_Albedo);
        m_Shader->BindUniformFloat1("u_Roughness", m_Roughness);
        m_Shader->BindUniformFloat1("u_Metallic", m_Metallic);
        m_Shader->BindUniformFloat1("u_AO", m_AO);

        m_Texture->Unbind();
    }

    void Material::SetShader(const Ref<RenderingEngine::Shader>& shader)
    {
        m_Shader = shader;
    }

    void Material::SetTextureMap(const Ref<Texture>& newTexture)
    {
        m_Texture = newTexture;
    }

    void Material::SetAlbedo(const glm::vec3 value)
    {
        m_Albedo = value;
    }

    void Material::SetRoughness(const float value)
    {
        m_Roughness = std::clamp(value, 0.0f, 1.0f);
    }

    void Material::SetMetallic(const float value)
    {
        m_Metallic = std::clamp(value, 0.0f, 1.0f);
    }

    void Material::SetAO(const float value)
    {
        m_AO = std::clamp(value, 0.0f, 1.0f);
    }

    void Material::OnGuiRender(const char* name)
    {
        ImGui::Begin(name);

        const uint32_t ID = m_Texture->GetRendererID();
        ImGui::Image(reinterpret_cast<void*>(ID), ImVec2(64, 64));

        if (ImGui::ColorEdit3("Albedo", glm::value_ptr(m_Albedo)))
            m_Shader->BindUniformFloat3("u_Albedo", m_Albedo);

        if (ImGui::SliderFloat("Rougness", &m_Roughness, 0, 1, "%.2f"))
            m_Shader->BindUniformFloat1("u_Roughness", m_Roughness);

        if (ImGui::SliderFloat("Metallic", &m_Metallic, 0, 1, "%.2f"))
            m_Shader->BindUniformFloat1("u_Metallic", m_Metallic);

        if (ImGui::SliderFloat("AO", &m_AO, 0, 1, "%.2f"))
            m_Shader->BindUniformFloat1("u_AO", m_AO);

        ImGui::End();
    }
}