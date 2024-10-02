#include "mxpch.h"
#include "SceneLayer.h"
#include "RendererEngine.h"

#include <imgui.h>

namespace RenderingEngine
{
    SceneLayer::SceneLayer()
        : m_Framebuffer(800, 600), m_Camera(glm::vec3(0, 0, -2))
    {
        const auto& m_BaseballMesh = MeshImporter::CreateMesh(RESOURCES_PATH "Models/baseballbat_mesh.fbx");
        m_Scene.Instantiate(m_BaseballMesh, glm::vec3(10, 6, 3));

        const auto& m_PlaneMesh = MeshImporter::CreatePlane(1.0f);
        m_Scene.Instantiate(m_PlaneMesh, glm::vec3(0));

        const auto& m_CubeMesh = MeshImporter::CreateCube(1.0f);
        m_Scene.Instantiate(m_CubeMesh, glm::vec3(0, 0, 2));

        const auto& m_SphereMesh = MeshImporter::CreateSphere(1.0f);
        m_Scene.Instantiate(m_SphereMesh, glm::vec3(0, 0, 5));
    }

    void SceneLayer::OnEveryUpdate(const Time deltaTime)
    {
        m_LastTime = deltaTime;

        m_Framebuffer.Bind();

        Renderer::OnEveryUpdate(m_Camera);
        m_Camera.EveryUpdate(deltaTime);

        Renderer::Clear(glm::vec4(0, 0, 0, 1));

        m_Scene.OnEveryUpdate();

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

        ImGui::Begin("Viewport");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto castSize = glm::i16vec2(viewportSize.x, viewportSize.y);

        if (castSize.x != 0 && castSize.y != 0)
        {
            if (glm::i16vec2(m_Framebuffer.GetWidth(), m_Framebuffer.GetHeight()) != castSize)
            {
                m_Camera.Resize(castSize.x, castSize.y);
                m_Framebuffer.Resize(castSize.x, castSize.y);
            }

            const uint32_t m_Texture = m_Framebuffer.GetTextureAttachment();
            ImGui::Image((void*)m_Texture, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, -1));
        }

        ImGui::End();

        m_Scene.OnGUIUpdate();
        ImGui::End();
    }

    void SceneLayer::OnEvent(Event& event)
    {
        m_Camera.OnEvent(event);
    }
}
