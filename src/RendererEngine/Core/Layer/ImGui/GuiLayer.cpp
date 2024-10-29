#include "GuiLayer.h"
#include "RendererEngine/Core/Bootstrapper.h"

#include <backends/imgui_impl_opengl3.cpp> 
#include <backends/imgui_impl_glfw.cpp>
#include <ImGuizmo.h>

namespace RenderingEngine
{
    void GuiLayer::Attach()
    {
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        const Bootstrapper& boot = Bootstrapper::GetInstance();
        const auto window = static_cast<GLFWwindow*>(boot.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void GuiLayer::Detach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame(); 
    }

    void GuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        const Bootstrapper& boot = Bootstrapper::GetInstance();
        io.DisplaySize = ImVec2(boot.GetWindow().GetWidth(), boot.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            const auto window = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window);
        }
    }
}
