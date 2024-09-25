#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Shader.h"
#include "Renderer/Core.h"

namespace RenderingEngine {
    struct Material {
        explicit Material(const Ref<Shader>& shader)
        {
            Shader = shader;
        }

        Ref<Shader> Shader;

        glm::vec3 Albedo = glm::vec3(1.0f);
        float Roughness = 0.0f;
        float Metallic = 1.0f;
        float AO = 1.0f;

        void OnGuiRender()
        {
            ImGui::Begin("Material");
            ImGui::ColorEdit3("Albedo", glm::value_ptr(Albedo));
            ImGui::InputFloat("Rougness", &Roughness);
            ImGui::InputFloat("Metallic", &Metallic);
            ImGui::End();
        }
    };
}
