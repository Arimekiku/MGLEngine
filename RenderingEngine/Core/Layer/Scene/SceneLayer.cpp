#include "mxpch.h"
#include "SceneLayer.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer/Renderer.h"

namespace RenderingEngine
{
    SceneLayer::SceneLayer() : m_Camera({0, 0, -2})
    {
        m_VertexArray = std::make_shared<VertexArray>();

        constexpr float ver[5 * 7] = {
            -0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        };
        vertexBuffer = std::make_shared<VertexBuffer>(ver, sizeof(ver));

        const RenderBufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->SetVertexBuffer(vertexBuffer);

        constexpr int count = 18;
        constexpr uint32_t indices[count] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };
        indexBuffer = std::make_shared<IndexBuffer>(indices, count);
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_TestShader = std::make_shared<Shader>(
            "Resources/Shaders/standart.vert",
            "Resources/Shaders/standart.frag");

        m_QuadShader = std::make_shared<Shader>(
            "Resources/Shaders/quad.vert",
            "Resources/Shaders/quad.frag");

        m_TestTransform = std::make_shared<Transform>();

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
        Renderer::RenderIndexed(m_VertexArray, m_TestShader, m_TestTransform->GetTRSMatrix());

        auto cubeTransform = Transform(glm::vec3(0, 0, 5));
        Renderer::RenderCube(m_QuadShader, cubeTransform.GetTRSMatrix());
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
