#include "GuiLayer.h"

#include "imgui.h"
#include "Core/Bootstrapper.h"

#include "GLFW/glfw3.h"
#include "backends/imgui_impl_opengl3.h"

namespace RenderingEngine
{
    void GuiLayer::Attach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void GuiLayer::Detach()
    {
    }

    void GuiLayer::EveryUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
        const Bootstrapper& boot = Bootstrapper::GetInstance();
        io.DisplaySize = ImVec2(boot.GetWindow().GetWidth(), boot.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time;
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GuiLayer::OnEvent(Event& e)
    {
    }
}
