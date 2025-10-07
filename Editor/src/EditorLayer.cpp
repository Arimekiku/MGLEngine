#include "EditorLayer.h"

#include <imgui.h>
#include <portable_file_dialogs.h> 

#include "Persistence/SceneSerializer.h"

namespace Editor
{
    EditorLayer::EditorLayer(const Ptr<Window>& window)
    {
		m_Parent = window;

        //SETUP SCENE RENDERING
        const auto viewportPanel = Ptr<SceneViewportPanel>::Create("Viewport");
        const auto hierarchyPanel = Ptr<SceneHierarchyPanel>::Create("Hierarchy");
        const auto contentBrowserPanel = Ptr<SceneContentBrowserPanel>::Create("ContentBrowser");

        m_SceneRenderPanels = { viewportPanel, hierarchyPanel, contentBrowserPanel};

        //IMPORT ASSETS
		const auto& lightMaterial = MaterialImporter::AddMaterial("Light", "Shaders/areaLight.vert", "Shaders/areaLight.frag");
        const auto& defaultMaterial = MaterialImporter::AddMaterial("Default", "Shaders/default.vert", "Shaders/default.frag");
        const auto& skyboxMaterial = MaterialImporter::AddMaterial("Skybox", "Shaders/skybox.vert", "Shaders/skybox.frag");

        Image faceImage = Image(RESOURCES_PATH "Images/face.png");
        const auto& faceTexture = Ptr<Texture>::Create(faceImage);
        Image houseImage = Image(RESOURCES_PATH "Images/house.png");
        const auto& houseTexture = Ptr<Texture>::Create(houseImage);

        const auto& cubemapTexture = Ptr<Texture>::Create(
            RESOURCES_PATH "Images/Cubemap/nx.png",
            RESOURCES_PATH "Images/Cubemap/px.png",
            RESOURCES_PATH "Images/Cubemap/ny.png",
            RESOURCES_PATH "Images/Cubemap/py.png",
            RESOURCES_PATH "Images/Cubemap/nz.png",
            RESOURCES_PATH "Images/Cubemap/pz.png"
        );

        MeshImporter::CreatePlane();
        MeshImporter::CreateCube();
        MeshImporter::CreateSphere();

        //BIND ASSETS
        m_Scene = Ptr<Scene>::Create(m_Parent->GetEventDispatcher());
        m_SceneRenderer.SetContext(m_Scene);
        for (auto& panels : m_SceneRenderPanels)
            panels->SetContext(m_Scene);

        lightMaterial->BindVec3Uniform("u_LightColor", {1, 1, 1});

        defaultMaterial->AddTexture(houseTexture);
        defaultMaterial->BindVec3Uniform("u_LightColor", {1, 1, 1});
        defaultMaterial->BindVec3Uniform("u_LightPos", {1, 5, -5});
		defaultMaterial->BindMat4Uniform("u_lightViewProj", glm::mat4(1.0f));

        skyboxMaterial->AddTexture(cubemapTexture);
        skyboxMaterial->BindVec3Uniform("u_Albedo", glm::vec3(1));
        skyboxMaterial->BindIntUniform("u_Mode", 1);
        skyboxMaterial->SetCullMode(Front);

        SceneSerializer::Deserialize(m_Scene, RESOURCES_PATH "Persistence/Scene.scene");
        Renderer::Initialize();
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
            windowFlags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Renderer", &docking, windowFlags);
        ImGui::PopStyleVar();

        if (optFullscreen)
            ImGui::PopStyleVar(2);

        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockId = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dockId, ImVec2(0, 0), dockFlags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    m_Scene = Ptr<Scene>::Create(m_Parent->GetEventDispatcher());
                    m_SceneRenderer.SetContext(m_Scene);
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    const auto& file = pfd::open_file("Select Scene", pfd::path::home(), { "Scene File (*.scene)", "*.scene" }).result();

                    if (file.empty() == false)
                    {
                        m_Scene = Ptr<Scene>::Create(m_Parent->GetEventDispatcher());
                        m_SceneRenderer.SetContext(m_Scene);

                        SceneSerializer::Deserialize(m_Scene, file[0]);
                    }
                }

                if (ImGui::MenuItem("Save As...", "Ctrs+Shift+S"))
                {
                    const auto& file = pfd::save_file("Select Scene", pfd::path::home(), { "Scene File (*.scene)", "*.scene" }).result();

                    if (file.empty() == false)
                        SceneSerializer::Serialize(m_Scene, file);
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
                for (const auto& renderPanel : m_SceneRenderPanels)
                    renderPanel->DrawAsMenuItem();

#ifdef DEBUG
                ImGui::MenuItem("ImGuiDemo (Debug only)", "", &m_IsImGuiDemoPanel);
#endif
                
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Parameters");

        ImGui::Text("DeltaTime: %.3f (%.0f mil)", m_LastTime.GetSeconds(), m_LastTime.GetMilliseconds());
		ImGui::Text("FPS: %.1f", 1.0f / m_LastTime.GetSeconds());

        ImGui::End();

        for (const auto& sceneRenderPanel : m_SceneRenderPanels)
            sceneRenderPanel->DrawPanel();

#ifdef DEBUG
        if (m_IsImGuiDemoPanel)
            ImGui::ShowDemoWindow();
#endif

        ImGui::End();
    }
}
