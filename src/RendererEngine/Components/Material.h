#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "RendererEngine/Core.h"

namespace RenderingEngine
{
    class Material
    {
    public:
        explicit Material(const Ref<Shader>& shader);

        const Ref<Shader>& GetShader() { return m_Shader; }
        void Bind() const;

        void SetShader(const Ref<Shader>& shader);

        void SetTextureMap(const Ref<Texture>& newTexture);

        void SetAlbedo(glm::vec3 value);
        void SetRoughness(float value);
        void SetMetallic(float value);
        void SetAO(float value);

        void OnGuiRender(const char* name = "Material");

    private:
        Ref<Shader> m_Shader;
        Ref<Texture> m_Texture;

        glm::vec3 m_Albedo = glm::vec3(1.0f);
        float m_Roughness = 0.0f;
        float m_Metallic = 1.0f;
        float m_AO = 1.0f;
    };
}
