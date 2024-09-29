#include "mxpch.h"
#include "SceneLayer.h"
#include "RendererEngine.h"

#include <imgui.h>

namespace RenderingEngine
{
    SceneLayer::SceneLayer() : m_Camera(glm::vec3(0, 0, -2))
    {
        const auto& shader = std::make_shared<Shader>(
            RESOURCES_PATH "Shaders/default.vert",
            RESOURCES_PATH "Shaders/default.frag");

        const auto& m_FaceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& m_HouseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

        m_Light = std::make_shared<AreaLighting>(glm::vec3(0, 4, 4));
        m_Light->Color = glm::vec3(300, 300, 300);

        shader->Bind();
        shader->BindUniformInt1("u_Texture", 0);
        shader->BindUniformFloat3("u_LightColor", m_Light->Color);
        shader->BindUniformFloat3("u_LightPos", m_Light->GetTransform()->Position);

        m_DefaultMat = std::make_shared<Material>(shader);
        m_DefaultMat->SetTextureMap(m_HouseTexture);

        m_PyramidMat = std::make_shared<Material>(shader);
        m_PyramidMat->SetAlbedo(glm::vec3(0.83f, 0.70f, 0.44f));
        m_PyramidMat->SetTextureMap(m_FaceTexture);

        m_Pyramid = std::make_shared<Model>(m_PyramidMat);

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

        m_Framebuffer = std::make_shared<Framebuffer>(800, 600);

        Renderer::Initialize();
    }

    void SceneLayer::OnEveryUpdate()
    {
        m_Framebuffer->Bind();

        Renderer::CreateWorld(m_Camera);

        m_Camera.EveryUpdate();

        Renderer::Clear(glm::vec4(0, 0, 0, 1));

        Renderer::RenderModel(m_Pyramid);

        const auto cubeTransform = Transform(glm::vec3(0, 0, 5));
        Renderer::RenderCube(m_DefaultMat, cubeTransform.GetTRSMatrix());

        const auto quadTransform = Transform(glm::vec3(3, 3, 2));
        Renderer::RenderQuad(m_DefaultMat, quadTransform.GetTRSMatrix());

        m_Framebuffer->Unbind();
    }

    void SceneLayer::OnGuiUpdate()
    {
        static bool docking = true;
        static bool optFullScreenPersistance = true;
        static bool optFullscreen = true;
        static ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_None;

        ImGuiDockNodeFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (optFullscreen == true)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockFlags & ImGuiDockNodeFlags_PassthruCentralNode)
            windowFlags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("DockSpace Test", &docking, windowFlags);
        ImGui::PopStyleVar();

        if (optFullscreen)
            ImGui::PopStyleVar(2);

        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockID = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockID, ImVec2(0, 0), dockFlags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close"))
                    docking = false;

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Scene");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);
        if (glm::i16vec2(m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight()) != castSize)
        {
            m_Camera.Resize(castSize.x, castSize.y);
            //Bootstrapper::GetInstance()
            m_Framebuffer->Resize(castSize.x, castSize.y);
        }

        const uint32_t m_Texture = m_Framebuffer->GetTextureAttachment();
        ImGui::Image(reinterpret_cast<void*>(m_Texture), ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));

        ImGui::End();

        ImGui::Begin("Temple");
        ImGui::InputFloat3("Position", glm::value_ptr(m_Pyramid->GetTransform()->Position));
        ImGui::InputFloat3("Rotation", glm::value_ptr(m_Pyramid->GetTransform()->Rotation));
        ImGui::InputFloat3("Scale", glm::value_ptr(m_Pyramid->GetTransform()->Scale));
        ImGui::End();

        m_DefaultMat->OnGuiRender("DefaultMat");
        m_PyramidMat->OnGuiRender("PyramidMat");

        ImGui::End();
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
