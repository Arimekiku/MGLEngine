#include "EditorLayer.h"
#include "Scene/ComponentRegistry.h"
#include "Persistence/SceneSerializer.h"

#include <imgui.h>
#include <portable_file_dialogs.h>

namespace RenderingEngine
{
    EditorLayer::EditorLayer()
    {
        //ASSETS
		const auto& lightMaterial = MaterialImporter::AddMaterial("Light", "Shaders/areaLight.vert", "Shaders/areaLight.frag");
        const auto& defaultMaterial = MaterialImporter::AddMaterial("Default", "Shaders/default.vert", "Shaders/default.frag");

        const auto& faceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& houseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

        MeshImporter::CreatePlane();
        MeshImporter::CreateCube();
        MeshImporter::CreateSphere();

        m_SceneRenderer.SetContext(m_Scene);

        lightMaterial->BindVec3Uniform("u_LightColor", {1, 1, 1});

        defaultMaterial->AddTexture(houseTexture);
        defaultMaterial->BindTextureSlot("u_Texture", 0);
		defaultMaterial->BindTextureSlot("u_DepthMap", 1);
        defaultMaterial->BindVec3Uniform("u_LightColor", {1, 1, 1});
        defaultMaterial->BindVec3Uniform("u_LightPos", {0, 5, 5});
		defaultMaterial->BindMat4Uniform("u_lightViewProj", glm::mat4(1.0f));
    }

    void EditorLayer::OnEveryUpdate(const Time deltaTime)
    {
        m_LastTime = deltaTime;

        m_Scene->OnEveryUpdate(deltaTime);
        m_SceneRenderer.DrawDepthBuffer();
        m_SceneRenderer.DrawTextureBuffer();
    }

    void EditorLayer::OnGuiUpdate()
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
        {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Renderer", &docking, windowFlags);
        ImGui::PopStyleVar();

        if (optFullscreen)
        {
            ImGui::PopStyleVar(2);
        }

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
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    m_Scene = std::make_shared<Scene>();
                    m_SceneRenderer.SetContext(m_Scene);
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    auto& file = pfd::open_file("Select Scene", pfd::path::home(), { "Scene File (*.scene)", "*.scene" }).result();

                    if (file.empty() == false)
                    {
                        m_Scene = std::make_shared<Scene>();
                        m_SceneRenderer.SetContext(m_Scene);

                        SceneSerializer serializer = SceneSerializer(m_Scene);
                        serializer.Deserialize(file[0]);
                    }
                }

                if (ImGui::MenuItem("Save As...", "Ctrs+Shift+S"))
                {
                    auto& file = pfd::save_file("Select Scene", pfd::path::home(), { "Scene File (*.scene)", "*.scene" }).result();

                    if (file.empty() == false)
                    {
                        SceneSerializer serializer = SceneSerializer(m_Scene);
                        serializer.Serialize(file);
                    }
                }

                if (ImGui::MenuItem("Close"))
                {
                    docking = false;
                    Bootstrapper::GetInstance().Abort();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Viewport", "", &m_IsViewportPanel);
                ImGui::MenuItem("Scene", "", &m_IsScenePanel);
                ImGui::MenuItem("Inspector", "", &m_IsInspectorPanel);

#ifdef DEBUG
                ImGui::MenuItem("ImGuiDemo (Debug only)", "", &m_IsImGuiDemoPanel);
#endif
                
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Parameters");

        ImGui::Text("DeltaTime: %.3f (%.0fmil)", m_LastTime.GetSeconds(), m_LastTime.GetMilliseconds());

        ImGui::End();

        if (m_IsViewportPanel)
        {
            m_SceneRenderer.DrawViewport();
        }

        if (m_IsScenePanel)
        {
            m_SceneRenderer.DrawScenePanel();
        }

        if (m_IsInspectorPanel)
        {
            m_SceneRenderer.DrawInspectorPanel();
            m_SceneRenderer.DrawGuizmos();
        }

#ifdef DEBUG
        if (m_IsImGuiDemoPanel)
        {
            ImGui::ShowDemoWindow();
        }
#endif

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_SceneRenderer.OnEvent(event);
        m_Scene->OnEvent(event);
    }
}
