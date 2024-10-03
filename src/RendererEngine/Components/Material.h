#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "RendererEngine/Core.h"

namespace RenderingEngine
{
    struct PBRMaterialProperties
    {
        uint32_t AlbedoID = 0;
        glm::vec3 Albedo = glm::vec3(1.0f);
        float Roughness = 0.0f;
        float Metallic = 1.0f;
        float AO = 1.0f;
    };

    class Material
    {
    public:
        explicit Material(const Ref<Shader>& shader);

        void Bind() const;

        void SetShader(const Ref<Shader>& shader);
        const Ref<Shader>& GetShader() { return m_Shader; }

        auto& GetProperties() { return m_Properties; }

        void SetTextureMap(const Ref<Texture>& newTexture);

        void SetAlbedo(glm::vec3 value);
        void SetRoughness(float value);
        void SetMetallic(float value);
        void SetAO(float value);

    private:
        Ref<Shader> m_Shader;
        Ref<Texture> m_Texture;

        PBRMaterialProperties m_Properties;
    };
}
