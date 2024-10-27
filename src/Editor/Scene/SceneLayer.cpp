#include "SceneLayer.h"
#include "ComponentRegistry.h"
#include "Editor/Persistence/SceneSerializer.h"

#include <imgui.h>

namespace RenderingEngine
{
    SceneLayer::SceneLayer()
    {
        //ASSETS
		const auto& lightMaterial = MaterialImporter::GetMaterial("Light", "Shaders/areaLight.vert", "Shaders/areaLight.frag");
        auto& defaultMaterial = MaterialImporter::GetMaterial("Default", "Shaders/default.vert", "Shaders/default.frag");

        const auto& m_FaceTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/face.png");
        const auto& m_HouseTexture = std::make_shared<Texture>(RESOURCES_PATH "Images/house.png");

        MeshImporter::CreatePlane();
        MeshImporter::CreateCube();
        MeshImporter::CreateSphere();

        SceneSerializer serializer = SceneSerializer(m_Scene);
        serializer.Deserialize(RESOURCES_PATH "Persistence/Scene.scene");

        m_GuiRenderer.SetContext(m_Scene);

        Entity directLight = m_Scene->GetDirectionalLightEntity();
        auto& dirLightComp = directLight.GetComponent<DirectLightComponent>();

        lightMaterial->BindVec3Uniform("u_LightColor", {1, 1, 1});

        defaultMaterial->AddTexture(m_HouseTexture);
        defaultMaterial->BindTextureSlot("u_Texture", 0);
		defaultMaterial->BindTextureSlot("u_DepthMap", 1);
        defaultMaterial->BindVec3Uniform("u_LightColor", {1, 1, 1});
        defaultMaterial->BindVec3Uniform("u_LightPos", {0, 5, 5});
		defaultMaterial->BindMat4Uniform("u_lightViewProj", dirLightComp.GetDLMatrix());
    }

    void SceneLayer::OnEveryUpdate(const Time deltaTime)
    {
        m_LastTime = deltaTime;

        m_Scene->OnEveryUpdate(deltaTime);
        m_GuiRenderer.DrawDepthBuffer();
        m_GuiRenderer.DrawTextureBuffer();
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

        m_GuiRenderer.DrawViewport();
        m_GuiRenderer.DrawScenePanel();
        m_GuiRenderer.DrawInspectorPanel();
        m_GuiRenderer.DrawGuizmos();

        ImGui::End();
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_GuiRenderer.OnEvent(event);
        m_Scene->OnEvent(event);
    }
}
