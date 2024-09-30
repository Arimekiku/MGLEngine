#include "mxpch.h"
#include "SceneLayer.h"
#include "RendererEngine.h"

#include <imgui.h>

namespace RenderingEngine
{
    SceneLayer::SceneLayer()
        : m_Framebuffer(800, 600), m_Camera(glm::vec3(0, 0, -2))
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

        m_BaseballBat = std::make_shared<Model>(RESOURCES_PATH "Models/baseballbat_mesh.fbx");
        m_BaseballBat->SetMaterial(m_DefaultMat);
        auto& position = m_BaseballBat->GetPosition();
        position = { 10, 6, 3 };

        Renderer::Initialize();
    }

    void SceneLayer::OnEveryUpdate(const Time deltaTime)
    {
        m_LastTime = deltaTime;

        m_Framebuffer.Bind();

        Renderer::CreateWorld(m_Camera);
        m_Camera.EveryUpdate(deltaTime);

        Renderer::Clear(glm::vec4(0, 0, 0, 1));
        const auto quadTransform = Transform(glm::vec3(3, 3, 2));
        Renderer::RenderQuad(m_DefaultMat, quadTransform.GetTRSMatrix());

        Renderer::RenderModel(m_BaseballBat);

        Framebuffer::Unbind();
    }

    void SceneLayer::OnGuiUpdate()
    {
        static bool docking = true;
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

        ImGui::Begin("Parameters");

        ImGui::Text("DeltaTime: %.3f (%.0fmil)", m_LastTime.GetSeconds(), m_LastTime.GetMilliseconds());

        ImGui::End();

        ImGui::Begin("Scene");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);
        if (glm::i16vec2(m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight()) != castSize)
        {
            m_Camera.Resize(castSize.x, castSize.y);
            m_Framebuffer.Resize(castSize.x, castSize.y);
        }

        const uint32_t m_Texture = m_Framebuffer.GetTextureAttachment();
        ImGui::Image(reinterpret_cast<void*>(m_Texture), ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));

        ImGui::End();

        ImGui::Begin("Baseball Bat");
        ImGui::InputFloat3("Position", glm::value_ptr(m_BaseballBat->GetPosition()));
        ImGui::InputFloat3("Rotation", glm::value_ptr(m_BaseballBat->GetRotation()));
        ImGui::InputFloat3("Scale", glm::value_ptr(m_BaseballBat->GetScale()));
        ImGui::End();

        m_DefaultMat->OnGuiRender("DefaultMat");

        ImGui::End();
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
