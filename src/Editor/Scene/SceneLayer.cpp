#include "SceneLayer.h"

#include <imgui.h>

namespace RenderingEngine
{
    SceneLayer::SceneLayer()
    {
        const auto& groundPlane = MeshImporter::CreatePlane(50.0f);
        auto& plane = m_Scene.Instantiate(groundPlane, glm::vec3(0, -10, 0));
        plane->SetName("Ground");

        auto planeTransform = plane->GetTransform();
        planeTransform->Rotation = glm::vec3(-90, 0, 0);

        const auto& baseballMesh = MeshImporter::CreateMesh(RESOURCES_PATH "Models/baseballbat_mesh.fbx");
        m_Scene.Instantiate(baseballMesh, glm::vec3(10, 6, 3));

        const auto& planeMesh = MeshImporter::CreatePlane(1.0f);
        m_Scene.Instantiate(planeMesh, glm::vec3(0));

        const auto& cubeMesh = MeshImporter::CreateCube(1.0f);
        m_Scene.Instantiate(cubeMesh, glm::vec3(0, 0, 2));

        const auto& sphereMesh = MeshImporter::CreateSphere(1.0f);
        m_Scene.Instantiate(sphereMesh, glm::vec3(0, 0, 5));
    }

    void SceneLayer::OnEveryUpdate(const Time deltaTime)
    {
        m_LastTime = deltaTime;

        m_Scene.OnEveryUpdate(deltaTime);
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
        ImGui::Begin("Renderer", &docking, windowFlags);
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
                {
                    docking = false;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Parameters");

        ImGui::Text("DeltaTime: %.3f (%.0fmil)", m_LastTime.GetSeconds(), m_LastTime.GetMilliseconds());

        ImGui::End();

        m_Scene.OnGUIUpdate();

        ImGui::End();
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Scene.GetSceneCamera().OnEvent(event);
    }
}
