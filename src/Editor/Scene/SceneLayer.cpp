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
        m_PyramidMesh = std::make_shared<Mesh>();

        constexpr float ver[16 * 11] = {
            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

            -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

             0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

             0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
            -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
             0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f,  // Facing side
        };
        m_PyramidMesh->SetVertices(ver, sizeof(ver));

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
        m_PyramidMesh->SetIndices(indices, count);

        m_TestShader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/standart.vert",
            RESOURCES_PATH "Shaders/standart.frag");

        m_QuadShader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/quad.vert",
            RESOURCES_PATH "Shaders/quad.frag");

        m_TestTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        m_QuadShader->Bind();
        m_QuadShader->BindUniformInt1("u_Texture", 0);

        m_TestTransform = std::make_shared<Transform>();

        m_Light = std::make_shared<AreaLighting>(glm::vec3(0, 4, 4));
        m_QuadShader->Bind();
        m_QuadShader->BindUniformFloat4("u_LightColor", m_Light->Color);
        m_QuadShader->BindUniformFloat3("u_LightPos", m_Light->GetTransform()->Position);
        m_TestShader->Bind();
        m_TestShader->BindUniformFloat4("u_LightColor", m_Light->Color);
        m_TestShader->BindUniformFloat3("u_LightPos", m_Light->GetTransform()->Position);

        Renderer::Initialize();
    }

    void SceneLayer::EveryUpdate()
    {
        Renderer::CreateWorld(m_Camera);

        m_Camera.EveryUpdate();

        ImGui::Begin("Temple");
        ImGui::InputFloat3("Position", glm::value_ptr(m_TestTransform->Position));
        ImGui::InputFloat3("Rotation", glm::value_ptr(m_TestTransform->Rotation));
        ImGui::InputFloat3("Scale", glm::value_ptr(m_TestTransform->Scale));
        ImGui::End();

        Renderer::Clear({0, 0, 0, 1});
        m_TestTexture->Bind();

        Renderer::RenderIndexed(m_PyramidMesh->GetVertexArray(), m_QuadShader, m_TestTransform->GetTRSMatrix());

        const auto cubeTransform = Transform(glm::vec3(0, 0, 5));
        Renderer::RenderCube(m_QuadShader, cubeTransform.GetTRSMatrix());

        const auto quadTransform = Transform(glm::vec3(3, 3, 2));
        Renderer::RenderQuad(m_QuadShader, quadTransform.GetTRSMatrix());
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
