#include "mxpch.h"
#include "SceneLayer.h"
#include "Renderer/Components/Renderer.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace RenderingEngine
{
    SceneLayer::SceneLayer() : m_Camera(glm::vec3(0, 0, -2))
    {
        const auto& m_DefaultShader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/default.vert",
            RESOURCES_PATH "Shaders/default.frag");

        m_Texture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        m_Light = std::make_shared<AreaLighting>(glm::vec3(0, 4, 4));

        m_DefaultShader->Bind();
        m_DefaultShader->BindUniformInt1("u_Texture", 0);
        m_DefaultShader->BindUniformFloat4("u_LightColor", m_Light->Color);
        m_DefaultShader->BindUniformFloat3("u_LightPos", m_Light->GetTransform()->Position);

        m_DefaultMat = std::make_shared<Material>(m_DefaultShader);
        m_DefaultMat->Albedo = glm::vec3(0.83f, 0.70f, 0.44f);

        m_Pyramid = std::make_shared<Model>(m_DefaultMat);

        constexpr float ver[16 * 8] = {
            -0.5f, 0.0f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f, // Bottom side
            -0.5f, 0.0f, -0.5f,	  0.0f, 5.0f,   0.0f, -1.0f, 0.0f, // Bottom side
             0.5f, 0.0f, -0.5f,	  5.0f, 5.0f,   0.0f, -1.0f, 0.0f, // Bottom side
             0.5f, 0.0f,  0.5f,	  5.0f, 0.0f,   0.0f, -1.0f, 0.0f, // Bottom side

            -0.5f, 0.0f,  0.5f,   0.0f, 0.0f,   -0.8f, 0.5f, 0.0f, // Left Side
            -0.5f, 0.0f, -0.5f,   5.0f, 0.0f,   -0.8f, 0.5f, 0.0f, // Left Side
             0.0f, 0.8f,  0.0f,   2.5f, 5.0f,   -0.8f, 0.5f, 0.0f, // Left Side

            -0.5f, 0.0f, -0.5f,   5.0f, 0.0f,   0.0f, 0.5f, -0.8f, // Non-facing side
             0.5f, 0.0f, -0.5f,   0.0f, 0.0f,   0.0f, 0.5f, -0.8f, // Non-facing side
             0.0f, 0.8f,  0.0f,   2.5f, 5.0f,   0.0f, 0.5f, -0.8f, // Non-facing side

             0.5f, 0.0f, -0.5f,   0.0f, 0.0f,   0.8f, 0.5f,  0.0f, // Right side
             0.5f, 0.0f,  0.5f,   5.0f, 0.0f,   0.8f, 0.5f,  0.0f, // Right side
             0.0f, 0.8f,  0.0f,   2.5f, 5.0f,   0.8f, 0.5f,  0.0f, // Right side

             0.5f, 0.0f,  0.5f,   5.0f, 0.0f,   0.0f, 0.5f,  0.8f, // Facing side
            -0.5f, 0.0f,  0.5f,   0.0f, 0.0f,   0.0f, 0.5f,  0.8f, // Facing side
             0.0f, 0.8f,  0.0f,   2.5f, 5.0f,   0.0f, 0.5f,  0.8f, // Facing side
        };
        m_Pyramid->GetMesh()->SetVertices(ver, sizeof(ver));

        constexpr int count = 18;
        constexpr uint32_t indices[count] =
        {
            0, 1, 2, // Bottom side
            0, 2, 3, // Bottom side
            4, 6, 5, // Left side
            7, 9, 8, // Non-facing side
            10, 12, 11, // Right side
            13, 15, 14 // Facing side
        };
        m_Pyramid->GetMesh()->SetIndices(indices, count);

        Renderer::Initialize();
    }

    void SceneLayer::EveryUpdate()
    {
        Renderer::CreateWorld(m_Camera);

        m_Camera.EveryUpdate();

        ImGui::Begin("Temple");
        ImGui::InputFloat3("Position", glm::value_ptr(m_Pyramid->GetTransform()->Position));
        ImGui::InputFloat3("Rotation", glm::value_ptr(m_Pyramid->GetTransform()->Rotation));
        ImGui::InputFloat3("Scale", glm::value_ptr(m_Pyramid->GetTransform()->Scale));
        ImGui::End();

        m_DefaultMat->OnGuiRender();

        Renderer::Clear({0, 0, 0, 1});
        m_Texture->Bind();

        Renderer::RenderModel(m_Pyramid);

        const auto cubeTransform = Transform(glm::vec3(0, 0, 5));
        Renderer::RenderCube(m_Pyramid->GetMaterial(), cubeTransform.GetTRSMatrix());

        const auto quadTransform = Transform(glm::vec3(3, 3, 2));
        Renderer::RenderQuad(m_Pyramid->GetMaterial(), quadTransform.GetTRSMatrix());
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
