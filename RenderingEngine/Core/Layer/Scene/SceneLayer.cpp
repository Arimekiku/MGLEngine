#include "SceneLayer.h"

#include "imgui.h"
#include "Core/Input.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/Renderer.h"

namespace RenderingEngine
{
    SceneLayer::SceneLayer() : m_Camera({0, 0, -2})
    {
        m_VertexArray.reset(new VertexArray());

        float ver[5 * 7] = {
            -0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        };
        m_VertexBuffer.reset(new VertexBuffer(ver, sizeof(ver)));

        RenderBufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };
        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->SetVertexBuffer(m_VertexBuffer);

        int count = 18;
        uint32_t indices[count] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };
        m_IndexBuffer.reset(new IndexBuffer(indices, count));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        m_TestShader.reset(new Shader(
            "Resources/Shaders/standart.vert",
            "Resources/Shaders/standart.frag"));
    }

    void SceneLayer::EveryUpdate()
    {
        Renderer::CreateWorld(m_Camera);

        if (Input::KeyPressed(GLFW_KEY_A))
            m_Camera.m_Position += 0.2f * -glm::normalize(glm::cross(m_Camera.m_Orientation, {0, 1, 0}));

        if (Input::KeyPressed(GLFW_KEY_D))
            m_Camera.m_Position += 0.2f * glm::normalize(glm::cross(m_Camera.m_Orientation, {0, 1, 0}));

        if (Input::KeyPressed(GLFW_KEY_W))
            m_Camera.m_Position += m_Camera.m_Orientation * 0.2f;

        if (Input::KeyPressed(GLFW_KEY_S))
            m_Camera.m_Position += -m_Camera.m_Orientation * 0.2f;

        if (Input::MouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            Input::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            glm::vec2 mousePos = Input::GetCursorPosition();

            glm::vec2 rotationVector = Input::GetNormalizedCursor();

            m_Camera.SetOrientation(rotationVector.x, rotationVector.y);
            Input::SetCursorInCenterOfWindow();
        }

        if (Input::MouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
        {
            Input::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        ImGui::Begin("Camera");
        ImGui::InputFloat3("Camera position", glm::value_ptr(m_Camera.m_Position));
        ImGui::InputFloat3("Camera orientation", glm::value_ptr(m_Camera.m_Orientation));
        ImGui::End();

        Renderer::Clear({0, 0, 0, 1});
        Renderer::RenderIndexed(m_VertexArray, m_TestShader);
    }
}
